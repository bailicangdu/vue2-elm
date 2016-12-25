#include "sass.hpp"
#include "bind.hpp"
#include "ast.hpp"
#include "context.hpp"
#include "eval.hpp"
#include <map>
#include <iostream>
#include <sstream>

namespace Sass {

  void bind(std::string type, std::string name, Parameters* ps, Arguments* as, Context* ctx, Env* env, Eval* eval)
  {
    std::string callee(type + " " + name);

    Listize listize(ctx->mem);
    std::map<std::string, Parameter*> param_map;

    for (size_t i = 0, L = as->length(); i < L; ++i) {
      if (auto str = dynamic_cast<String_Quoted*>((*as)[i]->value())) {
        // force optional quotes (only if needed)
        if (str->quote_mark()) {
          str->quote_mark('*');
          str->is_delayed(true);
        }
      }
    }

    // Set up a map to ensure named arguments refer to actual parameters. Also
    // eval each default value left-to-right, wrt env, populating env as we go.
    for (size_t i = 0, L = ps->length(); i < L; ++i) {
      Parameter*  p = (*ps)[i];
      param_map[p->name()] = p;
      // if (p->default_value()) {
      //   env->local_frame()[p->name()] = p->default_value()->perform(eval->with(env));
      // }
    }

    // plug in all args; if we have leftover params, deal with it later
    size_t ip = 0, LP = ps->length();
    size_t ia = 0, LA = as->length();
    while (ia < LA) {
      Argument* a = (*as)[ia];
      if (ip >= LP) {
        // skip empty rest arguments
        if (a->is_rest_argument()) {
          if (List* l = dynamic_cast<List*>(a->value())) {
            if (l->length() == 0) {
              ++ ia; continue;
            }
          }
        }
        std::stringstream msg;
        msg << "wrong number of arguments (" << LA << " for " << LP << ")";
        msg << " for `" << name << "'";
        return error(msg.str(), as->pstate());
      }
      Parameter* p = (*ps)[ip];

      // If the current parameter is the rest parameter, process and break the loop
      if (p->is_rest_parameter()) {
        // The next argument by coincidence provides a rest argument
        if (a->is_rest_argument()) {

          // We should always get a list for rest arguments
          if (List* rest = dynamic_cast<List*>(a->value())) {
              // create a new list object for wrapped items
              List* arglist = SASS_MEMORY_NEW(ctx->mem, List,
                                              p->pstate(),
                                              0,
                                              rest->separator(),
                                              true);
              // wrap each item from list as an argument
              for (Expression* item : rest->elements()) {
                if (Argument* arg = dynamic_cast<Argument*>(item)) {
                  (*arglist) << SASS_MEMORY_NEW(ctx->mem, Argument, *arg);
                } else {
                  (*arglist) << SASS_MEMORY_NEW(ctx->mem, Argument,
                                                item->pstate(),
                                                item,
                                                "",
                                                false,
                                                false);
                }
              }
              // assign new arglist to environment
              env->local_frame()[p->name()] = arglist;
            }
          // invalid state
          else {
            throw std::runtime_error("invalid state");
          }
        } else if (a->is_keyword_argument()) {

          // expand keyword arguments into their parameters
          List* arglist = SASS_MEMORY_NEW(ctx->mem, List, p->pstate(), 0, SASS_COMMA, true);
          env->local_frame()[p->name()] = arglist;
          Map* argmap = static_cast<Map*>(a->value());
          for (auto key : argmap->keys()) {
            std::string name = unquote(static_cast<String_Constant*>(key)->value());
            (*arglist) << SASS_MEMORY_NEW(ctx->mem, Argument,
                                          key->pstate(),
                                          argmap->at(key),
                                          "$" + name,
                                          false,
                                          false);
          }

        } else {

          // create a new list object for wrapped items
          List* arglist = SASS_MEMORY_NEW(ctx->mem, List,
                                          p->pstate(),
                                          0,
                                          SASS_COMMA,
                                          true);
          // consume the next args
          while (ia < LA) {
            // get and post inc
            a = (*as)[ia++];
            // maybe we have another list as argument
            List* ls = dynamic_cast<List*>(a->value());
            // skip any list completely if empty
            if (ls && ls->empty() && a->is_rest_argument()) continue;

            if (Argument* arg = dynamic_cast<Argument*>(a->value())) {
              (*arglist) << SASS_MEMORY_NEW(ctx->mem, Argument, *arg);
            }
            // check if we have rest argument
            else if (a->is_rest_argument()) {
              // preserve the list separator from rest args
              if (List* rest = dynamic_cast<List*>(a->value())) {
                arglist->separator(rest->separator());

                for (size_t i = 0, L = rest->size(); i < L; ++i) {
                  (*arglist) << SASS_MEMORY_NEW(ctx->mem, Argument,
                                                (*rest)[i]->pstate(),
                                                (*rest)[i],
                                                "",
                                                false,
                                                false);
                }
              }
              // no more arguments
              break;
            }
            // wrap all other value types into Argument
            else {
              (*arglist) << SASS_MEMORY_NEW(ctx->mem, Argument,
                                            a->pstate(),
                                            a->value(),
                                            a->name(),
                                            false,
                                            false);
            }
          }
          // assign new arglist to environment
          env->local_frame()[p->name()] = arglist;
        }
        // consumed parameter
        ++ip;
        // no more paramaters
        break;
      }

      // If the current argument is the rest argument, extract a value for processing
      else if (a->is_rest_argument()) {
        // normal param and rest arg
        List* arglist = static_cast<List*>(a->value());
        // empty rest arg - treat all args as default values
        if (!arglist->length()) {
          break;
        } else {
          if (arglist->length() > LP - ip && !ps->has_rest_parameter()) {
            int arg_count = (arglist->length() + LA - 1);
            std::stringstream msg;
            msg << callee << " takes " << LP;
            msg << (LP == 1 ? " argument" : " arguments");
            msg << " but " << arg_count;
            msg << (arg_count == 1 ? " was passed" : " were passed.");
            deprecated_bind(msg.str(), as->pstate());

            while (arglist->length() > LP - ip) {
              arglist->elements().erase(arglist->elements().end() - 1);
            }
          }
        }
        // otherwise move one of the rest args into the param, converting to argument if necessary
        if (!(a = dynamic_cast<Argument*>((*arglist)[0]))) {
          Expression* a_to_convert = (*arglist)[0];
          a = SASS_MEMORY_NEW(ctx->mem, Argument,
                              a_to_convert->pstate(),
                              a_to_convert,
                              "",
                              false,
                              false);
        }
        arglist->elements().erase(arglist->elements().begin());
        if (!arglist->length() || (!arglist->is_arglist() && ip + 1 == LP)) {
          ++ia;
        }

      } else if (a->is_keyword_argument()) {
        Map* argmap = static_cast<Map*>(a->value());

        for (auto key : argmap->keys()) {
          std::string name = "$" + unquote(static_cast<String_Constant*>(key)->value());

          if (!param_map.count(name)) {
            std::stringstream msg;
            msg << callee << " has no parameter named " << name;
            error(msg.str(), a->pstate());
          }
          env->local_frame()[name] = argmap->at(key);
        }
        ++ia;
        continue;
      } else {
        ++ia;
      }

      if (a->name().empty()) {
        if (env->has_local(p->name())) {
          std::stringstream msg;
          msg << "parameter " << p->name()
          << " provided more than once in call to " << callee;
          error(msg.str(), a->pstate());
        }
        // ordinal arg -- bind it to the next param
        env->local_frame()[p->name()] = a->value();
        ++ip;
      }
      else {
        // named arg -- bind it to the appropriately named param
        if (!param_map.count(a->name())) {
          std::stringstream msg;
          msg << callee << " has no parameter named " << a->name();
          error(msg.str(), a->pstate());
        }
        if (param_map[a->name()]->is_rest_parameter()) {
          std::stringstream msg;
          msg << "argument " << a->name() << " of " << callee
              << "cannot be used as named argument";
          error(msg.str(), a->pstate());
        }
        if (env->has_local(a->name())) {
          std::stringstream msg;
          msg << "parameter " << p->name()
              << "provided more than once in call to " << callee;
          error(msg.str(), a->pstate());
        }
        env->local_frame()[a->name()] = a->value();
      }
    }
    // EO while ia

    // If we make it here, we're out of args but may have leftover params.
    // That's only okay if they have default values, or were already bound by
    // named arguments, or if it's a single rest-param.
    for (size_t i = ip; i < LP; ++i) {
      Parameter* leftover = (*ps)[i];
      // cerr << "env for default params:" << endl;
      // env->print();
      // cerr << "********" << endl;
      if (!env->has_local(leftover->name())) {
        if (leftover->is_rest_parameter()) {
          env->local_frame()[leftover->name()] = SASS_MEMORY_NEW(ctx->mem, List,
                                                                   leftover->pstate(),
                                                                   0,
                                                                   SASS_COMMA,
                                                                   true);
        }
        else if (leftover->default_value()) {
          Expression* dv = leftover->default_value()->perform(eval);
          env->local_frame()[leftover->name()] = dv;
        }
        else {
          // param is unbound and has no default value -- error
          std::stringstream msg;
          msg << "required parameter " << leftover->name()
              << " is missing in call to " << callee;
          error(msg.str(), as->pstate());
        }
      }
    }

    return;
  }


}
