#include "sass.hpp"
#include <iostream>
#include <typeinfo>

#include "expand.hpp"
#include "bind.hpp"
#include "eval.hpp"
#include "backtrace.hpp"
#include "context.hpp"
#include "parser.hpp"

namespace Sass {

  Expand::Expand(Context& ctx, Env* env, Backtrace* bt)
  : ctx(ctx),
    eval(Eval(*this)),
    env_stack(std::vector<Env*>()),
    block_stack(std::vector<Block*>()),
    call_stack(std::vector<AST_Node*>()),
    property_stack(std::vector<String*>()),
    selector_stack(std::vector<Selector_List*>()),
    media_block_stack(std::vector<Media_Block*>()),
    backtrace_stack(std::vector<Backtrace*>()),
    in_keyframes(false)
  {
    env_stack.push_back(0);
    env_stack.push_back(env);
    block_stack.push_back(0);
    call_stack.push_back(0);
    // import_stack.push_back(0);
    property_stack.push_back(0);
    selector_stack.push_back(0);
    media_block_stack.push_back(0);
    backtrace_stack.push_back(0);
    backtrace_stack.push_back(bt);
  }

  Context& Expand::context()
  {
    return ctx;
  }

  Env* Expand::environment()
  {
    if (env_stack.size() > 0)
      return env_stack.back();
    return 0;
  }

  Selector_List* Expand::selector()
  {
    if (selector_stack.size() > 0)
      return selector_stack.back();
    return 0;
  }

  Backtrace* Expand::backtrace()
  {
    if (backtrace_stack.size() > 0)
      return backtrace_stack.back();
    return 0;
  }

  // blocks create new variable scopes
  Statement* Expand::operator()(Block* b)
  {
    // create new local environment
    // set the current env as parent
    Env env(environment());
    // copy the block object (add items later)
    Block* bb = SASS_MEMORY_NEW(ctx.mem, Block,
                                b->pstate(),
                                b->length(),
                                b->is_root());
    // setup block and env stack
    this->block_stack.push_back(bb);
    this->env_stack.push_back(&env);
    // operate on block
    this->append_block(b);
    // revert block and env stack
    this->block_stack.pop_back();
    this->env_stack.pop_back();
    // return copy
    return bb;
  }

  Statement* Expand::operator()(Ruleset* r)
  {
    // reset when leaving scope

    if (in_keyframes) {
      Keyframe_Rule* k = SASS_MEMORY_NEW(ctx.mem, Keyframe_Rule, r->pstate(), r->block()->perform(this)->block());
      if (r->selector()) {
        selector_stack.push_back(0);
        k->selector(static_cast<Selector_List*>(r->selector()->perform(&eval)));
        selector_stack.pop_back();
      }
      return k;
    }

    // do some special checks for the base level rules
    if (r->is_root()) {
      if (Selector_List* selector_list = dynamic_cast<Selector_List*>(r->selector())) {
        for (Complex_Selector* complex_selector : selector_list->elements()) {
          Complex_Selector* tail = complex_selector;
          while (tail) {
            if (tail->head()) for (Simple_Selector* header : tail->head()->elements()) {
              if (dynamic_cast<Parent_Selector*>(header) == NULL) continue; // skip all others
              std::string sel_str(complex_selector->to_string(ctx.c_options));
              error("Base-level rules cannot contain the parent-selector-referencing character '&'.", header->pstate(), backtrace());
            }
            tail = tail->tail();
          }
        }
      }
    }

    Expression* ex = r->selector()->perform(&eval);
    Selector_List* sel = dynamic_cast<Selector_List*>(ex);
    if (sel == 0) throw std::runtime_error("Expanded null selector");

    if (sel->length() == 0 || sel->has_parent_ref()) {
      bool has_parent_selector = false;
      for (size_t i = 0, L = selector_stack.size(); i < L && !has_parent_selector; i++) {
        Selector_List* ll = selector_stack.at(i);
        has_parent_selector = ll != 0 && ll->length() > 0;
      }
      if (!has_parent_selector) {
        error("Base-level rules cannot contain the parent-selector-referencing character '&'.", sel->pstate(), backtrace());
      }
    }

    selector_stack.push_back(sel);
    Env* env = 0;
    if (block_stack.back()->is_root()) {
      env = new Env(environment());
      env_stack.push_back(env);
    }
    sel->set_media_block(media_block_stack.back());
    Block* blk = r->block()->perform(this)->block();
    Ruleset* rr = SASS_MEMORY_NEW(ctx.mem, Ruleset,
                                  r->pstate(),
                                  sel,
                                  blk);
    selector_stack.pop_back();
    if (block_stack.back()->is_root()) {
      env_stack.pop_back();
      delete env;
    }
    rr->is_root(r->is_root());
    rr->tabs(r->tabs());

    return rr;
  }

  Statement* Expand::operator()(Propset* p)
  {
    property_stack.push_back(p->property_fragment());
    Block* expanded_block = p->block()->perform(this)->block();

    for (size_t i = 0, L = expanded_block->length(); i < L; ++i) {
      Statement* stm = (*expanded_block)[i];
      if (Declaration* dec = static_cast<Declaration*>(stm)) {
        String_Schema* combined_prop = SASS_MEMORY_NEW(ctx.mem, String_Schema, p->pstate());
        if (!property_stack.empty()) {
          *combined_prop << property_stack.back()->perform(&eval)
                         << SASS_MEMORY_NEW(ctx.mem, String_Quoted,
             p->pstate(), "-")
             << dec->property(); // TODO: eval the prop into a string constant
        }
        else {
          *combined_prop << dec->property();
        }
        dec->property(combined_prop);
        *block_stack.back() << dec;
      }
      else if (typeid(*stm) == typeid(Comment)) {
        // drop comments in propsets
      }
      else {
        error("contents of namespaced properties must result in style declarations only", stm->pstate(), backtrace());
      }
    }

    property_stack.pop_back();

    return 0;
  }

  Statement* Expand::operator()(Supports_Block* f)
  {
    Expression* condition = f->condition()->perform(&eval);
    Supports_Block* ff = SASS_MEMORY_NEW(ctx.mem, Supports_Block,
                                       f->pstate(),
                                       static_cast<Supports_Condition*>(condition),
                                       f->block()->perform(this)->block());
    return ff;
  }

  Statement* Expand::operator()(Media_Block* m)
  {
    media_block_stack.push_back(m);
    Expression* mq = m->media_queries()->perform(&eval);
    std::string str_mq(mq->to_string(ctx.c_options));
    char* str = sass_copy_c_string(str_mq.c_str());
    ctx.strings.push_back(str);
    Parser p(Parser::from_c_str(str, ctx, mq->pstate()));
    mq = p.parse_media_queries();
    Media_Block* mm = SASS_MEMORY_NEW(ctx.mem, Media_Block,
                                      m->pstate(),
                                      static_cast<List*>(mq->perform(&eval)),
                                      m->block()->perform(this)->block(),
                                      0);
    media_block_stack.pop_back();
    mm->tabs(m->tabs());
    return mm;
  }

  Statement* Expand::operator()(At_Root_Block* a)
  {
    Block* ab = a->block();
    // if (ab) ab->is_root(true);
    Expression* ae = a->expression();
    if (ae) ae = ae->perform(&eval);
    else ae = SASS_MEMORY_NEW(ctx.mem, At_Root_Query, a->pstate());
    Block* bb = ab ? ab->perform(this)->block() : 0;
    At_Root_Block* aa = SASS_MEMORY_NEW(ctx.mem, At_Root_Block,
                                        a->pstate(),
                                        bb,
                                        static_cast<At_Root_Query*>(ae));
    // aa->block()->is_root(true);
    return aa;
  }

  Statement* Expand::operator()(Directive* a)
  {
    LOCAL_FLAG(in_keyframes, a->is_keyframes());
    Block* ab = a->block();
    Selector* as = a->selector();
    Expression* av = a->value();
    selector_stack.push_back(0);
    if (av) av = av->perform(&eval);
    if (as) as = dynamic_cast<Selector*>(as->perform(&eval));
    selector_stack.pop_back();
    Block* bb = ab ? ab->perform(this)->block() : 0;
    Directive* aa = SASS_MEMORY_NEW(ctx.mem, Directive,
                                  a->pstate(),
                                  a->keyword(),
                                  as,
                                  bb,
                                  av);
    return aa;
  }

  Statement* Expand::operator()(Declaration* d)
  {
    String* old_p = d->property();
    String* new_p = static_cast<String*>(old_p->perform(&eval));
    Expression* value = d->value()->perform(&eval);
    if (!value || (value->is_invisible() && !d->is_important())) return 0;
    Declaration* decl = SASS_MEMORY_NEW(ctx.mem, Declaration,
                                        d->pstate(),
                                        new_p,
                                        value,
                                        d->is_important());
    decl->tabs(d->tabs());
    return decl;
  }

  Statement* Expand::operator()(Assignment* a)
  {
    Env* env = environment();
    std::string var(a->variable());
    if (a->is_global()) {
      if (a->is_default()) {
        if (env->has_global(var)) {
          Expression* e = dynamic_cast<Expression*>(env->get_global(var));
          if (!e || e->concrete_type() == Expression::NULL_VAL) {
            env->set_global(var, a->value()->perform(&eval));
          }
        }
        else {
          env->set_global(var, a->value()->perform(&eval));
        }
      }
      else {
        env->set_global(var, a->value()->perform(&eval));
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
                cur->set_local(var, a->value()->perform(&eval));
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
            env->set_global(var, a->value()->perform(&eval));
          }
        }
      }
      else if (env->is_lexical()) {
        env->set_local(var, a->value()->perform(&eval));
      }
      else {
        env->set_local(var, a->value()->perform(&eval));
      }
    }
    else {
      env->set_lexical(var, a->value()->perform(&eval));
    }
    return 0;
  }

  Statement* Expand::operator()(Import* imp)
  {
    Import* result = SASS_MEMORY_NEW(ctx.mem, Import, imp->pstate());
    if (imp->media_queries() && imp->media_queries()->size()) {
      Expression* ex = imp->media_queries()->perform(&eval);
      result->media_queries(dynamic_cast<List*>(ex));
    }
    for ( size_t i = 0, S = imp->urls().size(); i < S; ++i) {
      result->urls().push_back(imp->urls()[i]->perform(&eval));
    }
    // all resources have been dropped for Input_Stubs
    // for ( size_t i = 0, S = imp->incs().size(); i < S; ++i) {}
    return result;
  }

  Statement* Expand::operator()(Import_Stub* i)
  {
    // get parent node from call stack
    AST_Node* parent = call_stack.back();
    if (parent && dynamic_cast<Block*>(parent) == NULL) {
      error("Import directives may not be used within control directives or mixins.", i->pstate());
    }
    // we don't seem to need that actually afterall
    Sass_Import_Entry import = sass_make_import(
      i->imp_path().c_str(),
      i->abs_path().c_str(),
      0, 0
    );
    ctx.import_stack.push_back(import);
    const std::string& abs_path(i->resource().abs_path);
    append_block(ctx.sheets.at(abs_path).root);
    sass_delete_import(ctx.import_stack.back());
    ctx.import_stack.pop_back();
    return 0;
  }

  Statement* Expand::operator()(Warning* w)
  {
    // eval handles this too, because warnings may occur in functions
    w->perform(&eval);
    return 0;
  }

  Statement* Expand::operator()(Error* e)
  {
    // eval handles this too, because errors may occur in functions
    e->perform(&eval);
    return 0;
  }

  Statement* Expand::operator()(Debug* d)
  {
    // eval handles this too, because warnings may occur in functions
    d->perform(&eval);
    return 0;
  }

  Statement* Expand::operator()(Comment* c)
  {
    eval.is_in_comment = true;
    auto rv = SASS_MEMORY_NEW(ctx.mem, Comment, c->pstate(), static_cast<String*>(c->text()->perform(&eval)), c->is_important());
    eval.is_in_comment = false;
    // TODO: eval the text, once we're parsing/storing it as a String_Schema
    return rv;
  }

  Statement* Expand::operator()(If* i)
  {
    Env env(environment(), true);
    env_stack.push_back(&env);
    call_stack.push_back(i);
    if (*i->predicate()->perform(&eval)) {
      append_block(i->block());
    }
    else {
      Block* alt = i->alternative();
      if (alt) append_block(alt);
    }
    call_stack.pop_back();
    env_stack.pop_back();
    return 0;
  }

  // For does not create a new env scope
  // But iteration vars are reset afterwards
  Statement* Expand::operator()(For* f)
  {
    std::string variable(f->variable());
    Expression* low = f->lower_bound()->perform(&eval);
    if (low->concrete_type() != Expression::NUMBER) {
      throw Exception::TypeMismatch(*low, "integer");
    }
    Expression* high = f->upper_bound()->perform(&eval);
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
    env_stack.push_back(&env);
    call_stack.push_back(f);
    Number* it = SASS_MEMORY_NEW(env.mem, Number, low->pstate(), start, sass_end->unit());
    env.set_local(variable, it);
    Block* body = f->block();
    if (start < end) {
      if (f->is_inclusive()) ++end;
      for (double i = start;
           i < end;
           ++i) {
        it->value(i);
        env.set_local(variable, it);
        append_block(body);
      }
    } else {
      if (f->is_inclusive()) --end;
      for (double i = start;
           i > end;
           --i) {
        it->value(i);
        env.set_local(variable, it);
        append_block(body);
      }
    }
    call_stack.pop_back();
    env_stack.pop_back();
    return 0;
  }

  // Eval does not create a new env scope
  // But iteration vars are reset afterwards
  Statement* Expand::operator()(Each* e)
  {
    std::vector<std::string> variables(e->variables());
    Expression* expr = e->list()->perform(&eval);
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
    // remember variables and then reset them
    Env env(environment(), true);
    env_stack.push_back(&env);
    call_stack.push_back(e);
    Block* body = e->block();

    if (map) {
      for (auto key : map->keys()) {
        Expression* k = key->perform(&eval);
        Expression* v = map->at(key)->perform(&eval);

        if (variables.size() == 1) {
          List* variable = SASS_MEMORY_NEW(ctx.mem, List, map->pstate(), 2, SASS_SPACE);
          *variable << k;
          *variable << v;
          env.set_local(variables[0], variable);
        } else {
          env.set_local(variables[0], k);
          env.set_local(variables[1], v);
        }
        append_block(body);
      }
    }
    else {
      // bool arglist = list->is_arglist();
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
            // if (arglist) var = (*scalars)[0];
            env.set_local(variables[0], var);
          } else {
            for (size_t j = 0, K = variables.size(); j < K; ++j) {
              Expression* res = j >= scalars->length()
                ? SASS_MEMORY_NEW(ctx.mem, Null, expr->pstate())
                : (*scalars)[j]->perform(&eval);
              env.set_local(variables[j], res);
            }
          }
        } else {
          if (variables.size() > 0) {
            env.set_local(variables[0], e);
            for (size_t j = 1, K = variables.size(); j < K; ++j) {
              Expression* res = SASS_MEMORY_NEW(ctx.mem, Null, expr->pstate());
              env.set_local(variables[j], res);
            }
          }
        }
        append_block(body);
      }
    }
    call_stack.pop_back();
    env_stack.pop_back();
    return 0;
  }

  Statement* Expand::operator()(While* w)
  {
    Expression* pred = w->predicate();
    Block* body = w->block();
    Env env(environment(), true);
    env_stack.push_back(&env);
    call_stack.push_back(w);
    while (*pred->perform(&eval)) {
      append_block(body);
    }
    call_stack.pop_back();
    env_stack.pop_back();
    return 0;
  }

  Statement* Expand::operator()(Return* r)
  {
    error("@return may only be used within a function", r->pstate(), backtrace());
    return 0;
  }


  void Expand::expand_selector_list(Selector* s, Selector_List* extender) {

    if (Selector_List* sl = dynamic_cast<Selector_List*>(s)) {
      for (Complex_Selector* complex_selector : sl->elements()) {
        Complex_Selector* tail = complex_selector;
        while (tail) {
          if (tail->head()) for (Simple_Selector* header : tail->head()->elements()) {
            if (dynamic_cast<Parent_Selector*>(header) == NULL) continue; // skip all others
            std::string sel_str(complex_selector->to_string(ctx.c_options));
            error("Can't extend " + sel_str + ": can't extend parent selectors", header->pstate(), backtrace());
          }
          tail = tail->tail();
        }
      }
    }


    Selector_List* contextualized = dynamic_cast<Selector_List*>(s->perform(&eval));
    if (contextualized == NULL) return;
    for (auto complex_sel : contextualized->elements()) {
      Complex_Selector* c = complex_sel;
      if (!c->head() || c->tail()) {
        std::string sel_str(contextualized->to_string(ctx.c_options));
        error("Can't extend " + sel_str + ": can't extend nested selectors", c->pstate(), backtrace());
      }
      Compound_Selector* placeholder = c->head();
      if (contextualized->is_optional()) placeholder->is_optional(true);
      for (size_t i = 0, L = extender->length(); i < L; ++i) {
        Complex_Selector* sel = (*extender)[i];
        if (!(sel->head() && sel->head()->length() > 0 &&
            dynamic_cast<Parent_Selector*>((*sel->head())[0])))
        {
          Compound_Selector* hh = SASS_MEMORY_NEW(ctx.mem, Compound_Selector, (*extender)[i]->pstate());
          hh->media_block((*extender)[i]->media_block());
          Complex_Selector* ssel = SASS_MEMORY_NEW(ctx.mem, Complex_Selector, (*extender)[i]->pstate());
          ssel->media_block((*extender)[i]->media_block());
          if (sel->has_line_feed()) ssel->has_line_feed(true);
          Parent_Selector* ps = SASS_MEMORY_NEW(ctx.mem, Parent_Selector, (*extender)[i]->pstate());
          ps->media_block((*extender)[i]->media_block());
          *hh << ps;
          ssel->tail(sel);
          ssel->head(hh);
          sel = ssel;
        }
        // if (c->has_line_feed()) sel->has_line_feed(true);
        ctx.subset_map.put(placeholder->to_str_vec(), std::make_pair(sel, placeholder));
      }
    }

  }

  Statement* Expand::operator()(Extension* e)
  {
    if (Selector_List* extender = dynamic_cast<Selector_List*>(selector())) {
      Selector* s = e->selector();
      if (Selector_Schema* schema = dynamic_cast<Selector_Schema*>(s)) {
        if (schema->has_parent_ref()) s = eval(schema);
      }
      if (Selector_List* sl = dynamic_cast<Selector_List*>(s)) {
        for (Complex_Selector* cs : *sl) {
          if (cs != NULL && cs->head() != NULL) {
            cs->head()->media_block(media_block_stack.back());
          }
        }
      }
      selector_stack.push_back(0);
      expand_selector_list(s, extender);
      selector_stack.pop_back();
    }
    return 0;
  }

  Statement* Expand::operator()(Definition* d)
  {
    Env* env = environment();
    Definition* dd = SASS_MEMORY_NEW(ctx.mem, Definition, *d);
    env->local_frame()[d->name() +
                        (d->type() == Definition::MIXIN ? "[m]" : "[f]")] = dd;

    if (d->type() == Definition::FUNCTION && (
      Prelexer::calc_fn_call(d->name().c_str()) ||
      d->name() == "element"    ||
      d->name() == "expression" ||
      d->name() == "url"
    )) {
      deprecated(
        "Naming a function \"" + d->name() + "\" is disallowed",
        "This name conflicts with an existing CSS function with special parse rules.",
         d->pstate()
      );
    }


    // set the static link so we can have lexical scoping
    dd->environment(env);
    return 0;
  }

  Statement* Expand::operator()(Mixin_Call* c)
  {
    Env* env = environment();
    std::string full_name(c->name() + "[m]");
    if (!env->has(full_name)) {
      error("no mixin named " + c->name(), c->pstate(), backtrace());
    }
    Definition* def = static_cast<Definition*>((*env)[full_name]);
    Block* body = def->block();
    Parameters* params = def->parameters();

    if (c->block() && c->name() != "@content" && !body->has_content()) {
      error("Mixin \"" + c->name() + "\" does not accept a content block.", c->pstate(), backtrace());
    }
    Arguments* args = static_cast<Arguments*>(c->arguments()
                                               ->perform(&eval));
    Backtrace new_bt(backtrace(), c->pstate(), ", in mixin `" + c->name() + "`");
    backtrace_stack.push_back(&new_bt);
    Env new_env(def->environment());
    env_stack.push_back(&new_env);
    if (c->block()) {
      // represent mixin content blocks as thunks/closures
      Definition* thunk = SASS_MEMORY_NEW(ctx.mem, Definition,
                                          c->pstate(),
                                          "@content",
                                          SASS_MEMORY_NEW(ctx.mem, Parameters, c->pstate()),
                                          c->block(),
                                          Definition::MIXIN);
      thunk->environment(env);
      new_env.local_frame()["@content[m]"] = thunk;
    }

    bind(std::string("Mixin"), c->name(), params, args, &ctx, &new_env, &eval);
    append_block(body);
    backtrace_stack.pop_back();
    env_stack.pop_back();
    return 0;
  }

  Statement* Expand::operator()(Content* c)
  {
    Env* env = environment();
    // convert @content directives into mixin calls to the underlying thunk
    if (!env->has("@content[m]")) return 0;
    if (block_stack.back()->is_root()) {
      selector_stack.push_back(0);
    }
    Mixin_Call* call = SASS_MEMORY_NEW(ctx.mem, Mixin_Call,
                                       c->pstate(),
                                       "@content",
                                       SASS_MEMORY_NEW(ctx.mem, Arguments, c->pstate()));
    Statement* stm = call->perform(this);
    if (block_stack.back()->is_root()) {
      selector_stack.pop_back();
    }
    return stm;
  }

  // produce an error if something is not implemented
  inline Statement* Expand::fallback_impl(AST_Node* n)
  {
    std::string err =std:: string("`Expand` doesn't handle ") + typeid(*n).name();
    String_Quoted* msg = SASS_MEMORY_NEW(ctx.mem, String_Quoted, ParserState("[WARN]"), err);
    error("unknown internal error; please contact the LibSass maintainers", n->pstate(), backtrace());
    return SASS_MEMORY_NEW(ctx.mem, Warning, ParserState("[WARN]"), msg);
  }

  // process and add to last block on stack
  inline void Expand::append_block(Block* b)
  {
    if (b->is_root()) call_stack.push_back(b);
    for (size_t i = 0, L = b->length(); i < L; ++i) {
      Statement* ith = (*b)[i]->perform(this);
      if (ith) *block_stack.back() << ith;
    }
    if (b->is_root()) call_stack.pop_back();
  }

}
