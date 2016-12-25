#include "sass.hpp"
#include "ast.hpp"
#include "to_value.hpp"

namespace Sass {

  Value* To_Value::fallback_impl(AST_Node* n)
  {
    // throw a runtime error if this happens
    // we want a well defined set of possible nodes
    throw std::runtime_error("invalid node for to_value");
    // mute warning
    return 0;
  }

  // Custom_Error is a valid value
  Value* To_Value::operator()(Custom_Error* e)
  {
    return e;
  }

  // Custom_Warning is a valid value
  Value* To_Value::operator()(Custom_Warning* w)
  {
    return w;
  }

  // Boolean is a valid value
  Value* To_Value::operator()(Boolean* b)
  {
    return b;
  }

  // Number is a valid value
  Value* To_Value::operator()(Number* n)
  {
    return n;
  }

  // Color is a valid value
  Value* To_Value::operator()(Color* c)
  {
    return c;
  }

  // String_Constant is a valid value
  Value* To_Value::operator()(String_Constant* s)
  {
    return s;
  }

  // String_Quoted is a valid value
  Value* To_Value::operator()(String_Quoted* s)
  {
    return s;
  }

  // List is a valid value
  Value* To_Value::operator()(List* l)
  {
    List* ll = SASS_MEMORY_NEW(mem, List,
                               l->pstate(),
                               l->length(),
                               l->separator(),
                               l->is_arglist());
    for (size_t i = 0, L = l->length(); i < L; ++i) {
      *ll << (*l)[i]->perform(this);
    }
    return ll;
  }

  // Map is a valid value
  Value* To_Value::operator()(Map* m)
  {
    return m;
  }

  // Null is a valid value
  Value* To_Value::operator()(Null* n)
  {
    return n;
  }

  // Argument returns its value
  Value* To_Value::operator()(Argument* arg)
  {
    if (!arg->name().empty()) return 0;
    return arg->value()->perform(this);
  }

  // Selector_List is converted to a string
  Value* To_Value::operator()(Selector_List* s)
  {
    return SASS_MEMORY_NEW(mem, String_Quoted,
                           s->pstate(),
                           s->to_string(ctx.c_options));
  }

  // Binary_Expression is converted to a string
  Value* To_Value::operator()(Binary_Expression* s)
  {
    return SASS_MEMORY_NEW(mem, String_Quoted,
                           s->pstate(),
                           s->to_string(ctx.c_options));
  }

};
