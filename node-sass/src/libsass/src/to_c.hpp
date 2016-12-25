#ifndef SASS_TO_C_H
#define SASS_TO_C_H

#include "ast_fwd_decl.hpp"
#include "operation.hpp"
#include "sass/values.h"

namespace Sass {

  class To_C : public Operation_CRTP<union Sass_Value*, To_C> {
    // override this to define a catch-all
    union Sass_Value* fallback_impl(AST_Node* n);

  public:

    To_C() { }
    ~To_C() { }

    union Sass_Value* operator()(Boolean*);
    union Sass_Value* operator()(Number*);
    union Sass_Value* operator()(Color*);
    union Sass_Value* operator()(String_Constant*);
    union Sass_Value* operator()(String_Quoted*);
    union Sass_Value* operator()(Custom_Warning*);
    union Sass_Value* operator()(Custom_Error*);
    union Sass_Value* operator()(List*);
    union Sass_Value* operator()(Map*);
    union Sass_Value* operator()(Null*);
    union Sass_Value* operator()(Arguments*);
    union Sass_Value* operator()(Argument*);

    // dispatch to fallback implementation
    union Sass_Value* fallback(AST_Node* x)
    { return fallback_impl(x); }
  };

}

#endif
