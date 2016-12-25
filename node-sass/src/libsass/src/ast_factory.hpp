#ifndef SASS_AST_FACTORY_H
#define SASS_AST_FACTORY_H

#include <vector>

#include "ast.hpp"

namespace Sass {

  class AST_Factory {
    std::vector<AST_Node*> nodes;
  public:
    // statements
    Block* new_Block(std::string p, size_t l, size_t s = 0, bool r = false);
    Ruleset* new_Ruleset(std::string p, size_t l, Selector* s, Block* b);
    Propset* new_Propset(std::string p, size_t l, String* pf, Block* b);
    Supports_Query* new_Supports_Query(std::string p, size_t l, Supports_Query* f, Block* b);
    Media_Query* new_Media_Query(std::string p, size_t l, List* q, Block* b);
    At_Root_Block* new_At_Root_Block(std::string p, size_t l, Selector* sel, Block* b);
    Directive* new_At_Rule(std::string p, size_t l, std::string kwd, Selector* sel, Block* b);
    Keyframe_Rule* new_Keyframe_Rule(std::string p, size_t l, Block* b);
    Declaration* new_Declaration(std::string p, size_t l, String* prop, List* vals);
    Assignment* new_Assignment(std::string p, size_t l, std::string var, Expression* val, bool guarded = false);
    Import<Function_Call*>* new_CSS_Import(std::string p, size_t l, Function_Call* loc);
    Import<String*>* new_SASS_Import(std::string p, size_t l, String* loc);
    Custom_Warning* new_Custom_Warning(std::string msg, size_t l, std::string msg);
    Custom_Error* new_Custom_Error(std::string p, size_t l, std::string msg);
    Warning* new_Warning(std::string p, size_t l, Expression* msg);
    Error* new_Error(std::string p, size_t l, Expression* msg);
    Debug* new_Debug(std::string p, size_t l, Expression* val);
    Comment* new_Comment(std::string p, size_t l, String* txt);
    If* new_If(std::string p, size_t l, Expression* pred, Block* con, Block* alt = 0);
    For* new_For(std::string p, size_t l, std::string var, Expression* lo, Expression* hi, Block* b, bool inc);
    Each* new_Each(std::string p, size_t l, std::vector<std::string> vars, Expression* lst, Block* b);
    While* new_While(std::string p, size_t l, Expression* pred, Block* b);
    Extension* new_Extension(std::string p, size_t l, Selector* s);
    Definition<MIXIN>* new_Mixin_Definition(std::string p, size_t l, std::string n, Parameters* params, Block* b);
    Definition<FUNCTION>* new_Function_Definition(std::string p, size_t l, std::string n, Parameters* params, Block* b);
    Mixin_Call* new_Mixin_Call(std::string p, size_t l, std::string n, Arguments* args, Block* b = 0);
    // expressions
    List* new_List(std::string p, size_t l, size_t size = 0, enum Sass_Separator sep = List::space, bool argl = false);
    Map* new_Map(std::string p, size_t l, size_t size = 0);
    Binary_Expression<AND>* new_And(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<OR>* new_Or(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<EQ>* new_Eq(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<NEQ>* new_Neq(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<GT>* new_Gt(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<GTE>* new_Gte(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<LT>* new_Lt(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<LTE>* new_Lte(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<ADD>* new_Add(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<SUB>* new_Sub(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<MUL>* new_Mul(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Binary_Expression<DIV>* new_Div(std::string p, size_t l, Expression* lhs, Expression* rhs);
    Negation* new_Negation(std::string p, size_t l, Expression* o);
    Function_Call* new_Function_Call(std::string p, size_t l, String* n, Arguments* args);
    Variable* new_Variable(std::string p, size_t l, std::string n);
    Textual<NUMBER>* new_Textual_Number(std::string p, size_t l, std::string val);
    Textual<PERCENTAGE>* new_Textual_Percentage(std::string p, size_t l, std::string val);
    Textual<DIMENSION>* new_Textual_Dimension(std::string p, size_t l, std::string val);
    Textual<HEX>* new_Textual_Hex(std::string p, size_t l, std::string val);
    Number* new_Number(std::string p, size_t l, double val);
    Percentage* new_Percentage(std::string p, size_t l, double val);
    Dimension* new_Dimension(std::string p, size_t l, double val, std::string unit);
    Color* new_Color(std::string p, size_t l, double r, double g, double b, double a = 1, std::string disp = "");
    Boolean* new_Boolean(std::string p, size_t l, bool val);
    String_Schema* new_String_Schema(std::string p, size_t l, size_t size = 0);
    String_Constant* new_String_Constant(std::string p, size_t l, std::string val);
    String_Constant* new_String_Constant(std::string p, size_t l, const char* beg);
    String_Constant* new_String_Constant(std::string p, size_t l, const char* beg, const char* end);
    Supports_Condition* new_Supports_Condition(std::string p, size_t l, String* f, Expression* v);
    Media_Expression* new_Media_Expression(std::string p, size_t l, String* f, Expression* v);
    Parent_Selector* new_Parent_Selector(std::string p, size_t l, Selector* s);
    // parameters and arguments
    Parameter* new_Parameter(std::string p, size_t l, std::string n, Expression* def = 0, bool rest = false);
    Parameters* new_Parameters(std::string p, size_t l);
    Argument* new_Argument(std::string p, size_t l, Expression* val, std::string n = "", bool rest = false);
    Arguments* new_Arguments(std::string p, size_t l);
    // selectors
    Selector_Schema* new_Selector_Schema(std::string p, size_t l, String* c);
    Attribute_Selector* new_Attribute_Selector(std::string p, size_t l, std::string n, std::string m, String* v);
    Simple_Selector* new_Simple_Selector(std::string p, size_t l, std::string c);
    Reference_Selector* new_Reference_Selector(std::string p, size_t l);
    Placeholder_Selector* new_Placeholder_Selector(std::string p, size_t l, std::string n);
    Pseudo_Selector* new_Pseudo_Selector(std::string p, size_t l, std::string n, Expression* expr = 0);
    Wrapped_Selector* new_Wrapped_Selector(std::string p, size_t l, std::string n, Simple_Base* sel);
    Compound_Selector* new_Compound_Selector(std::string p, size_t l, size_t s = 0);
    Complex_Selector* new_Complex_Selector(std::string p, size_t l, Complex_Selector::Combinator c, Complex_Selector* ctx, Compound_Selector* sel);
    Selector_List* new_Selector_List(std::string p, size_t l, size_t s = 0);
  };
}

#endif
