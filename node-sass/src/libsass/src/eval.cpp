#include "sass.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>

#include "file.hpp"
#include "eval.hpp"
#include "ast.hpp"
#include "bind.hpp"
#include "util.hpp"
#include "inspect.hpp"
#include "environment.hpp"
#include "position.hpp"
#include "sass/values.h"
#include "to_value.hpp"
#include "to_c.hpp"
#include "context.hpp"
#include "backtrace.hpp"
#include "lexer.hpp"
#include "prelexer.hpp"
#include "parser.hpp"
#include "expand.hpp"
#include "color_maps.hpp"

namespace Sass {

  inline double add(double x, double y) { return x + y; }
  inline double sub(double x, double y) { return x - y; }
  inline double mul(double x, double y) { return x * y; }
  inline double div(double x, double y) { return x / y; } // x/0 checked by caller
  inline double mod(double x, double y) { return std::abs(std::fmod(x, y)); } // x/0 checked by caller
  typedef double (*bop)(double, double);
  bop ops[Sass_OP::NUM_OPS] = {
    0, 0, // and, or
    0, 0, 0, 0, 0, 0, // eq, neq, gt, gte, lt, lte
    add, sub, mul, div, mod
  };

  Eval::Eval(Expand& exp)
  : exp(exp),
    ctx(exp.ctx)
  { }
  Eval::~Eval() { }

  Context& Eval::context()
  {
    return ctx;
  }

  Env* Eval::environment()
  {
    return exp.environment();
  }

  Selector_List* Eval::selector()
  {
    return exp.selector();
  }

  Backtrace* Eval::backtrace()
  {
    return exp.backtrace();
  }

  Expression* Eval::operator()(Block* b)
  {
    Expression* val = 0;
    for (size_t i = 0, L = b->length(); i < L; ++i) {
      val = (*b)[i]->perform(this);
      if (val) return val;
    }
    return val;
  }

  Expression* Eval::operator()(Assignment* a)
  {
    Env* env = exp.environment();
    std::string var(a->variable());
    if (a->is_global()) {
      if (a->is_default()) {
        if (env->has_global(var)) {
          Expression* e = dynamic_cast<Expression*>(env->get_global(var));
          if (!e || e->concrete_type() == Expression::NULL_VAL) {
            env->set_global(var, a->value()->perform(this));
          }
        }
        else {
          env->set_global(var, a->value()->perform(this));
        }
      }
      else {
        env->set_global(var, a->value()->perform(this));
      }
    }
    else if (a->is_default()) {
      if (env->has_lexical(var)) {
        auto cur = env;
        while (cur && cur->is_lexical()) {
          if (cur->has_local(var)) {
            if (AST_Node* node = cur->get_local(var)) {
              Expression* e = dynamic_cast<Expression*>(node);
              if (!e || e->concrete_type() == Expression::NULL_VAL) {
                cur->set_local(var, a->value()->perform(this));
              }
            }
            else {
              throw std::runtime_error("Env not in sync");
            }
            return 0;
          }
          cur = cur->parent();
        }
        throw std::runtime_error("Env not in sync");
      }
      else if (env->has_global(var)) {
        if (AST_Node* node = env->get_global(var)) {
          Expression* e = dynamic_cast<Expression*>(node);
          if (!e || e->concrete_type() == Expression::NULL_VAL) {
            env->set_global(var, a->value()->perform(this));
          }
        }
      }
      else if (env->is_lexical()) {
        env->set_local(var, a->value()->perform(this));
      }
      else {
        env->set_local(var, a->value()->perform(this));
      }
    }
    else {
      env->set_lexical(var, a->value()->perform(this));
    }
    return 0;
  }

  Expression* Eval::operator()(If* i)
  {
    Expression* rv = 0;
    Env env(exp.environment());
    exp.env_stack.push_back(&env);
    if (*i->predicate()->perform(this)) {
      rv = i->block()->perform(this);
    }
    else {
      Block* alt = i->alternative();
      if (alt) rv = alt->perform(this);
    }
    exp.env_stack.pop_back();
    return rv;
  }

  // For does not create a new env scope
  // But iteration vars are reset afterwards
  Expression* Eval::operator()(For* f)
  {
    std::string variable(f->variable());
    Expression* low = f->lower_bound()->perform(this);
    if (low->concrete_type() != Expression::NUMBER) {
      throw Exception::TypeMismatch(*low, "integer");
    }
    Expression* high = f->upper_bound()->perform(this);
    if (high->concrete_type() != Expression::NUMBER) {
      throw Exception::TypeMismatch(*high, "integer");
    }
    Number* sass_start = static_cast<Number*>(low);
    Number* sass_end = static_cast<Number*>(high);
    // check if units are valid for sequence
    if (sass_start->unit() != sass_end->unit()) {
      std::stringstream msg; msg << "Incompatible units: '"
        << sass_start->unit() << "' and '"
        << sass_end->unit() << "'.";
      error(msg.str(), low->pstate(), backtrace());
    }
    double start = sass_start->value();
    double end = sass_end->value();
    // only create iterator once in this environment
    Env env(environment(), true);
    exp.env_stack.push_back(&env);
    Number* it = SASS_MEMORY_NEW(env.mem, Number, low->pstate(), start, sass_end->unit());
    env.set_local(variable, it);
    Block* body = f->block();
    Expression* val = 0;
    if (start < end) {
      if (f->is_inclusive()) ++end;
      for (double i = start;
           i < end;
           ++i) {
        it->value(i);
        env.set_local(variable, it);
        val = body->perform(this);
        if (val) break;
      }
    } else {
      if (f->is_inclusive()) --end;
      for (double i = start;
           i > end;
           --i) {
        it->value(i);
        env.set_local(variable, it);
        val = body->perform(this);
        if (val) break;
      }
    }
    exp.env_stack.pop_back();
    return val;
  }

  // Eval does not create a new env scope
  // But iteration vars are reset afterwards
  Expression* Eval::operator()(Each* e)
  {
    std::vector<std::string> variables(e->variables());
    Expression* expr = e->list()->perform(this);
    Env env(environment(), true);
    exp.env_stack.push_back(&env);
    Vectorized<Expression*>* list = 0;
    Map* map = 0;
    if (expr->concrete_type() == Expression::MAP) {
      map = static_cast<Map*>(expr);
    }
    else if (Selector_List* ls = dynamic_cast<Selector_List*>(expr)) {
      Listize listize(ctx.mem);
      list = dynamic_cast<List*>(ls->perform(&listize));
    }
    else if (expr->concrete_type() != Expression::LIST) {
      list = SASS_MEMORY_NEW(ctx.mem, List, expr->pstate(), 1, SASS_COMMA);
      *list << expr;
    }
    else {
      list = static_cast<List*>(expr);
    }

    Block* body = e->block();
    Expression* val = 0;

    if (map) {
      for (auto key : map->keys()) {
        Expression* value = map->at(key);

        if (variables.size() == 1) {
          List* variable = SASS_MEMORY_NEW(ctx.mem, List, map->pstate(), 2, SASS_SPACE);
          *variable << key;
          *variable << value;
          env.set_local(variables[0], variable);
        } else {
          env.set_local(variables[0], key);
          env.set_local(variables[1], value);
        }

        val = body->perform(this);
        if (val) break;
      }
    }
    else {
      if (list->length() == 1 && dynamic_cast<Selector_List*>(list)) {
        list = dynamic_cast<Vectorized<Expression*>*>(list);
      }
      for (size_t i = 0, L = list->length(); i < L; ++i) {
        Expression* e = (*list)[i];
        // unwrap value if the expression is an argument
        if (Argument* arg = dynamic_cast<Argument*>(e)) e = arg->value();
        // check if we got passed a list of args (investigate)
        if (List* scalars = dynamic_cast<List*>(e)) {
          if (variables.size() == 1) {
            Expression* var = scalars;
            env.set_local(variables[0], var);
          } else {
            // XXX: this is never hit via spec tests
            for (size_t j = 0, K = variables.size(); j < K; ++j) {
              Expression* res = j >= scalars->length()
                ? SASS_MEMORY_NEW(ctx.mem, Null, expr->pstate())
                : (*scalars)[j];
              env.set_local(variables[j], res);
            }
          }
        } else {
          if (variables.size() > 0) {
            env.set_local(variables[0], e);
            for (size_t j = 1, K = variables.size(); j < K; ++j) {
              // XXX: this is never hit via spec tests
              Expression* res = SASS_MEMORY_NEW(ctx.mem, Null, expr->pstate());
              env.set_local(variables[j], res);
            }
          }
        }
        val = body->perform(this);
        if (val) break;
      }
    }
    exp.env_stack.pop_back();
    return val;
  }

  Expression* Eval::operator()(While* w)
  {
    Expression* pred = w->predicate();
    Block* body = w->block();
    Env env(environment(), true);
    exp.env_stack.push_back(&env);
    while (*pred->perform(this)) {
      Expression* val = body->perform(this);
      if (val) {
        exp.env_stack.pop_back();
        return val;
      }
    }
    exp.env_stack.pop_back();
    return 0;
  }

  Expression* Eval::operator()(Return* r)
  {
    return r->value()->perform(this);
  }

  Expression* Eval::operator()(Warning* w)
  {
    Sass_Output_Style outstyle = ctx.c_options.output_style;
    ctx.c_options.output_style = NESTED;
    Expression* message = w->message()->perform(this);
    Env* env = exp.environment();

    // try to use generic function
    if (env->has("@warn[f]")) {

      Definition* def = static_cast<Definition*>((*env)["@warn[f]"]);
      // Block*          body   = def->block();
      // Native_Function func   = def->native_function();
      Sass_Function_Entry c_function = def->c_function();
      Sass_Function_Fn c_func = sass_function_get_function(c_function);

      To_C to_c;
      union Sass_Value* c_args = sass_make_list(1, SASS_COMMA);
      sass_list_set_value(c_args, 0, message->perform(&to_c));
      union Sass_Value* c_val = c_func(c_args, c_function, ctx.c_compiler);
      ctx.c_options.output_style = outstyle;
      sass_delete_value(c_args);
      sass_delete_value(c_val);
      return 0;

    }

    std::string result(unquote(message->to_sass()));
    Backtrace top(backtrace(), w->pstate(), "");
    std::cerr << "WARNING: " << result;
    std::cerr << top.to_string();
    std::cerr << std::endl << std::endl;
    ctx.c_options.output_style = outstyle;
    return 0;
  }

  Expression* Eval::operator()(Error* e)
  {
    Sass_Output_Style outstyle = ctx.c_options.output_style;
    ctx.c_options.output_style = NESTED;
    Expression* message = e->message()->perform(this);
    Env* env = exp.environment();

    // try to use generic function
    if (env->has("@error[f]")) {

      Definition* def = static_cast<Definition*>((*env)["@error[f]"]);
      // Block*          body   = def->block();
      // Native_Function func   = def->native_function();
      Sass_Function_Entry c_function = def->c_function();
      Sass_Function_Fn c_func = sass_function_get_function(c_function);

      To_C to_c;
      union Sass_Value* c_args = sass_make_list(1, SASS_COMMA);
      sass_list_set_value(c_args, 0, message->perform(&to_c));
      union Sass_Value* c_val = c_func(c_args, c_function, ctx.c_compiler);
      ctx.c_options.output_style = outstyle;
      sass_delete_value(c_args);
      sass_delete_value(c_val);
      return 0;

    }

    std::string result(unquote(message->to_sass()));
    ctx.c_options.output_style = outstyle;
    error(result, e->pstate());
    return 0;
  }

  Expression* Eval::operator()(Debug* d)
  {
    Sass_Output_Style outstyle = ctx.c_options.output_style;
    ctx.c_options.output_style = NESTED;
    Expression* message = d->value()->perform(this);
    Env* env = exp.environment();

    // try to use generic function
    if (env->has("@debug[f]")) {

      Definition* def = static_cast<Definition*>((*env)["@debug[f]"]);
      // Block*          body   = def->block();
      // Native_Function func   = def->native_function();
      Sass_Function_Entry c_function = def->c_function();
      Sass_Function_Fn c_func = sass_function_get_function(c_function);

      To_C to_c;
      union Sass_Value* c_args = sass_make_list(1, SASS_COMMA);
      sass_list_set_value(c_args, 0, message->perform(&to_c));
      union Sass_Value* c_val = c_func(c_args, c_function, ctx.c_compiler);
      ctx.c_options.output_style = outstyle;
      sass_delete_value(c_args);
      sass_delete_value(c_val);
      return 0;

    }

    std::string cwd(ctx.cwd());
    std::string result(unquote(message->to_sass()));
    std::string abs_path(Sass::File::rel2abs(d->pstate().path, cwd, cwd));
    std::string rel_path(Sass::File::abs2rel(d->pstate().path, cwd, cwd));
    std::string output_path(Sass::File::path_for_console(rel_path, abs_path, d->pstate().path));
    ctx.c_options.output_style = outstyle;

    std::cerr << output_path << ":" << d->pstate().line+1 << " DEBUG: " << result;
    std::cerr << std::endl;
    return 0;
  }

  Expression* Eval::operator()(List* l)
  {
    // special case for unevaluated map
    if (l->separator() == SASS_HASH) {
      Map* lm = SASS_MEMORY_NEW(ctx.mem, Map,
                                l->pstate(),
                                l->length() / 2);
      for (size_t i = 0, L = l->length(); i < L; i += 2)
      {
        Expression* key = (*l)[i+0]->perform(this);
        Expression* val = (*l)[i+1]->perform(this);
        // make sure the color key never displays its real name
        key->is_delayed(true);
        *lm << std::make_pair(key, val);
      }
      if (lm->has_duplicate_key()) {
        throw Exception::DuplicateKeyError(*lm, *l);
      }

      lm->is_interpolant(l->is_interpolant());
      return lm->perform(this);
    }
    // check if we should expand it
    if (l->is_expanded()) return l;
    // regular case for unevaluated lists
    List* ll = SASS_MEMORY_NEW(ctx.mem, List,
                               l->pstate(),
                               l->length(),
                               l->separator(),
                               l->is_arglist());
    for (size_t i = 0, L = l->length(); i < L; ++i) {
      *ll << (*l)[i]->perform(this);
    }
    ll->is_interpolant(l->is_interpolant());
    ll->from_selector(l->from_selector());
    ll->is_expanded(true);
    return ll;
  }

  Expression* Eval::operator()(Map* m)
  {
    if (m->is_expanded()) return m;

    // make sure we're not starting with duplicate keys.
    // the duplicate key state will have been set in the parser phase.
    if (m->has_duplicate_key()) {
      throw Exception::DuplicateKeyError(*m, *m);
    }

    Map* mm = SASS_MEMORY_NEW(ctx.mem, Map,
                                m->pstate(),
                                m->length());
    for (auto key : m->keys()) {
      Expression* ex_key = key->perform(this);
      Expression* ex_val = m->at(key)->perform(this);
      *mm << std::make_pair(ex_key, ex_val);
    }

    // check the evaluated keys aren't duplicates.
    if (mm->has_duplicate_key()) {
      throw Exception::DuplicateKeyError(*mm, *m);
    }

    mm->is_expanded(true);
    return mm;
  }

  Expression* Eval::operator()(Binary_Expression* b)
  {

    String_Schema* ret_schema = 0;
    enum Sass_OP op_type = b->type();

    // don't eval delayed expressions (the '/' when used as a separator)
    if (op_type == Sass_OP::DIV && b->is_delayed()) {
      b->right(b->right()->perform(this));
      b->left(b->left()->perform(this));
      return b;
    }

    // only the last item will be used to eval the binary expression
    if (String_Schema* s_l = dynamic_cast<String_Schema*>(b->left())) {
      if (!s_l->has_interpolant() && (!s_l->is_right_interpolant())) {
        ret_schema = SASS_MEMORY_NEW(ctx.mem, String_Schema, s_l->pstate());
        Binary_Expression* bin_ex = SASS_MEMORY_NEW(ctx.mem, Binary_Expression, b->pstate(),
                                                    b->op(), s_l->last(), b->right());
        bin_ex->is_delayed(b->left()->is_delayed() || b->right()->is_delayed());
        // bin_ex->is_interpolant(b->left()->is_interpolant());
        for (size_t i = 0; i < s_l->length() - 1; ++i) {
          *ret_schema << s_l->at(i)->perform(this);
        }
        *ret_schema << bin_ex->perform(this);
        return ret_schema->perform(this);
      }
    }
    if (String_Schema* s_r = dynamic_cast<String_Schema*>(b->right())) {
      if (!s_r->has_interpolant() && (!s_r->is_left_interpolant() || op_type == Sass_OP::DIV)) {
        ret_schema = SASS_MEMORY_NEW(ctx.mem, String_Schema, s_r->pstate());
        Binary_Expression* bin_ex = SASS_MEMORY_NEW(ctx.mem, Binary_Expression, b->pstate(),
                                                    b->op(), b->left(), s_r->first());
        bin_ex->is_delayed(b->left()->is_delayed() || b->right()->is_delayed());
        // if (op_type == Sass_OP::SUB && b->is_right_interpolant()) bin_ex->is_interpolant(true);
        *ret_schema << bin_ex->perform(this);
        for (size_t i = 1; i < s_r->length(); ++i) {
          *ret_schema << s_r->at(i)->perform(this);
        }
        return ret_schema->perform(this);
      }
    }


    // don't eval delayed expressions (the '/' when used as a separator)
    if (op_type == Sass_OP::DIV && b->is_delayed()) {
      b->right(b->right()->perform(this));
      b->left(b->left()->perform(this));
      return b;
    }

    // b->is_delayed(false);
    Expression* lhs = b->left();
    Expression* rhs = b->right();

    // bool delay_lhs = false;
    // bool delay_rhs = false;

    if (String_Schema* schema = dynamic_cast<String_Schema*>(lhs)) {
      if (schema->is_right_interpolant()) {
        b->is_delayed(true);
        // delay_lhs = true;
      }
    }
    if (String_Schema* schema = dynamic_cast<String_Schema*>(rhs)) {
      if (schema->is_left_interpolant()) {
        b->is_delayed(true);
        // delay_rhs = true;
      }
    }

    // maybe fully evaluate structure
    if (op_type == Sass_OP::EQ ||
        op_type == Sass_OP::NEQ ||
        op_type == Sass_OP::GT ||
        op_type == Sass_OP::GTE ||
        op_type == Sass_OP::LT ||
        op_type == Sass_OP::LTE)
    {

      if (String_Schema* schema = dynamic_cast<String_Schema*>(lhs)) {
        if (schema->has_interpolants()) {
          b->is_delayed(true);
        }
      }
      if (String_Schema* schema = dynamic_cast<String_Schema*>(rhs)) {
        if (schema->has_interpolants()) {
          b->is_delayed(true);
        }
      }
      lhs->is_expanded(false);
      lhs->set_delayed(false);
      lhs = lhs->perform(this);
      lhs->is_expanded(false);
      lhs->set_delayed(false);
      lhs = lhs->perform(this);
      rhs->is_expanded(false);
      rhs->set_delayed(false);
      rhs = rhs->perform(this);
      rhs->is_expanded(false);
      rhs->set_delayed(false);
      rhs = rhs->perform(this);
    }
    else
    {
      // rhs->set_delayed(false);
      // rhs = rhs->perform(this);
    }

    // if one of the operands is a '/' then make sure it's evaluated
    lhs = lhs->perform(this);
    lhs->is_delayed(false);
    while (typeid(*lhs) == typeid(Binary_Expression)) {
      Binary_Expression* lhs_ex = static_cast<Binary_Expression*>(lhs);
      if (lhs_ex->type() == Sass_OP::DIV && lhs_ex->is_delayed()) break;
      lhs = Eval::operator()(lhs_ex);
    }

    switch (op_type) {
      case Sass_OP::AND:
        return *lhs ? b->right()->perform(this) : lhs;
        break;

      case Sass_OP::OR:
        return *lhs ? lhs : b->right()->perform(this);
        break;

      default:
        break;
    }
    // not a logical connective, so go ahead and eval the rhs
    rhs = rhs->perform(this);

    // upgrade string to number if possible (issue #948)
    if (op_type == Sass_OP::DIV || op_type == Sass_OP::MUL) {
      if (String_Constant* str = dynamic_cast<String_Constant*>(rhs)) {
        std::string value(str->value());
        const char* start = value.c_str();
        if (Prelexer::sequence < Prelexer::number >(start) != 0) {
          rhs = SASS_MEMORY_NEW(ctx.mem, Textual, rhs->pstate(), Textual::DIMENSION, str->value());
          rhs->is_delayed(false); rhs = rhs->perform(this);
        }
      }
    }


    Expression::Concrete_Type l_type = lhs->concrete_type();
    Expression::Concrete_Type r_type = rhs->concrete_type();

    // Is one of the operands an interpolant?
    String_Schema* s1 = dynamic_cast<String_Schema*>(b->left());
    String_Schema* s2 = dynamic_cast<String_Schema*>(b->right());
    Binary_Expression* b1 = dynamic_cast<Binary_Expression*>(b->left());
    Binary_Expression* b2 = dynamic_cast<Binary_Expression*>(b->right());

    bool schema_op = false;

    bool force_delay = (s2 && s2->is_left_interpolant()) ||
                       (s1 && s1->is_right_interpolant()) ||
                       (b1 && b1->is_right_interpolant()) ||
                       (b2 && b2->is_left_interpolant());

    if ((s1 && s1->has_interpolants()) || (s2 && s2->has_interpolants()) || force_delay)
    {
      // If possible upgrade LHS to a number
      if (op_type == Sass_OP::DIV || op_type == Sass_OP::MUL || op_type == Sass_OP::MOD || op_type == Sass_OP::ADD || op_type == Sass_OP::SUB ||
          op_type == Sass_OP::EQ) {
        if (String_Constant* str = dynamic_cast<String_Constant*>(lhs)) {
          std::string value(str->value());
          const char* start = value.c_str();
          if (Prelexer::sequence < Prelexer::dimension, Prelexer::end_of_file >(start) != 0) {
            lhs = SASS_MEMORY_NEW(ctx.mem, Textual, lhs->pstate(), Textual::DIMENSION, str->value());
            lhs->is_delayed(false); lhs = lhs->perform(this);
          }
        }
        if (String_Constant* str = dynamic_cast<String_Constant*>(rhs)) {
          std::string value(str->value());
          const char* start = value.c_str();
          if (Prelexer::sequence < Prelexer::number >(start) != 0) {
            rhs = SASS_MEMORY_NEW(ctx.mem, Textual, rhs->pstate(), Textual::DIMENSION, str->value());
            rhs->is_delayed(false); rhs = rhs->perform(this);
          }
        }
      }

      To_Value to_value(ctx, ctx.mem);
      Value* v_l = dynamic_cast<Value*>(lhs->perform(&to_value));
      Value* v_r = dynamic_cast<Value*>(rhs->perform(&to_value));
      l_type = lhs->concrete_type();
      r_type = rhs->concrete_type();

      if (s2 && s2->has_interpolants() && s2->length()) {
        Textual* front = dynamic_cast<Textual*>(s2->elements().front());
        if (front && !front->is_interpolant())
        {
          // XXX: this is never hit via spec tests
          schema_op = true;
          rhs = front->perform(this);
        }
      }

      if (force_delay) {
        std::string str("");
        str += v_l->to_string(ctx.c_options);
        if (b->op().ws_before) str += " ";
        str += b->separator();
        if (b->op().ws_after) str += " ";
        str += v_r->to_string(ctx.c_options);
        String_Constant* val = SASS_MEMORY_NEW(ctx.mem, String_Constant, lhs->pstate(), str);
        val->is_interpolant(b->left()->has_interpolant());
        return val;
      }
    }

    // see if it's a relational expression
    try {
      switch(op_type) {
        case Sass_OP::EQ:  return SASS_MEMORY_NEW(ctx.mem, Boolean, b->pstate(), eq(lhs, rhs));
        case Sass_OP::NEQ: return SASS_MEMORY_NEW(ctx.mem, Boolean, b->pstate(), !eq(lhs, rhs));
        case Sass_OP::GT:  return SASS_MEMORY_NEW(ctx.mem, Boolean, b->pstate(), !lt(lhs, rhs, "gt") && !eq(lhs, rhs));
        case Sass_OP::GTE: return SASS_MEMORY_NEW(ctx.mem, Boolean, b->pstate(), !lt(lhs, rhs, "gte"));
        case Sass_OP::LT:  return SASS_MEMORY_NEW(ctx.mem, Boolean, b->pstate(), lt(lhs, rhs, "lt"));
        case Sass_OP::LTE: return SASS_MEMORY_NEW(ctx.mem, Boolean, b->pstate(), lt(lhs, rhs, "lte") || eq(lhs, rhs));
        default:                     break;
      }
    }
    catch (Exception::OperationError& err)
    {
      // throw Exception::Base(b->pstate(), err.what());
      throw Exception::SassValueError(b->pstate(), err);
    }

    l_type = lhs->concrete_type();
    r_type = rhs->concrete_type();

    // ToDo: throw error in op functions
    // ToDo: then catch and re-throw them
    Expression* rv = 0;
    try {
      ParserState pstate(b->pstate());
      if (l_type == Expression::NUMBER && r_type == Expression::NUMBER) {
        const Number* l_n = dynamic_cast<const Number*>(lhs);
        const Number* r_n = dynamic_cast<const Number*>(rhs);
        rv = op_numbers(ctx.mem, op_type, *l_n, *r_n, ctx.c_options, &pstate);
      }
      else if (l_type == Expression::NUMBER && r_type == Expression::COLOR) {
        const Number* l_n = dynamic_cast<const Number*>(lhs);
        const Color* r_c = dynamic_cast<const Color*>(rhs);
        rv = op_number_color(ctx.mem, op_type, *l_n, *r_c, ctx.c_options, &pstate);
      }
      else if (l_type == Expression::COLOR && r_type == Expression::NUMBER) {
        const Color* l_c = dynamic_cast<const Color*>(lhs);
        const Number* r_n = dynamic_cast<const Number*>(rhs);
        rv = op_color_number(ctx.mem, op_type, *l_c, *r_n, ctx.c_options, &pstate);
      }
      else if (l_type == Expression::COLOR && r_type == Expression::COLOR) {
        const Color* l_c = dynamic_cast<const Color*>(lhs);
        const Color* r_c = dynamic_cast<const Color*>(rhs);
        rv = op_colors(ctx.mem, op_type, *l_c, *r_c, ctx.c_options, &pstate);
      }
      else {
        To_Value to_value(ctx, ctx.mem);
        Value* v_l = dynamic_cast<Value*>(lhs->perform(&to_value));
        Value* v_r = dynamic_cast<Value*>(rhs->perform(&to_value));
        bool interpolant = b->is_right_interpolant() ||
                           b->is_left_interpolant() ||
                           b->is_interpolant();
        if (op_type == Sass_OP::SUB) interpolant = false;
        // if (op_type == Sass_OP::DIV) interpolant = true;
        Value* ex = op_strings(ctx.mem, b->op(), *v_l, *v_r, ctx.c_options, &pstate, !interpolant); // pass true to compress
        if (String_Constant* str = dynamic_cast<String_Constant*>(ex))
        {
          if (str->concrete_type() == Expression::STRING)
          {
            String_Constant* lstr = dynamic_cast<String_Constant*>(lhs);
            String_Constant* rstr = dynamic_cast<String_Constant*>(rhs);
            if (op_type != Sass_OP::SUB) {
              if (String_Constant* org = lstr ? lstr : rstr)
              { str->quote_mark(org->quote_mark()); }
            }
          }
        }
        ex->is_interpolant(b->is_interpolant());
        rv = ex;
      }
    }
    catch (Exception::OperationError& err)
    {
      // throw Exception::Base(b->pstate(), err.what());
      throw Exception::SassValueError(b->pstate(), err);
    }

    if (rv) {
      if (schema_op) {
        // XXX: this is never hit via spec tests
        (*s2)[0] = rv;
        rv = s2->perform(this);
      }
    }
    return rv;

  }

  Expression* Eval::operator()(Unary_Expression* u)
  {
    Expression* operand = u->operand()->perform(this);
    if (u->type() == Unary_Expression::NOT) {
      Boolean* result = SASS_MEMORY_NEW(ctx.mem, Boolean, u->pstate(), (bool)*operand);
      result->value(!result->value());
      return result;
    }
    else if (operand->concrete_type() == Expression::NUMBER) {
      Number* result = SASS_MEMORY_NEW(ctx.mem, Number, *static_cast<Number*>(operand));
      result->value(u->type() == Unary_Expression::MINUS
                    ? -result->value()
                    :  result->value());
      return result;
    }
    else {
      // Special cases: +/- variables which evaluate to null ouput just +/-,
      // but +/- null itself outputs the string
      if (operand->concrete_type() == Expression::NULL_VAL && dynamic_cast<Variable*>(u->operand())) {
        u->operand(SASS_MEMORY_NEW(ctx.mem, String_Quoted, u->pstate(), ""));
      }
      else u->operand(operand);
      String_Constant* result = SASS_MEMORY_NEW(ctx.mem, String_Quoted,
                                                  u->pstate(),
                                                  u->inspect());
      return result;
    }
    // unreachable
    return u;
  }

  Expression* Eval::operator()(Function_Call* c)
  {
    if (backtrace()->parent != NULL && backtrace()->depth() > Constants::MaxCallStack) {
        // XXX: this is never hit via spec tests
        std::ostringstream stm;
        stm << "Stack depth exceeded max of " << Constants::MaxCallStack;
        error(stm.str(), c->pstate(), backtrace());
    }
    std::string name(Util::normalize_underscores(c->name()));
    std::string full_name(name + "[f]");
    Arguments* args = SASS_MEMORY_NEW(ctx.mem, Arguments, *c->arguments());

    // handle call here if valid arg
    // otherwise we eval arguments to early
    if (name == "call" && args->length() > 0) {
      Expression* redirect = args->at(0)->perform(this);
      args->erase(args->begin());
      Function_Call* lit = SASS_MEMORY_NEW(ctx.mem, Function_Call,
                                           c->pstate(),
                                           unquote(redirect->to_string()),
                                           args);
      return operator()(lit);
    }

    Env* env = environment();
    if (!env->has(full_name)) {
      if (!env->has("*[f]")) {
        // just pass it through as a literal
        for (Argument* arg : *args) {
          if (Binary_Expression* b = dynamic_cast<Binary_Expression*>(arg->value())) {
            b->reset_whitespace();
            arg->is_delayed(b->can_delay()); // delay
          }
        }
        args = static_cast<Arguments*>(args->perform(this));
        Function_Call* lit = SASS_MEMORY_NEW(ctx.mem, Function_Call,
                                             c->pstate(),
                                             c->name(),
                                             args);
        if (args->has_named_arguments()) {
          error("Function " + c->name() + " doesn't support keyword arguments", c->pstate());
        }
        String_Quoted* str = SASS_MEMORY_NEW(ctx.mem, String_Quoted,
                                             c->pstate(),
                                             lit->to_string(ctx.c_options));
        str->is_interpolant(c->is_interpolant());
        return str;
      } else {
        // call generic function
        full_name = "*[f]";
      }
    }

    if (full_name != "if[f]") {
      args = static_cast<Arguments*>(args->perform(this));
    }

    Definition* def = static_cast<Definition*>((*env)[full_name]);

    if (def->is_overload_stub()) {
      std::stringstream ss;
      ss << full_name
         << args->length();
      full_name = ss.str();
      std::string resolved_name(full_name);
      if (!env->has(resolved_name)) error("overloaded function `" + std::string(c->name()) + "` given wrong number of arguments", c->pstate());
      def = static_cast<Definition*>((*env)[resolved_name]);
    }

    Expression*     result = c;
    Block*          body   = def->block();
    Native_Function func   = def->native_function();
    Sass_Function_Entry c_function = def->c_function();

    Parameters* params = def->parameters();
    Env fn_env(def->environment());
    exp.env_stack.push_back(&fn_env);

    if (func || body) {
      bind(std::string("Function"), c->name(), params, args, &ctx, &fn_env, this);
      Backtrace here(backtrace(), c->pstate(), ", in function `" + c->name() + "`");
      exp.backtrace_stack.push_back(&here);
      // eval the body if user-defined or special, invoke underlying CPP function if native
      if (body && !Prelexer::re_special_fun(c->name().c_str())) { result = body->perform(this); }
      else if (func) { result = func(fn_env, *env, ctx, def->signature(), c->pstate(), backtrace()); }
      if (!result) error(std::string("Function ") + c->name() + " did not return a value", c->pstate());
      exp.backtrace_stack.pop_back();
    }

    // else if it's a user-defined c function
    // convert call into C-API compatible form
    else if (c_function) {
      Sass_Function_Fn c_func = sass_function_get_function(c_function);
      if (full_name == "*[f]") {
        String_Quoted *str = SASS_MEMORY_NEW(ctx.mem, String_Quoted, c->pstate(), c->name());
        Arguments* new_args = SASS_MEMORY_NEW(ctx.mem, Arguments, c->pstate());
        *new_args << SASS_MEMORY_NEW(ctx.mem, Argument, c->pstate(), str);
        *new_args += args;
        args = new_args;
      }

      // populates env with default values for params
      std::string ff(c->name());
      bind(std::string("Function"), c->name(), params, args, &ctx, &fn_env, this);

      Backtrace here(backtrace(), c->pstate(), ", in function `" + c->name() + "`");
      exp.backtrace_stack.push_back(&here);

      To_C to_c;
      union Sass_Value* c_args = sass_make_list(params[0].length(), SASS_COMMA);
      for(size_t i = 0; i < params[0].length(); i++) {
        std::string key = params[0][i]->name();
        AST_Node* node = fn_env.get_local(key);
        Expression* arg = static_cast<Expression*>(node);
        sass_list_set_value(c_args, i, arg->perform(&to_c));
      }
      union Sass_Value* c_val = c_func(c_args, c_function, ctx.c_compiler);
      if (sass_value_get_tag(c_val) == SASS_ERROR) {
        error("error in C function " + c->name() + ": " + sass_error_get_message(c_val), c->pstate(), backtrace());
      } else if (sass_value_get_tag(c_val) == SASS_WARNING) {
        error("warning in C function " + c->name() + ": " + sass_warning_get_message(c_val), c->pstate(), backtrace());
      }
      result = cval_to_astnode(ctx.mem, c_val, ctx, backtrace(), c->pstate());

      exp.backtrace_stack.pop_back();
      sass_delete_value(c_args);
      if (c_val != c_args)
        sass_delete_value(c_val);
    }

    // link back to function definition
    // only do this for custom functions
    if (result->pstate().file == std::string::npos)
      result->pstate(c->pstate());

    result->is_delayed(result->concrete_type() == Expression::STRING);
    if (!result->is_delayed()) result = result->perform(this);
    result->is_interpolant(c->is_interpolant());
    exp.env_stack.pop_back();
    return result;
  }

  Expression* Eval::operator()(Function_Call_Schema* s)
  {
    Expression* evaluated_name = s->name()->perform(this);
    Expression* evaluated_args = s->arguments()->perform(this);
    String_Schema* ss = SASS_MEMORY_NEW(ctx.mem, String_Schema, s->pstate(), 2);
    (*ss) << evaluated_name << evaluated_args;
    return ss->perform(this);
  }

  Expression* Eval::operator()(Variable* v)
  {
    std::string name(v->name());
    Expression* value = 0;
    Env* env = environment();
    if (env->has(name)) value = static_cast<Expression*>((*env)[name]);
    else error("Undefined variable: \"" + v->name() + "\".", v->pstate());
    if (typeid(*value) == typeid(Argument)) value = static_cast<Argument*>(value)->value();

    // behave according to as ruby sass (add leading zero)
    if (value->concrete_type() == Expression::NUMBER) {
      value = SASS_MEMORY_NEW(ctx.mem, Number, *static_cast<Number*>(value));
      static_cast<Number*>(value)->zero(true);
    }
    else if (value->concrete_type() == Expression::STRING) {
      if (auto str = dynamic_cast<String_Quoted*>(value)) {
        value = SASS_MEMORY_NEW(ctx.mem, String_Quoted, *str);
      } else if (auto str = dynamic_cast<String_Constant*>(value)) {
        value = SASS_MEMORY_NEW(ctx.mem, String_Quoted, str->pstate(), str->value());
      }
    }
    else if (value->concrete_type() == Expression::LIST) {
      value = SASS_MEMORY_NEW(ctx.mem, List, *static_cast<List*>(value));
    }
    else if (value->concrete_type() == Expression::MAP) {
      value = SASS_MEMORY_NEW(ctx.mem, Map, *static_cast<Map*>(value));
    }
    else if (value->concrete_type() == Expression::BOOLEAN) {
      value = SASS_MEMORY_NEW(ctx.mem, Boolean, *static_cast<Boolean*>(value));
    }
    else if (value->concrete_type() == Expression::COLOR) {
      value = SASS_MEMORY_NEW(ctx.mem, Color, *static_cast<Color*>(value));
    }
    else if (value->concrete_type() == Expression::NULL_VAL) {
      value = SASS_MEMORY_NEW(ctx.mem, Null, value->pstate());
    }
    else if (value->concrete_type() == Expression::SELECTOR) {
      value = value->perform(this); // ->perform(&listize);
    }

    value->is_interpolant(v->is_interpolant());
    value->is_expanded(false);
    return value->perform(this);
  }

  Expression* Eval::operator()(Textual* t)
  {
    using Prelexer::number;
    Expression* result = 0;
    size_t L = t->value().length();
    bool zero = !( (L > 0 && t->value().substr(0, 1) == ".") ||
                   (L > 1 && t->value().substr(0, 2) == "0.") ||
                   (L > 1 && t->value().substr(0, 2) == "-.")  ||
                   (L > 2 && t->value().substr(0, 3) == "-0.")
                 );

    const std::string& text = t->value();
    size_t num_pos = text.find_first_not_of(" \n\r\t");
    if (num_pos == std::string::npos) num_pos = text.length();
    size_t unit_pos = text.find_first_not_of("-+0123456789.", num_pos);
    if (unit_pos == std::string::npos) unit_pos = text.length();
    const std::string& num = text.substr(num_pos, unit_pos - num_pos);

    switch (t->type())
    {
      case Textual::NUMBER:
        result = SASS_MEMORY_NEW(ctx.mem, Number,
                                 t->pstate(),
                                 sass_atof(num.c_str()),
                                 "",
                                 zero);
        break;
      case Textual::PERCENTAGE:
        result = SASS_MEMORY_NEW(ctx.mem, Number,
                                 t->pstate(),
                                 sass_atof(num.c_str()),
                                 "%",
                                 true);
        break;
      case Textual::DIMENSION:
        result = SASS_MEMORY_NEW(ctx.mem, Number,
                                 t->pstate(),
                                 sass_atof(num.c_str()),
                                 Token(number(text.c_str())),
                                 zero);
        break;
      case Textual::HEX: {
        if (t->value().substr(0, 1) != "#") {
          result = SASS_MEMORY_NEW(ctx.mem, String_Quoted, t->pstate(), t->value());
          break;
        }
        std::string hext(t->value().substr(1)); // chop off the '#'
        if (hext.length() == 6) {
          std::string r(hext.substr(0,2));
          std::string g(hext.substr(2,2));
          std::string b(hext.substr(4,2));
          result = SASS_MEMORY_NEW(ctx.mem, Color,
                                   t->pstate(),
                                   static_cast<double>(strtol(r.c_str(), NULL, 16)),
                                   static_cast<double>(strtol(g.c_str(), NULL, 16)),
                                   static_cast<double>(strtol(b.c_str(), NULL, 16)),
                                   1, // alpha channel
                                   t->value());
        }
        else {
          result = SASS_MEMORY_NEW(ctx.mem, Color,
                                   t->pstate(),
                                   static_cast<double>(strtol(std::string(2,hext[0]).c_str(), NULL, 16)),
                                   static_cast<double>(strtol(std::string(2,hext[1]).c_str(), NULL, 16)),
                                   static_cast<double>(strtol(std::string(2,hext[2]).c_str(), NULL, 16)),
                                   1, // alpha channel
                                   t->value());
        }
      } break;
    }
    result->is_interpolant(t->is_interpolant());
    return result;
  }

  Expression* Eval::operator()(Number* n)
  {
    return n;
  }

  Expression* Eval::operator()(Boolean* b)
  {
    return b;
  }

  void Eval::interpolation(Context& ctx, std::string& res, Expression* ex, bool into_quotes, bool was_itpl) {

    bool needs_closing_brace = false;

    if (Arguments* args = dynamic_cast<Arguments*>(ex)) {
      List* ll = SASS_MEMORY_NEW(ctx.mem, List, args->pstate(), 0, SASS_COMMA);
      for(auto arg : *args) {
        *ll << arg->value();
      }
      ll->is_interpolant(args->is_interpolant());
      needs_closing_brace = true;
      res += "(";
      ex = ll;
    }
    if (Number* nr = dynamic_cast<Number*>(ex)) {
      if (!nr->is_valid_css_unit()) {
        throw Exception::InvalidValue(*nr);
      }
    }
    if (Argument* arg = dynamic_cast<Argument*>(ex)) {
      ex = arg->value();
    }
    if (String_Quoted* sq = dynamic_cast<String_Quoted*>(ex)) {
      if (was_itpl) {
        bool was_interpolant = ex->is_interpolant();
        ex = SASS_MEMORY_NEW(ctx.mem, String_Constant, sq->pstate(), sq->value());
        ex->is_interpolant(was_interpolant);
      }
    }

    if (dynamic_cast<Null*>(ex)) { return; }

    // parent selector needs another go
    if (dynamic_cast<Parent_Selector*>(ex)) {
      // XXX: this is never hit via spec tests
      ex = ex->perform(this);
    }

    if (List* l = dynamic_cast<List*>(ex)) {
      List* ll = SASS_MEMORY_NEW(ctx.mem, List, l->pstate(), 0, l->separator());
      // this fixes an issue with bourbon sample, not really sure why
      // if (l->size() && dynamic_cast<Null*>((*l)[0])) { res += ""; }
      for(auto item : *l) {
        item->is_interpolant(l->is_interpolant());
        std::string rl(""); interpolation(ctx, rl, item, into_quotes, l->is_interpolant());
        bool is_null = dynamic_cast<Null*>(item) != 0; // rl != ""
        if (!is_null) *ll << SASS_MEMORY_NEW(ctx.mem, String_Quoted, item->pstate(), rl);
      }
      res += (ll->to_string(ctx.c_options));
      ll->is_interpolant(l->is_interpolant());
    }

    // Value
    // Textual
    // Function_Call
    // Selector_List
    // String_Quoted
    // String_Constant
    // Parent_Selector
    // Binary_Expression
    else {
      // ex = ex->perform(this);
      if (into_quotes && ex->is_interpolant()) {
        res += evacuate_escapes(ex ? ex->to_string(ctx.c_options) : "");
      } else {
        res += ex ? ex->to_string(ctx.c_options) : "";
      }
    }

    if (needs_closing_brace) res += ")";

  }

  Expression* Eval::operator()(String_Schema* s)
  {
    size_t L = s->length();
    bool into_quotes = false;
    if (L > 1) {
      if (!dynamic_cast<String_Quoted*>((*s)[0]) && !dynamic_cast<String_Quoted*>((*s)[L - 1])) {
      if (String_Constant* l = dynamic_cast<String_Constant*>((*s)[0])) {
        if (String_Constant* r = dynamic_cast<String_Constant*>((*s)[L - 1])) {
          if (r->value().size() > 0) {
            if (l->value()[0] == '"' && r->value()[r->value().size() - 1] == '"') into_quotes = true;
            if (l->value()[0] == '\'' && r->value()[r->value().size() - 1] == '\'') into_quotes = true;
          }
        }
      }
      }
    }
    bool was_quoted = false;
    bool was_interpolant = false;
    std::string res("");
    for (size_t i = 0; i < L; ++i) {
      bool is_quoted = dynamic_cast<String_Quoted*>((*s)[i]) != NULL;
      if (was_quoted && !(*s)[i]->is_interpolant() && !was_interpolant) { res += " "; }
      else if (i > 0 && is_quoted && !(*s)[i]->is_interpolant() && !was_interpolant) { res += " "; }
      Expression* ex = (*s)[i]->is_delayed() ? (*s)[i] : (*s)[i]->perform(this);
      interpolation(ctx, res, ex, into_quotes, ex->is_interpolant());
      was_quoted = dynamic_cast<String_Quoted*>((*s)[i]) != NULL;
      was_interpolant = (*s)[i]->is_interpolant();

    }
    if (!s->is_interpolant()) {
      if (s->length() > 1 && res == "") return SASS_MEMORY_NEW(ctx.mem, Null, s->pstate());
      return SASS_MEMORY_NEW(ctx.mem, String_Constant, s->pstate(), res);
    }
    String_Quoted* str = SASS_MEMORY_NEW(ctx.mem, String_Quoted, s->pstate(), res);
    // if (s->is_interpolant()) str->quote_mark(0);
    // String_Constant* str = SASS_MEMORY_NEW(ctx.mem, String_Constant, s->pstate(), res);
    if (str->quote_mark()) str->quote_mark('*');
    else if (!is_in_comment) str->value(string_to_output(str->value()));
    str->is_interpolant(s->is_interpolant());
    return str;
  }


  Expression* Eval::operator()(String_Constant* s)
  {
    if (!s->is_delayed() && name_to_color(s->value())) {
      Color* c = SASS_MEMORY_NEW(ctx.mem, Color, *name_to_color(s->value()));
      c->pstate(s->pstate());
      c->disp(s->value());
      return c;
    }
    return s;
  }

  Expression* Eval::operator()(String_Quoted* s)
  {
    String_Quoted* str = SASS_MEMORY_NEW(ctx.mem, String_Quoted, s->pstate(), "");
    str->value(s->value());
    str->quote_mark(s->quote_mark());
    str->is_interpolant(s->is_interpolant());
    return str;
  }

  Expression* Eval::operator()(Supports_Operator* c)
  {
    Expression* left = c->left()->perform(this);
    Expression* right = c->right()->perform(this);
    Supports_Operator* cc = SASS_MEMORY_NEW(ctx.mem, Supports_Operator,
                                 c->pstate(),
                                 static_cast<Supports_Condition*>(left),
                                 static_cast<Supports_Condition*>(right),
                                 c->operand());
    return cc;
  }

  Expression* Eval::operator()(Supports_Negation* c)
  {
    Expression* condition = c->condition()->perform(this);
    Supports_Negation* cc = SASS_MEMORY_NEW(ctx.mem, Supports_Negation,
                                 c->pstate(),
                                 static_cast<Supports_Condition*>(condition));
    return cc;
  }

  Expression* Eval::operator()(Supports_Declaration* c)
  {
    Expression* feature = c->feature()->perform(this);
    Expression* value = c->value()->perform(this);
    Supports_Declaration* cc = SASS_MEMORY_NEW(ctx.mem, Supports_Declaration,
                              c->pstate(),
                              feature,
                              value);
    return cc;
  }

  Expression* Eval::operator()(Supports_Interpolation* c)
  {
    Expression* value = c->value()->perform(this);
    Supports_Interpolation* cc = SASS_MEMORY_NEW(ctx.mem, Supports_Interpolation,
                            c->pstate(),
                            value);
    return cc;
  }

  Expression* Eval::operator()(At_Root_Query* e)
  {
    Expression* feature = e->feature();
    feature = (feature ? feature->perform(this) : 0);
    Expression* value = e->value();
    value = (value ? value->perform(this) : 0);
    Expression* ee = SASS_MEMORY_NEW(ctx.mem, At_Root_Query,
                                     e->pstate(),
                                     static_cast<String*>(feature),
                                     value);
    return ee;
  }

  Expression* Eval::operator()(Media_Query* q)
  {
    String* t = q->media_type();
    t = static_cast<String*>(t ? t->perform(this) : 0);
    Media_Query* qq = SASS_MEMORY_NEW(ctx.mem, Media_Query,
                                      q->pstate(),
                                      t,
                                      q->length(),
                                      q->is_negated(),
                                      q->is_restricted());
    for (size_t i = 0, L = q->length(); i < L; ++i) {
      *qq << static_cast<Media_Query_Expression*>((*q)[i]->perform(this));
    }
    return qq;
  }

  Expression* Eval::operator()(Media_Query_Expression* e)
  {
    Expression* feature = e->feature();
    feature = (feature ? feature->perform(this) : 0);
    if (feature && dynamic_cast<String_Quoted*>(feature)) {
      feature = SASS_MEMORY_NEW(ctx.mem, String_Quoted,
                                  feature->pstate(),
                                  dynamic_cast<String_Quoted*>(feature)->value());
    }
    Expression* value = e->value();
    value = (value ? value->perform(this) : 0);
    if (value && dynamic_cast<String_Quoted*>(value)) {
      // XXX: this is never hit via spec tests
      value = SASS_MEMORY_NEW(ctx.mem, String_Quoted,
                                value->pstate(),
                                dynamic_cast<String_Quoted*>(value)->value());
    }
    return SASS_MEMORY_NEW(ctx.mem, Media_Query_Expression,
                           e->pstate(),
                           feature,
                           value,
                           e->is_interpolated());
  }

  Expression* Eval::operator()(Null* n)
  {
    return n;
  }

  Expression* Eval::operator()(Argument* a)
  {
    Expression* val = a->value();
    // delay missin function arguments?
    val->is_delayed(a->is_delayed());
    val = val->perform(this);
    val->is_delayed(false);

    bool is_rest_argument = a->is_rest_argument();
    bool is_keyword_argument = a->is_keyword_argument();

    if (a->is_rest_argument()) {
      if (val->concrete_type() == Expression::MAP) {
        is_rest_argument = false;
        is_keyword_argument = true;
      }
      else if(val->concrete_type() != Expression::LIST) {
        List* wrapper = SASS_MEMORY_NEW(ctx.mem, List,
                                        val->pstate(),
                                        0,
                                        SASS_COMMA,
                                        true);
        *wrapper << val;
        val = wrapper;
      }
    }
    return SASS_MEMORY_NEW(ctx.mem, Argument,
                           a->pstate(),
                           val,
                           a->name(),
                           is_rest_argument,
                           is_keyword_argument);
  }

  Expression* Eval::operator()(Arguments* a)
  {
    Arguments* aa = SASS_MEMORY_NEW(ctx.mem, Arguments, a->pstate());
    if (a->length() == 0) return aa;
    for (size_t i = 0, L = a->length(); i < L; ++i) {
      Argument* arg = static_cast<Argument*>((*a)[i]->perform(this));
      if (!(arg->is_rest_argument() || arg->is_keyword_argument())) {
        *aa << arg;
      }
    }

    if (a->has_rest_argument()) {
      Expression* splat = static_cast<Argument*>(
                            a->get_rest_argument()->perform(this)
                          )->value()->perform(this);

      Sass_Separator separator = SASS_COMMA;
      List* ls = dynamic_cast<List*>(splat);
      Map* ms = dynamic_cast<Map*>(splat);

      List* arglist = SASS_MEMORY_NEW(ctx.mem, List,
                                      splat->pstate(),
                                      0,
                                      ls ? ls->separator() : separator,
                                      true);

      if (ls && ls->is_arglist()) {
        for (auto as : *ls) *arglist << as;
      } else if (ms) {
        *aa << SASS_MEMORY_NEW(ctx.mem, Argument, splat->pstate(), ms, "", false, true);
      } else if (ls) {
        for (auto as : *ls) *arglist << as;
      } else {
        *arglist << splat;
      }
      if (arglist->length()) {
        *aa << SASS_MEMORY_NEW(ctx.mem, Argument, splat->pstate(), arglist, "", true);
      }
    }

    if (a->has_keyword_argument()) {
      Expression* kwarg = static_cast<Argument*>(
                            a->get_keyword_argument()->perform(this)
                          )->value()->perform(this);

      *aa << SASS_MEMORY_NEW(ctx.mem, Argument, kwarg->pstate(), kwarg, "", false, true);
    }

    return aa;
  }

  Expression* Eval::operator()(Comment* c)
  {
    return 0;
  }

  inline Expression* Eval::fallback_impl(AST_Node* n)
  {
    return static_cast<Expression*>(n);
  }

  // All the binary helpers.

  bool Eval::eq(Expression* lhs, Expression* rhs)
  {
    // use compare operator from ast node
    return lhs && rhs && *lhs == *rhs;
  }

  bool Eval::lt(Expression* lhs, Expression* rhs, std::string op)
  {
    Number* l = dynamic_cast<Number*>(lhs);
    Number* r = dynamic_cast<Number*>(rhs);
    // use compare operator from ast node
    if (!l || !r) throw Exception::UndefinedOperation(lhs, rhs, op);
    // use compare operator from ast node
    return *l < *r;
  }

  Value* Eval::op_numbers(Memory_Manager& mem, enum Sass_OP op, const Number& l, const Number& r, struct Sass_Inspect_Options opt, ParserState* pstate)
  {
    double lv = l.value();
    double rv = r.value();
    if (op == Sass_OP::DIV && rv == 0) {
      // XXX: this is never hit via spec tests
      return SASS_MEMORY_NEW(mem, String_Quoted, pstate ? *pstate : l.pstate(), lv ? "Infinity" : "NaN");
    }
    if (op == Sass_OP::MOD && !rv) {
      // XXX: this is never hit via spec tests
      throw Exception::ZeroDivisionError(l, r);
    }

    Number tmp(r);
    bool strict = op != Sass_OP::MUL && op != Sass_OP::DIV;
    tmp.normalize(l.find_convertible_unit(), strict);
    std::string l_unit(l.unit());
    std::string r_unit(tmp.unit());
    Number* v = SASS_MEMORY_NEW(mem, Number, l);
    v->pstate(pstate ? *pstate : l.pstate());
    if (l_unit.empty() && (op == Sass_OP::ADD || op == Sass_OP::SUB || op == Sass_OP::MOD)) {
      v->numerator_units() = r.numerator_units();
      v->denominator_units() = r.denominator_units();
    }

    if (op == Sass_OP::MUL) {
      v->value(ops[op](lv, rv));
      for (size_t i = 0, S = r.numerator_units().size(); i < S; ++i) {
        v->numerator_units().push_back(r.numerator_units()[i]);
      }
      for (size_t i = 0, S = r.denominator_units().size(); i < S; ++i) {
        v->denominator_units().push_back(r.denominator_units()[i]);
      }
    }
    else if (op == Sass_OP::DIV) {
      v->value(ops[op](lv, rv));
      for (size_t i = 0, S = r.numerator_units().size(); i < S; ++i) {
        v->denominator_units().push_back(r.numerator_units()[i]);
      }
      for (size_t i = 0, S = r.denominator_units().size(); i < S; ++i) {
        v->numerator_units().push_back(r.denominator_units()[i]);
      }
    } else {
      Number rh(r);
      v->value(ops[op](lv, rh.value() * r.convert_factor(l)));
      // v->normalize();
      return v;

      v->value(ops[op](lv, tmp.value()));
    }
    v->normalize();
    return v;
  }

  Value* Eval::op_number_color(Memory_Manager& mem, enum Sass_OP op, const Number& l, const Color& rh, struct Sass_Inspect_Options opt, ParserState* pstate)
  {
    Color r(rh);
    double lv = l.value();
    switch (op) {
      case Sass_OP::ADD:
      case Sass_OP::MUL: {
        return SASS_MEMORY_NEW(mem, Color,
                               pstate ? *pstate : l.pstate(),
                               ops[op](lv, r.r()),
                               ops[op](lv, r.g()),
                               ops[op](lv, r.b()),
                               r.a());
      } break;
      case Sass_OP::SUB:
      case Sass_OP::DIV: {
        std::string sep(op == Sass_OP::SUB ? "-" : "/");
        std::string color(r.to_string(opt));
        return SASS_MEMORY_NEW(mem, String_Quoted,
                               pstate ? *pstate : l.pstate(),
                               l.to_string(opt)
                               + sep
                               + color);
      } break;
      case Sass_OP::MOD: {
        throw Exception::UndefinedOperation(&l, &r, sass_op_to_name(op));
      } break;
      default: break; // caller should ensure that we don't get here
    }
    // unreachable
    return SASS_MEMORY_NEW(mem, Color, rh);
  }

  Value* Eval::op_color_number(Memory_Manager& mem, enum Sass_OP op, const Color& l, const Number& r, struct Sass_Inspect_Options opt, ParserState* pstate)
  {
    double rv = r.value();
    if (op == Sass_OP::DIV && !rv) {
      // comparison of Fixnum with Float failed?
      throw Exception::ZeroDivisionError(l, r);
    }
    return SASS_MEMORY_NEW(mem, Color,
                           pstate ? *pstate : l.pstate(),
                           ops[op](l.r(), rv),
                           ops[op](l.g(), rv),
                           ops[op](l.b(), rv),
                           l.a());
  }

  Value* Eval::op_colors(Memory_Manager& mem, enum Sass_OP op, const Color& l, const Color& r, struct Sass_Inspect_Options opt, ParserState* pstate)
  {
    if (l.a() != r.a()) {
      throw Exception::AlphaChannelsNotEqual(&l, &r, "+");
    }
    if (op == Sass_OP::DIV && (!r.r() || !r.g() ||!r.b())) {
      // comparison of Fixnum with Float failed?
      throw Exception::ZeroDivisionError(l, r);
    }
    return SASS_MEMORY_NEW(mem, Color,
                           pstate ? *pstate : l.pstate(),
                           ops[op](l.r(), r.r()),
                           ops[op](l.g(), r.g()),
                           ops[op](l.b(), r.b()),
                           l.a());
  }

  Value* Eval::op_strings(Memory_Manager& mem, Sass::Operand operand, Value& lhs, Value& rhs, struct Sass_Inspect_Options opt, ParserState* pstate, bool delayed)
  {
    Expression::Concrete_Type ltype = lhs.concrete_type();
    Expression::Concrete_Type rtype = rhs.concrete_type();
    enum Sass_OP op = operand.operand;

    String_Quoted* lqstr = dynamic_cast<String_Quoted*>(&lhs);
    String_Quoted* rqstr = dynamic_cast<String_Quoted*>(&rhs);

    std::string lstr(lqstr ? lqstr->value() : lhs.to_string(opt));
    std::string rstr(rqstr ? rqstr->value() : rhs.to_string(opt));

    if (ltype == Expression::NULL_VAL) throw Exception::InvalidNullOperation(&lhs, &rhs, sass_op_to_name(op));
    if (rtype == Expression::NULL_VAL) throw Exception::InvalidNullOperation(&lhs, &rhs, sass_op_to_name(op));
    if (op == Sass_OP::MOD) throw Exception::UndefinedOperation(&lhs, &rhs, sass_op_to_name(op));
    if (op == Sass_OP::MUL) throw Exception::UndefinedOperation(&lhs, &rhs, sass_op_to_name(op));
    std::string sep;
    switch (op) {
      case Sass_OP::SUB: sep = "-"; break;
      case Sass_OP::DIV: sep = "/"; break;
      case Sass_OP::MUL: sep = "*"; break;
      case Sass_OP::MOD: sep = "%"; break;
      case Sass_OP::EQ:  sep = "=="; break;
      case Sass_OP::NEQ:  sep = "!="; break;
      case Sass_OP::LT:  sep = "<"; break;
      case Sass_OP::GT:  sep = ">"; break;
      case Sass_OP::LTE:  sep = "<="; break;
      case Sass_OP::GTE:  sep = ">="; break;
      default:                      break;
    }

    if ( (sep == "") /* &&
         (sep != "/" || !rqstr || !rqstr->quote_mark()) */
    ) {
      char quote_mark = 0;
      std::string unq(unquote(lstr + sep + rstr, &quote_mark, true));
      if (quote_mark && quote_mark != '*') {
        return SASS_MEMORY_NEW(mem, String_Constant, lhs.pstate(), quote_mark + unq + quote_mark);
      }
      return SASS_MEMORY_NEW(mem, String_Quoted, lhs.pstate(), lstr + sep + rstr);
    }

    if (sep != "" && !delayed) {
      if (operand.ws_before) sep = " " + sep;
      if (operand.ws_after) sep = sep + " ";
    }

    if (op == Sass_OP::SUB || op == Sass_OP::DIV) {
      if (lqstr && lqstr->quote_mark()) lstr = quote(lstr);
      if (rqstr && rqstr->quote_mark()) rstr = quote(rstr);
      return SASS_MEMORY_NEW(mem, String_Constant, lhs.pstate(), lstr + sep + rstr);
    }

    return SASS_MEMORY_NEW(mem, String_Constant, lhs.pstate(), (lstr) + sep + (rstr));
  }

  Expression* cval_to_astnode(Memory_Manager& mem, union Sass_Value* v, Context& ctx, Backtrace* backtrace, ParserState pstate)
  {
    using std::strlen;
    using std::strcpy;
    Expression* e = 0;
    switch (sass_value_get_tag(v)) {
      case SASS_BOOLEAN: {
        e = SASS_MEMORY_NEW(mem, Boolean, pstate, !!sass_boolean_get_value(v));
      } break;
      case SASS_NUMBER: {
        e = SASS_MEMORY_NEW(mem, Number, pstate, sass_number_get_value(v), sass_number_get_unit(v));
      } break;
      case SASS_COLOR: {
        e = SASS_MEMORY_NEW(mem, Color, pstate, sass_color_get_r(v), sass_color_get_g(v), sass_color_get_b(v), sass_color_get_a(v));
      } break;
      case SASS_STRING: {
        if (sass_string_is_quoted(v))
          e = SASS_MEMORY_NEW(mem, String_Quoted, pstate, sass_string_get_value(v));
        else {
          e = SASS_MEMORY_NEW(mem, String_Constant, pstate, sass_string_get_value(v));
        }
      } break;
      case SASS_LIST: {
        List* l = SASS_MEMORY_NEW(mem, List, pstate, sass_list_get_length(v), sass_list_get_separator(v));
        for (size_t i = 0, L = sass_list_get_length(v); i < L; ++i) {
          *l << cval_to_astnode(mem, sass_list_get_value(v, i), ctx, backtrace, pstate);
        }
        e = l;
      } break;
      case SASS_MAP: {
        Map* m = SASS_MEMORY_NEW(mem, Map, pstate);
        for (size_t i = 0, L = sass_map_get_length(v); i < L; ++i) {
          *m << std::make_pair(
            cval_to_astnode(mem, sass_map_get_key(v, i), ctx, backtrace, pstate),
            cval_to_astnode(mem, sass_map_get_value(v, i), ctx, backtrace, pstate));
        }
        e = m;
      } break;
      case SASS_NULL: {
        e = SASS_MEMORY_NEW(mem, Null, pstate);
      } break;
      case SASS_ERROR: {
        error("Error in C function: " + std::string(sass_error_get_message(v)), pstate, backtrace);
      } break;
      case SASS_WARNING: {
        error("Warning in C function: " + std::string(sass_warning_get_message(v)), pstate, backtrace);
      } break;
    }
    return e;
  }

  Selector_List* Eval::operator()(Selector_List* s)
  {
    std::vector<Selector_List*> rv;
    Selector_List* sl = SASS_MEMORY_NEW(ctx.mem, Selector_List, s->pstate());
    sl->is_optional(s->is_optional());
    sl->media_block(s->media_block());
    sl->is_optional(s->is_optional());
    for (size_t i = 0, iL = s->length(); i < iL; ++i) {
      rv.push_back(operator()((*s)[i]));
    }

    // we should actually permutate parent first
    // but here we have permutated the selector first
    size_t round = 0;
    while (round != std::string::npos) {
      bool abort = true;
      for (size_t i = 0, iL = rv.size(); i < iL; ++i) {
        if (rv[i]->length() > round) {
          *sl << (*rv[i])[round];
          abort = false;
        }
      }
      if (abort) {
        round = std::string::npos;
      } else {
        ++ round;
      }

    }
    return sl;
  }


  Selector_List* Eval::operator()(Complex_Selector* s)
  {
    return s->parentize(selector(), ctx);

  }

  // XXX: this is never hit via spec tests
  Attribute_Selector* Eval::operator()(Attribute_Selector* s)
  {
    String* attr = s->value();
    if (attr) { attr = static_cast<String*>(attr->perform(this)); }
    Attribute_Selector* ss = SASS_MEMORY_NEW(ctx.mem, Attribute_Selector, *s);
    ss->value(attr);
    return ss;
  }

  Selector_List* Eval::operator()(Selector_Schema* s)
  {
    // the parser will look for a brace to end the selector
    std::string result_str(s->contents()->perform(this)->to_string(ctx.c_options));
    result_str = unquote(Util::rtrim(result_str)) + "\n{";
    Parser p = Parser::from_c_str(result_str.c_str(), ctx, s->pstate());
    p.last_media_block = s->media_block();
    Selector_List* sl = p.parse_selector_list(exp.block_stack.back()->is_root());
    if (s->has_parent_ref()) sl->remove_parent_selectors();
    return operator()(sl);
  }

  Expression* Eval::operator()(Parent_Selector* p)
  {
    Selector_List* pr = selector();
    if (pr) {
      exp.selector_stack.pop_back();
      pr = operator()(pr);
      exp.selector_stack.push_back(pr);
      return pr;
    } else {
      return SASS_MEMORY_NEW(ctx.mem, Null, p->pstate());
    }
  }

}
