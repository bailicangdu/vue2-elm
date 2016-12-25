#ifndef SASS_OPERATION_H
#define SASS_OPERATION_H

#include "ast_fwd_decl.hpp"

namespace Sass {

  template<typename T>
  class Operation {
  public:
    virtual T operator()(AST_Node* x)               = 0;
    virtual ~Operation()                            { }
    // statements
    virtual T operator()(Block* x)                  = 0;
    virtual T operator()(Ruleset* x)                = 0;
    virtual T operator()(Propset* x)                = 0;
    virtual T operator()(Bubble* x)                 = 0;
    virtual T operator()(Supports_Block* x)         = 0;
    virtual T operator()(Media_Block* x)            = 0;
    virtual T operator()(At_Root_Block* x)          = 0;
    virtual T operator()(Directive* x)                = 0;
    virtual T operator()(Keyframe_Rule* x)          = 0;
    virtual T operator()(Declaration* x)            = 0;
    virtual T operator()(Assignment* x)             = 0;
    virtual T operator()(Import* x)                 = 0;
    virtual T operator()(Import_Stub* x)            = 0;
    virtual T operator()(Warning* x)                = 0;
    virtual T operator()(Error* x)                  = 0;
    virtual T operator()(Debug* x)                  = 0;
    virtual T operator()(Comment* x)                = 0;
    virtual T operator()(If* x)                     = 0;
    virtual T operator()(For* x)                    = 0;
    virtual T operator()(Each* x)                   = 0;
    virtual T operator()(While* x)                  = 0;
    virtual T operator()(Return* x)                 = 0;
    virtual T operator()(Content* x)                = 0;
    virtual T operator()(Extension* x)              = 0;
    virtual T operator()(Definition* x)             = 0;
    virtual T operator()(Mixin_Call* x)             = 0;
    // expressions
    virtual T operator()(List* x)                   = 0;
    virtual T operator()(Map* x)                    = 0;
    virtual T operator()(Binary_Expression* x)      = 0;
    virtual T operator()(Unary_Expression* x)       = 0;
    virtual T operator()(Function_Call* x)          = 0;
    virtual T operator()(Function_Call_Schema* x)   = 0;
    virtual T operator()(Custom_Warning* x)         = 0;
    virtual T operator()(Custom_Error* x)           = 0;
    virtual T operator()(Variable* x)               = 0;
    virtual T operator()(Textual* x)                = 0;
    virtual T operator()(Number* x)                 = 0;
    virtual T operator()(Color* x)                  = 0;
    virtual T operator()(Boolean* x)                = 0;
    virtual T operator()(String_Schema* x)          = 0;
    virtual T operator()(String_Quoted* x)          = 0;
    virtual T operator()(String_Constant* x)        = 0;
    virtual T operator()(Supports_Condition* x)     = 0;
    virtual T operator()(Supports_Operator* x)      = 0;
    virtual T operator()(Supports_Negation* x)      = 0;
    virtual T operator()(Supports_Declaration* x)   = 0;
    virtual T operator()(Supports_Interpolation* x) = 0;
    virtual T operator()(Media_Query* x)            = 0;
    virtual T operator()(Media_Query_Expression* x) = 0;
    virtual T operator()(At_Root_Query* x)     = 0;
    virtual T operator()(Null* x)                   = 0;
    virtual T operator()(Parent_Selector* x)        = 0;
    // parameters and arguments
    virtual T operator()(Parameter* x)              = 0;
    virtual T operator()(Parameters* x)             = 0;
    virtual T operator()(Argument* x)               = 0;
    virtual T operator()(Arguments* x)              = 0;
    // selectors
    virtual T operator()(Selector_Schema* x)        = 0;
    virtual T operator()(Selector_Placeholder* x)   = 0;
    virtual T operator()(Type_Selector* x)          = 0;
    virtual T operator()(Selector_Qualifier* x)     = 0;
    virtual T operator()(Attribute_Selector* x)     = 0;
    virtual T operator()(Pseudo_Selector* x)        = 0;
    virtual T operator()(Wrapped_Selector* x)       = 0;
    virtual T operator()(Compound_Selector* x)      = 0;
    virtual T operator()(Complex_Selector* x)       = 0;
    virtual T operator()(Selector_List* x)          = 0;

    template <typename U>
    T fallback(U x) { return T(); }
  };

  template <typename T, typename D>
  class Operation_CRTP : public Operation<T> {
  public:
    D& impl() { return static_cast<D&>(*this); }
  public:
    T operator()(AST_Node* x)               { return static_cast<D*>(this)->fallback(x); }
    // statements
    T operator()(Block* x)                  { return static_cast<D*>(this)->fallback(x); }
    T operator()(Ruleset* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(Propset* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(Bubble* x)                 { return static_cast<D*>(this)->fallback(x); }
    T operator()(Supports_Block* x)         { return static_cast<D*>(this)->fallback(x); }
    T operator()(Media_Block* x)            { return static_cast<D*>(this)->fallback(x); }
    T operator()(At_Root_Block* x)          { return static_cast<D*>(this)->fallback(x); }
    T operator()(Directive* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(Keyframe_Rule* x)          { return static_cast<D*>(this)->fallback(x); }
    T operator()(Declaration* x)            { return static_cast<D*>(this)->fallback(x); }
    T operator()(Assignment* x)             { return static_cast<D*>(this)->fallback(x); }
    T operator()(Import* x)                 { return static_cast<D*>(this)->fallback(x); }
    T operator()(Import_Stub* x)            { return static_cast<D*>(this)->fallback(x); }
    T operator()(Warning* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(Error* x)                  { return static_cast<D*>(this)->fallback(x); }
    T operator()(Debug* x)                  { return static_cast<D*>(this)->fallback(x); }
    T operator()(Comment* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(If* x)                     { return static_cast<D*>(this)->fallback(x); }
    T operator()(For* x)                    { return static_cast<D*>(this)->fallback(x); }
    T operator()(Each* x)                   { return static_cast<D*>(this)->fallback(x); }
    T operator()(While* x)                  { return static_cast<D*>(this)->fallback(x); }
    T operator()(Return* x)                 { return static_cast<D*>(this)->fallback(x); }
    T operator()(Content* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(Extension* x)              { return static_cast<D*>(this)->fallback(x); }
    T operator()(Definition* x)             { return static_cast<D*>(this)->fallback(x); }
    T operator()(Mixin_Call* x)             { return static_cast<D*>(this)->fallback(x); }
    // expressions
    T operator()(List* x)                   { return static_cast<D*>(this)->fallback(x); }
    T operator()(Map* x)                    { return static_cast<D*>(this)->fallback(x); }
    T operator()(Binary_Expression* x)      { return static_cast<D*>(this)->fallback(x); }
    T operator()(Unary_Expression* x)       { return static_cast<D*>(this)->fallback(x); }
    T operator()(Function_Call* x)          { return static_cast<D*>(this)->fallback(x); }
    T operator()(Function_Call_Schema* x)   { return static_cast<D*>(this)->fallback(x); }
    T operator()(Custom_Warning* x)         { return static_cast<D*>(this)->fallback(x); }
    T operator()(Custom_Error* x)           { return static_cast<D*>(this)->fallback(x); }
    T operator()(Variable* x)               { return static_cast<D*>(this)->fallback(x); }
    T operator()(Textual* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(Number* x)                 { return static_cast<D*>(this)->fallback(x); }
    T operator()(Color* x)                  { return static_cast<D*>(this)->fallback(x); }
    T operator()(Boolean* x)                { return static_cast<D*>(this)->fallback(x); }
    T operator()(String_Schema* x)          { return static_cast<D*>(this)->fallback(x); }
    T operator()(String_Constant* x)        { return static_cast<D*>(this)->fallback(x); }
    T operator()(String_Quoted* x)          { return static_cast<D*>(this)->fallback(x); }
    T operator()(Supports_Condition* x)     { return static_cast<D*>(this)->fallback(x); }
    T operator()(Supports_Operator* x)      { return static_cast<D*>(this)->fallback(x); }
    T operator()(Supports_Negation* x)      { return static_cast<D*>(this)->fallback(x); }
    T operator()(Supports_Declaration* x)   { return static_cast<D*>(this)->fallback(x); }
    T operator()(Supports_Interpolation* x) { return static_cast<D*>(this)->fallback(x); }
    T operator()(Media_Query* x)            { return static_cast<D*>(this)->fallback(x); }
    T operator()(Media_Query_Expression* x) { return static_cast<D*>(this)->fallback(x); }
    T operator()(At_Root_Query* x)     { return static_cast<D*>(this)->fallback(x); }
    T operator()(Null* x)                   { return static_cast<D*>(this)->fallback(x); }
    T operator()(Parent_Selector* x)        { return static_cast<D*>(this)->fallback(x); }
    // parameters and arguments
    T operator()(Parameter* x)              { return static_cast<D*>(this)->fallback(x); }
    T operator()(Parameters* x)             { return static_cast<D*>(this)->fallback(x); }
    T operator()(Argument* x)               { return static_cast<D*>(this)->fallback(x); }
    T operator()(Arguments* x)              { return static_cast<D*>(this)->fallback(x); }
    // selectors
    T operator()(Selector_Schema* x)        { return static_cast<D*>(this)->fallback(x); }
    T operator()(Selector_Placeholder* x)   { return static_cast<D*>(this)->fallback(x); }
    T operator()(Type_Selector* x)          { return static_cast<D*>(this)->fallback(x); }
    T operator()(Selector_Qualifier* x)     { return static_cast<D*>(this)->fallback(x); }
    T operator()(Attribute_Selector* x)     { return static_cast<D*>(this)->fallback(x); }
    T operator()(Pseudo_Selector* x)        { return static_cast<D*>(this)->fallback(x); }
    T operator()(Wrapped_Selector* x)       { return static_cast<D*>(this)->fallback(x); }
    T operator()(Compound_Selector* x)      { return static_cast<D*>(this)->fallback(x); }
    T operator()(Complex_Selector* x)       { return static_cast<D*>(this)->fallback(x); }
    T operator()(Selector_List* x)          { return static_cast<D*>(this)->fallback(x); }

    template <typename U>
    T fallback(U x)                                 { return T(); }
  };

}

#endif
