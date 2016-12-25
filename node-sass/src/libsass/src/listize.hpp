#ifndef SASS_LISTIZE_H
#define SASS_LISTIZE_H

#include <vector>
#include <iostream>

#include "ast.hpp"
#include "context.hpp"
#include "operation.hpp"
#include "environment.hpp"

namespace Sass {

  typedef Environment<AST_Node*> Env;
  struct Backtrace;

  class Listize : public Operation_CRTP<Expression*, Listize> {

    Memory_Manager& mem;

    Expression* fallback_impl(AST_Node* n);

  public:
    Listize(Memory_Manager&);
    ~Listize() { }

    Expression* operator()(Selector_List*);
    Expression* operator()(Complex_Selector*);
    Expression* operator()(Compound_Selector*);

    template <typename U>
    Expression* fallback(U x) { return fallback_impl(x); }
  };

}

#endif
