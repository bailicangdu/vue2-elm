#ifndef SASS_TO_VALUE_H
#define SASS_TO_VALUE_H

#include "operation.hpp"
#include "sass/values.h"
#include "ast_fwd_decl.hpp"

namespace Sass {

  class To_Value : public Operation_CRTP<Value*, To_Value> {

    Value* fallback_impl(AST_Node* n);

  private:

    Context& ctx;
    Memory_Manager& mem;

  public:

    To_Value(Context& ctx, Memory_Manager& mem)
    : ctx(ctx), mem(mem)
    { }
    ~To_Value() { }
    using Operation<Value*>::operator();

    Value* operator()(Argument*);
    Value* operator()(Boolean*);
    Value* operator()(Number*);
    Value* operator()(Color*);
    Value* operator()(String_Constant*);
    Value* operator()(String_Quoted*);
    Value* operator()(Custom_Warning*);
    Value* operator()(Custom_Error*);
    Value* operator()(List*);
    Value* operator()(Map*);
    Value* operator()(Null*);

    // convert to string via `To_String`
    Value* operator()(Selector_List*);
    Value* operator()(Binary_Expression*);

    // fallback throws error
    template <typename U>
    Value* fallback(U x) { return fallback_impl(x); }
  };

}

#endif
