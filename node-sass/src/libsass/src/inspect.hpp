#ifndef SASS_INSPECT_H
#define SASS_INSPECT_H

#include "position.hpp"
#include "operation.hpp"
#include "emitter.hpp"

namespace Sass {
  class Context;

  class Inspect : public Operation_CRTP<void, Inspect>, public Emitter {
  protected:
    // import all the class-specific methods and override as desired
    using Operation_CRTP<void, Inspect>::operator();

    void fallback_impl(AST_Node* n);

  public:

    Inspect(Emitter emi);
    virtual ~Inspect();

    // statements
    virtual void operator()(Block*);
    virtual void operator()(Ruleset*);
    virtual void operator()(Propset*);
    virtual void operator()(Bubble*);
    virtual void operator()(Supports_Block*);
    virtual void operator()(Media_Block*);
    virtual void operator()(At_Root_Block*);
    virtual void operator()(Directive*);
    virtual void operator()(Keyframe_Rule*);
    virtual void operator()(Declaration*);
    virtual void operator()(Assignment*);
    virtual void operator()(Import*);
    virtual void operator()(Import_Stub*);
    virtual void operator()(Warning*);
    virtual void operator()(Error*);
    virtual void operator()(Debug*);
    virtual void operator()(Comment*);
    virtual void operator()(If*);
    virtual void operator()(For*);
    virtual void operator()(Each*);
    virtual void operator()(While*);
    virtual void operator()(Return*);
    virtual void operator()(Extension*);
    virtual void operator()(Definition*);
    virtual void operator()(Mixin_Call*);
    virtual void operator()(Content*);
    // expressions
    virtual void operator()(Map*);
    virtual void operator()(List*);
    virtual void operator()(Binary_Expression*);
    virtual void operator()(Unary_Expression*);
    virtual void operator()(Function_Call*);
    virtual void operator()(Function_Call_Schema*);
    // virtual void operator()(Custom_Warning*);
    // virtual void operator()(Custom_Error*);
    virtual void operator()(Variable*);
    virtual void operator()(Textual*);
    virtual void operator()(Number*);
    virtual void operator()(Color*);
    virtual void operator()(Boolean*);
    virtual void operator()(String_Schema*);
    virtual void operator()(String_Constant*);
    virtual void operator()(String_Quoted*);
    virtual void operator()(Custom_Error*);
    virtual void operator()(Custom_Warning*);
    virtual void operator()(Supports_Operator*);
    virtual void operator()(Supports_Negation*);
    virtual void operator()(Supports_Declaration*);
    virtual void operator()(Supports_Interpolation*);
    virtual void operator()(Media_Query*);
    virtual void operator()(Media_Query_Expression*);
    virtual void operator()(At_Root_Query*);
    virtual void operator()(Null*);
    virtual void operator()(Parent_Selector* p);
    // parameters and arguments
    virtual void operator()(Parameter*);
    virtual void operator()(Parameters*);
    virtual void operator()(Argument*);
    virtual void operator()(Arguments*);
    // selectors
    virtual void operator()(Selector_Schema*);
    virtual void operator()(Selector_Placeholder*);
    virtual void operator()(Type_Selector*);
    virtual void operator()(Selector_Qualifier*);
    virtual void operator()(Attribute_Selector*);
    virtual void operator()(Pseudo_Selector*);
    virtual void operator()(Wrapped_Selector*);
    virtual void operator()(Compound_Selector*);
    virtual void operator()(Complex_Selector*);
    virtual void operator()(Selector_List*);

    // template <typename U>
    // void fallback(U x) { fallback_impl(reinterpret_cast<AST_Node*>(x)); }
  };

}
#endif
