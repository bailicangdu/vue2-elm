#include "sass.hpp"
#include <iostream>
#include <typeinfo>
#include <vector>

#include "cssize.hpp"
#include "context.hpp"
#include "backtrace.hpp"

namespace Sass {

  Cssize::Cssize(Context& ctx, Backtrace* bt)
  : ctx(ctx),
    block_stack(std::vector<Block*>()),
    p_stack(std::vector<Statement*>()),
    s_stack(std::vector<Selector_List*>()),
    backtrace(bt)
  {
    s_stack.push_back(NULL);
  }

  Statement* Cssize::parent()
  {
    return p_stack.size() ? p_stack.back() : block_stack.front();
  }

  Selector_List* Cssize::selector()
  {
    return s_stack.size() ? s_stack.back() : NULL;
  }

  Statement* Cssize::operator()(Block* b)
  {
    Block* bb = SASS_MEMORY_NEW(ctx.mem, Block, b->pstate(), b->length(), b->is_root());
    // bb->tabs(b->tabs());
    block_stack.push_back(bb);
    append_block(b);
    block_stack.pop_back();
    return bb;
  }

  Statement* Cssize::operator()(Directive* r)
  {
    if (!r->block() || !r->block()->length()) return r;

    if (parent()->statement_type() == Statement::RULESET)
    {
      return (r->is_keyframes()) ? SASS_MEMORY_NEW(ctx.mem, Bubble, r->pstate(), r) : bubble(r);
    }

    p_stack.push_back(r);
    Directive* rr = SASS_MEMORY_NEW(ctx.mem, Directive,
                                  r->pstate(),
                                  r->keyword(),
                                  r->selector(),
                                  r->block() ? r->block()->perform(this)->block() : 0);
    if (r->value()) rr->value(r->value());
    p_stack.pop_back();

    bool directive_exists = false;
    size_t L = rr->block() ? rr->block()->length() : 0;
    for (size_t i = 0; i < L && !directive_exists; ++i) {
      Statement* s = (*r->block())[i];
      if (s->statement_type() != Statement::BUBBLE) directive_exists = true;
      else {
        s = static_cast<Bubble*>(s)->node();
        if (s->statement_type() != Statement::DIRECTIVE) directive_exists = false;
        else directive_exists = (static_cast<Directive*>(s)->keyword() == rr->keyword());
      }

    }

    Block* result = SASS_MEMORY_NEW(ctx.mem, Block, rr->pstate());
    if (!(directive_exists || rr->is_keyframes()))
    {
      Directive* empty_node = static_cast<Directive*>(rr);
      empty_node->block(SASS_MEMORY_NEW(ctx.mem, Block, rr->block() ? rr->block()->pstate() : rr->pstate()));
      *result << empty_node;
    }

    Statement* ss = debubble(rr->block() ? rr->block() : SASS_MEMORY_NEW(ctx.mem, Block, rr->pstate()), rr);
    for (size_t i = 0, L = ss->block()->length(); i < L; ++i) {
      *result << (*ss->block())[i];
    }

    return result;
  }

  Statement* Cssize::operator()(Keyframe_Rule* r)
  {
    if (!r->block() || !r->block()->length()) return r;

    Keyframe_Rule* rr = SASS_MEMORY_NEW(ctx.mem, Keyframe_Rule,
                                        r->pstate(),
                                        r->block()->perform(this)->block());
    if (r->selector()) rr->selector(r->selector());

    return debubble(rr->block(), rr)->block();
  }

  Statement* Cssize::operator()(Ruleset* r)
  {
    p_stack.push_back(r);
    s_stack.push_back(dynamic_cast<Selector_List*>(r->selector()));
    Ruleset* rr = SASS_MEMORY_NEW(ctx.mem, Ruleset,
                                  r->pstate(),
                                  r->selector(),
                                  r->block()->perform(this)->block());
    rr->is_root(r->is_root());
    // rr->tabs(r->block()->tabs());
    s_stack.pop_back();
    p_stack.pop_back();

    if (!rr->block()) {
      error("Illegal nesting: Only properties may be nested beneath properties.", r->block()->pstate());
    }

    Block* props = SASS_MEMORY_NEW(ctx.mem, Block, rr->block()->pstate());
    Block* rules = SASS_MEMORY_NEW(ctx.mem, Block, rr->block()->pstate());
    for (size_t i = 0, L = rr->block()->length(); i < L; i++)
    {
      Statement* s = (*rr->block())[i];
      if (bubblable(s)) *rules << s;
      if (!bubblable(s)) *props << s;
    }

    if (props->length())
    {
      Block* bb = SASS_MEMORY_NEW(ctx.mem, Block, rr->block()->pstate());
      *bb += props;
      rr->block(bb);

      for (size_t i = 0, L = rules->length(); i < L; i++)
      {
        (*rules)[i]->tabs((*rules)[i]->tabs() + 1);
      }

      rules->unshift(rr);
    }

    rules = debubble(rules)->block();

    if (!(!rules->length() ||
          !bubblable(rules->last()) ||
          parent()->statement_type() == Statement::RULESET))
    {
      rules->last()->group_end(true);
    }

    return rules;
  }

  Statement* Cssize::operator()(Null* m)
  {
    return 0;
  }

  Statement* Cssize::operator()(Media_Block* m)
  {
    if (parent()->statement_type() == Statement::RULESET)
    { return bubble(m); }

    if (parent()->statement_type() == Statement::MEDIA)
    { return SASS_MEMORY_NEW(ctx.mem, Bubble, m->pstate(), m); }

    p_stack.push_back(m);

    Media_Block* mm = SASS_MEMORY_NEW(ctx.mem, Media_Block,
                                      m->pstate(),
                                      m->media_queries(),
                                      m->block()->perform(this)->block());
    mm->tabs(m->tabs());

    p_stack.pop_back();

    return debubble(mm->block(), mm)->block();
  }

  Statement* Cssize::operator()(Supports_Block* m)
  {
    if (!m->block()->length())
    { return m; }

    if (parent()->statement_type() == Statement::RULESET)
    { return bubble(m); }

    p_stack.push_back(m);

    Supports_Block* mm = SASS_MEMORY_NEW(ctx.mem, Supports_Block,
                                       m->pstate(),
                                       m->condition(),
                                       m->block()->perform(this)->block());
    mm->tabs(m->tabs());

    p_stack.pop_back();

    return debubble(mm->block(), mm)->block();
  }

  Statement* Cssize::operator()(At_Root_Block* m)
  {
    bool tmp = false;
    for (size_t i = 0, L = p_stack.size(); i < L; ++i) {
      Statement* s = p_stack[i];
      tmp |= m->exclude_node(s);
    }

    if (!tmp)
    {
      Block* bb = m->block()->perform(this)->block();
      for (size_t i = 0, L = bb->length(); i < L; ++i) {
        // (bb->elements())[i]->tabs(m->tabs());
        if (bubblable((*bb)[i])) (*bb)[i]->tabs((*bb)[i]->tabs() + m->tabs());
      }
      if (bb->length() && bubblable(bb->last())) bb->last()->group_end(m->group_end());
      return bb;
    }

    if (m->exclude_node(parent()))
    {
      return SASS_MEMORY_NEW(ctx.mem, Bubble, m->pstate(), m);
    }

    return bubble(m);
  }

  Statement* Cssize::bubble(Directive* m)
  {
    Block* bb = SASS_MEMORY_NEW(ctx.mem, Block, this->parent()->pstate());
    Has_Block* new_rule = static_cast<Has_Block*>(shallow_copy(this->parent()));
    new_rule->block(bb);
    new_rule->tabs(this->parent()->tabs());

    size_t L = m->block() ? m->block()->length() : 0;
    for (size_t i = 0; i < L; ++i) {
      *new_rule->block() << (*m->block())[i];
    }

    Block* wrapper_block = SASS_MEMORY_NEW(ctx.mem, Block, m->block() ? m->block()->pstate() : m->pstate());
    *wrapper_block << new_rule;
    Directive* mm = SASS_MEMORY_NEW(ctx.mem, Directive,
                                  m->pstate(),
                                  m->keyword(),
                                  m->selector(),
                                  wrapper_block);
    if (m->value()) mm->value(m->value());

    Bubble* bubble = SASS_MEMORY_NEW(ctx.mem, Bubble, mm->pstate(), mm);
    return bubble;
  }

  Statement* Cssize::bubble(At_Root_Block* m)
  {
    Block* bb = SASS_MEMORY_NEW(ctx.mem, Block, this->parent()->pstate());
    Has_Block* new_rule = static_cast<Has_Block*>(shallow_copy(this->parent()));
    new_rule->block(bb);
    new_rule->tabs(this->parent()->tabs());

    for (size_t i = 0, L = m->block()->length(); i < L; ++i) {
      *new_rule->block() << (*m->block())[i];
    }

    Block* wrapper_block = SASS_MEMORY_NEW(ctx.mem, Block, m->block()->pstate());
    *wrapper_block << new_rule;
    At_Root_Block* mm = SASS_MEMORY_NEW(ctx.mem, At_Root_Block,
                                        m->pstate(),
                                        wrapper_block,
                                        m->expression());
    Bubble* bubble = SASS_MEMORY_NEW(ctx.mem, Bubble, mm->pstate(), mm);
    return bubble;
  }

  Statement* Cssize::bubble(Supports_Block* m)
  {
    Ruleset* parent = static_cast<Ruleset*>(shallow_copy(this->parent()));

    Block* bb = SASS_MEMORY_NEW(ctx.mem, Block, parent->block()->pstate());
    Ruleset* new_rule = SASS_MEMORY_NEW(ctx.mem, Ruleset,
                                        parent->pstate(),
                                        parent->selector(),
                                        bb);
    new_rule->tabs(parent->tabs());

    for (size_t i = 0, L = m->block()->length(); i < L; ++i) {
      *new_rule->block() << (*m->block())[i];
    }

    Block* wrapper_block = SASS_MEMORY_NEW(ctx.mem, Block, m->block()->pstate());
    *wrapper_block << new_rule;
    Supports_Block* mm = SASS_MEMORY_NEW(ctx.mem, Supports_Block,
                                       m->pstate(),
                                       m->condition(),
                                       wrapper_block);

    mm->tabs(m->tabs());

    Bubble* bubble = SASS_MEMORY_NEW(ctx.mem, Bubble, mm->pstate(), mm);
    return bubble;
  }

  Statement* Cssize::bubble(Media_Block* m)
  {
    Ruleset* parent = static_cast<Ruleset*>(shallow_copy(this->parent()));

    Block* bb = SASS_MEMORY_NEW(ctx.mem, Block, parent->block()->pstate());
    Ruleset* new_rule = SASS_MEMORY_NEW(ctx.mem, Ruleset,
                                        parent->pstate(),
                                        parent->selector(),
                                        bb);
    new_rule->tabs(parent->tabs());

    for (size_t i = 0, L = m->block()->length(); i < L; ++i) {
      *new_rule->block() << (*m->block())[i];
    }

    Block* wrapper_block = SASS_MEMORY_NEW(ctx.mem, Block, m->block()->pstate());
    *wrapper_block << new_rule;
    Media_Block* mm = SASS_MEMORY_NEW(ctx.mem, Media_Block,
                                      m->pstate(),
                                      m->media_queries(),
                                      wrapper_block,
                                      0);

    mm->tabs(m->tabs());

    Bubble* bubble = SASS_MEMORY_NEW(ctx.mem, Bubble, mm->pstate(), mm);

    return bubble;
  }

  bool Cssize::bubblable(Statement* s)
  {
    return s->statement_type() == Statement::RULESET || s->bubbles();
  }

  Statement* Cssize::flatten(Statement* s)
  {
    Block* bb = s->block();
    Block* result = SASS_MEMORY_NEW(ctx.mem, Block, bb->pstate(), 0, bb->is_root());
    for (size_t i = 0, L = bb->length(); i < L; ++i) {
      Statement* ss = (*bb)[i];
      if (ss->block()) {
        ss = flatten(ss);
        for (size_t j = 0, K = ss->block()->length(); j < K; ++j) {
          *result << (*ss->block())[j];
        }
      }
      else {
        *result << ss;
      }
    }
    return result;
  }

  std::vector<std::pair<bool, Block*>> Cssize::slice_by_bubble(Statement* b)
  {
    std::vector<std::pair<bool, Block*>> results;
    for (size_t i = 0, L = b->block()->length(); i < L; ++i) {
      Statement* value = (*b->block())[i];
      bool key = value->statement_type() == Statement::BUBBLE;

      if (!results.empty() && results.back().first == key)
      {
        Block* wrapper_block = results.back().second;
        *wrapper_block << value;
      }
      else
      {
        Block* wrapper_block = SASS_MEMORY_NEW(ctx.mem, Block, value->pstate());
        *wrapper_block << value;
        results.push_back(std::make_pair(key, wrapper_block));
      }
    }
    return results;
  }

  Statement* Cssize::shallow_copy(Statement* s)
  {
    switch (s->statement_type())
    {
      case Statement::RULESET:
        return SASS_MEMORY_NEW(ctx.mem, Ruleset, *static_cast<Ruleset*>(s));
      case Statement::MEDIA:
        return SASS_MEMORY_NEW(ctx.mem, Media_Block, *static_cast<Media_Block*>(s));
      case Statement::BUBBLE:
        return SASS_MEMORY_NEW(ctx.mem, Bubble, *static_cast<Bubble*>(s));
      case Statement::DIRECTIVE:
        return SASS_MEMORY_NEW(ctx.mem, Directive, *static_cast<Directive*>(s));
      case Statement::SUPPORTS:
        return SASS_MEMORY_NEW(ctx.mem, Supports_Block, *static_cast<Supports_Block*>(s));
      case Statement::ATROOT:
        return SASS_MEMORY_NEW(ctx.mem, At_Root_Block, *static_cast<At_Root_Block*>(s));
      case Statement::KEYFRAMERULE:
        return SASS_MEMORY_NEW(ctx.mem, Keyframe_Rule, *static_cast<Keyframe_Rule*>(s));
      case Statement::NONE:
      default:
        error("unknown internal error; please contact the LibSass maintainers", s->pstate(), backtrace);
        String_Quoted* msg = SASS_MEMORY_NEW(ctx.mem, String_Quoted, ParserState("[WARN]"), std::string("`CSSize` can't clone ") + typeid(*s).name());
        return SASS_MEMORY_NEW(ctx.mem, Warning, ParserState("[WARN]"), msg);
    }
  }

  Statement* Cssize::debubble(Block* children, Statement* parent)
  {
    Has_Block* previous_parent = 0;
    std::vector<std::pair<bool, Block*>> baz = slice_by_bubble(children);
    Block* result = SASS_MEMORY_NEW(ctx.mem, Block, children->pstate());

    for (size_t i = 0, L = baz.size(); i < L; ++i) {
      bool is_bubble = baz[i].first;
      Block* slice = baz[i].second;

      if (!is_bubble) {
        if (!parent) {
          *result << slice;
        }
        else if (previous_parent) {
          *previous_parent->block() += slice;
        }
        else {
          previous_parent = static_cast<Has_Block*>(shallow_copy(parent));
          previous_parent->tabs(parent->tabs());

          Has_Block* new_parent = static_cast<Has_Block*>(shallow_copy(parent));
          new_parent->block(slice);
          new_parent->tabs(parent->tabs());

          *result << new_parent;
        }
        continue;
      }

      Block* wrapper_block = SASS_MEMORY_NEW(ctx.mem, Block,
                                             children->block()->pstate(),
                                             children->block()->length(),
                                             children->block()->is_root());

      for (size_t j = 0, K = slice->length(); j < K; ++j)
      {
        Statement* ss = 0;
        Bubble* b = static_cast<Bubble*>((*slice)[j]);

        if (!parent ||
            parent->statement_type() != Statement::MEDIA ||
            b->node()->statement_type() != Statement::MEDIA ||
            static_cast<Media_Block*>(b->node())->media_queries() == static_cast<Media_Block*>(parent)->media_queries())
        {
          ss = b->node();
        }
        else
        {
          List* mq = merge_media_queries(static_cast<Media_Block*>(b->node()), static_cast<Media_Block*>(parent));
          if (!mq->length()) continue;
          static_cast<Media_Block*>(b->node())->media_queries(mq);
          ss = b->node();
        }

        if (!ss) continue;

        ss->tabs(ss->tabs() + b->tabs());
        ss->group_end(b->group_end());

        if (!ss) continue;

        Block* bb = SASS_MEMORY_NEW(ctx.mem, Block,
                                    children->block()->pstate(),
                                    children->block()->length(),
                                    children->block()->is_root());
        *bb << ss->perform(this);
        Statement* wrapper = flatten(bb);
        *wrapper_block << wrapper;

        if (wrapper->block()->length()) {
          previous_parent = 0;
        }
      }

      if (wrapper_block) {
        *result << flatten(wrapper_block);
      }
    }

    return flatten(result);
  }

  Statement* Cssize::fallback_impl(AST_Node* n)
  {
    return static_cast<Statement*>(n);
  }

  void Cssize::append_block(Block* b)
  {
    Block* current_block = block_stack.back();

    for (size_t i = 0, L = b->length(); i < L; ++i) {
      Statement* ith = (*b)[i]->perform(this);
      if (ith && ith->block()) {
        for (size_t j = 0, K = ith->block()->length(); j < K; ++j) {
          *current_block << (*ith->block())[j];
        }
      }
      else if (ith) {
        *current_block << ith;
      }
    }
  }

  List* Cssize::merge_media_queries(Media_Block* m1, Media_Block* m2)
  {
    List* qq = SASS_MEMORY_NEW(ctx.mem, List,
                               m1->media_queries()->pstate(),
                               m1->media_queries()->length(),
                               SASS_COMMA);

    for (size_t i = 0, L = m1->media_queries()->length(); i < L; i++) {
      for (size_t j = 0, K = m2->media_queries()->length(); j < K; j++) {
        Media_Query* mq1 = static_cast<Media_Query*>((*m1->media_queries())[i]);
        Media_Query* mq2 = static_cast<Media_Query*>((*m2->media_queries())[j]);
        Media_Query* mq = merge_media_query(mq1, mq2);

        if (mq) *qq << mq;
      }
    }

    return qq;
  }


  Media_Query* Cssize::merge_media_query(Media_Query* mq1, Media_Query* mq2)
  {

    std::string type;
    std::string mod;

    std::string m1 = std::string(mq1->is_restricted() ? "only" : mq1->is_negated() ? "not" : "");
    std::string t1 = mq1->media_type() ? mq1->media_type()->to_string(ctx.c_options) : "";
    std::string m2 = std::string(mq2->is_restricted() ? "only" : mq1->is_negated() ? "not" : "");
    std::string t2 = mq2->media_type() ? mq2->media_type()->to_string(ctx.c_options) : "";


    if (t1.empty()) t1 = t2;
    if (t2.empty()) t2 = t1;

    if ((m1 == "not") ^ (m2 == "not")) {
      if (t1 == t2) {
        return 0;
      }
      type = m1 == "not" ? t2 : t1;
      mod = m1 == "not" ? m2 : m1;
    }
    else if (m1 == "not" && m2 == "not") {
      if (t1 != t2) {
        return 0;
      }
      type = t1;
      mod = "not";
    }
    else if (t1 != t2) {
      return 0;
    } else {
      type = t1;
      mod = m1.empty() ? m2 : m1;
    }

    Media_Query* mm = SASS_MEMORY_NEW(ctx.mem, Media_Query,

mq1->pstate(), 0,
mq1->length() + mq2->length(), mod == "not", mod == "only"
);

    if (!type.empty()) {
      mm->media_type(SASS_MEMORY_NEW(ctx.mem, String_Quoted, mq1->pstate(), type));
    }

    *mm += mq2;
    *mm += mq1;
    return mm;
  }
}
