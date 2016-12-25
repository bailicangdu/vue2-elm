#ifndef SASS_AST_H
#define SASS_AST_H

#include <set>
#include <deque>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <unordered_map>
#include "sass/base.h"

#ifdef __clang__

/*
 * There are some overloads used here that trigger the clang overload
 * hiding warning. Specifically:
 *
 * Type type() which hides string type() from Expression
 *
 * and
 *
 * Block* block() which hides virtual Block* block() from Statement
 *
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"

#endif

#include "util.hpp"
#include "units.hpp"
#include "context.hpp"
#include "position.hpp"
#include "constants.hpp"
#include "operation.hpp"
#include "position.hpp"
#include "inspect.hpp"
#include "source_map.hpp"
#include "environment.hpp"
#include "error_handling.hpp"
#include "ast_def_macros.hpp"
#include "ast_fwd_decl.hpp"
#include "source_map.hpp"

#include "sass.h"

namespace Sass {

  // ToDo: should this really be hardcoded
  // Note: most methods follow precision option
  const double NUMBER_EPSILON = 0.00000000000001;

  // ToDo: where does this fit best?
  // We don't share this with C-API?
  class Operand {
    public:
      Operand(Sass_OP operand, bool ws_before = false, bool ws_after = false)
      : operand(operand), ws_before(ws_before), ws_after(ws_after)
      { }
    public:
      enum Sass_OP operand;
      bool ws_before;
      bool ws_after;
  };

  // from boost (functional/hash):
  // http://www.boost.org/doc/libs/1_35_0/doc/html/hash/combine.html
  // Boost Software License - Version 1.0
  // http://www.boost.org/users/license.html
  template <typename T>
  void hash_combine (std::size_t& seed, const T& val)
  {
    seed ^= std::hash<T>()(val) + 0x9e3779b9
             + (seed<<6) + (seed>>2);
  }

  //////////////////////////////////////////////////////////
  // Abstract base class for all abstract syntax tree nodes.
  //////////////////////////////////////////////////////////
  class AST_Node : public Memory_Object {
    ADD_PROPERTY(ParserState, pstate)
  public:
    AST_Node(ParserState pstate)
    : pstate_(pstate)
    { }
    virtual ~AST_Node() = 0;
    virtual size_t hash() { return 0; }
    virtual std::string inspect() const { return to_string({ INSPECT, 5 }); }
    virtual std::string to_sass() const { return to_string({ TO_SASS, 5 }); }
    virtual std::string to_string(Sass_Inspect_Options opt) const;
    virtual std::string to_string() const;
    // virtual Block* block() { return 0; }
  public:
    void update_pstate(const ParserState& pstate);
  public:
    Offset off() { return pstate(); }
    Position pos() { return pstate(); }
    ATTACH_OPERATIONS()
  };
  inline AST_Node::~AST_Node() { }


  //////////////////////////////////////////////////////////////////////
  // Abstract base class for expressions. This side of the AST hierarchy
  // represents elements in value contexts, which exist primarily to be
  // evaluated and returned.
  //////////////////////////////////////////////////////////////////////
  class Expression : public AST_Node {
  public:
    enum Concrete_Type {
      NONE,
      BOOLEAN,
      NUMBER,
      COLOR,
      STRING,
      LIST,
      MAP,
      SELECTOR,
      NULL_VAL,
      C_WARNING,
      C_ERROR,
      NUM_TYPES
    };
  private:
    // expressions in some contexts shouldn't be evaluated
    ADD_PROPERTY(bool, is_delayed)
    ADD_PROPERTY(bool, is_expanded)
    ADD_PROPERTY(bool, is_interpolant)
    ADD_PROPERTY(Concrete_Type, concrete_type)
  public:
    Expression(ParserState pstate,
               bool d = false, bool e = false, bool i = false, Concrete_Type ct = NONE)
    : AST_Node(pstate),
      is_delayed_(d),
      is_expanded_(d),
      is_interpolant_(i),
      concrete_type_(ct)
    { }
    virtual operator bool() { return true; }
    virtual ~Expression() { }
    virtual std::string type() { return ""; /* TODO: raise an error? */ }
    virtual bool is_invisible() const { return false; }
    static std::string type_name() { return ""; }
    virtual bool is_false() { return false; }
    virtual bool operator== (const Expression& rhs) const { return false; }
    virtual void set_delayed(bool delayed) { is_delayed(delayed); }
    virtual bool has_interpolant() const { return is_interpolant(); }
    virtual bool is_left_interpolant() const { return is_interpolant(); }
    virtual bool is_right_interpolant() const { return is_interpolant(); }
    virtual std::string inspect() const { return to_string({ INSPECT, 5 }); }
    virtual std::string to_sass() const { return to_string({ TO_SASS, 5 }); }
    virtual size_t hash() { return 0; }
  };

  //////////////////////////////////////////////////////////////////////
  // Still just an expression, but with a to_string method
  //////////////////////////////////////////////////////////////////////
  class PreValue : public Expression {
  public:
    PreValue(ParserState pstate,
               bool d = false, bool e = false, bool i = false, Concrete_Type ct = NONE)
    : Expression(pstate, d, e, i, ct)
    { }
    virtual ~PreValue() { }
  };

  //////////////////////////////////////////////////////////////////////
  // base class for values that support operations
  //////////////////////////////////////////////////////////////////////
  class Value : public Expression {
  public:
    Value(ParserState pstate,
          bool d = false, bool e = false, bool i = false, Concrete_Type ct = NONE)
    : Expression(pstate, d, e, i, ct)
    { }
    virtual bool operator== (const Expression& rhs) const = 0;
  };
}

/////////////////////////////////////////////////////////////////////////////////////
// Hash method specializations for std::unordered_map to work with Sass::Expression
/////////////////////////////////////////////////////////////////////////////////////

namespace std {
  template<>
  struct hash<Sass::Expression*>
  {
    size_t operator()(Sass::Expression* s) const
    {
      return s->hash();
    }
  };
  template<>
  struct equal_to<Sass::Expression*>
  {
    bool operator()( Sass::Expression* lhs,  Sass::Expression* rhs) const
    {
      return lhs->hash() == rhs->hash();
    }
  };
}

namespace Sass {

  /////////////////////////////////////////////////////////////////////////////
  // Mixin class for AST nodes that should behave like vectors. Uses the
  // "Template Method" design pattern to allow subclasses to adjust their flags
  // when certain objects are pushed.
  /////////////////////////////////////////////////////////////////////////////
  template <typename T>
  class Vectorized {
    std::vector<T> elements_;
  protected:
    size_t hash_;
    void reset_hash() { hash_ = 0; }
    virtual void adjust_after_pushing(T element) { }
  public:
    Vectorized(size_t s = 0) : elements_(std::vector<T>())
    { elements_.reserve(s); }
    virtual ~Vectorized() = 0;
    size_t length() const   { return elements_.size(); }
    bool empty() const      { return elements_.empty(); }
    T last() const          { return elements_.back(); }
    T first() const         { return elements_.front(); }
    T& operator[](size_t i) { return elements_[i]; }
    virtual const T& at(size_t i) const { return elements_.at(i); }
    const T& operator[](size_t i) const { return elements_[i]; }
    Vectorized& operator<<(T element)
    {
      if (!element) return *this;
      reset_hash();
      elements_.push_back(element);
      adjust_after_pushing(element);
      return *this;
    }
    Vectorized& operator+=(Vectorized* v)
    {
      for (size_t i = 0, L = v->length(); i < L; ++i) *this << (*v)[i];
      return *this;
    }
    Vectorized& unshift(T element)
    {
      elements_.insert(elements_.begin(), element);
      return *this;
    }
    std::vector<T>& elements() { return elements_; }
    const std::vector<T>& elements() const { return elements_; }
    std::vector<T>& elements(std::vector<T>& e) { elements_ = e; return elements_; }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        for (T& el : elements_) {
          hash_combine(hash_, el->hash());
        }
      }
      return hash_;
    }

    typename std::vector<T>::iterator end() { return elements_.end(); }
    typename std::vector<T>::iterator begin() { return elements_.begin(); }
    typename std::vector<T>::const_iterator end() const { return elements_.end(); }
    typename std::vector<T>::const_iterator begin() const { return elements_.begin(); }
    typename std::vector<T>::iterator erase(typename std::vector<T>::iterator el) { return elements_.erase(el); }
    typename std::vector<T>::const_iterator erase(typename std::vector<T>::const_iterator el) { return elements_.erase(el); }

  };
  template <typename T>
  inline Vectorized<T>::~Vectorized() { }

  /////////////////////////////////////////////////////////////////////////////
  // Mixin class for AST nodes that should behave like a hash table. Uses an
  // extra <std::vector> internally to maintain insertion order for interation.
  /////////////////////////////////////////////////////////////////////////////
  class Hashed {
  struct HashExpression {
    size_t operator() (Expression* ex) const {
      return ex ? ex->hash() : 0;
    }
  };
  struct CompareExpression {
    bool operator()(const Expression* lhs, const Expression* rhs) const {
      return lhs && rhs && *lhs == *rhs;
    }
  };
  typedef std::unordered_map<
    Expression*, // key
    Expression*, // value
    HashExpression, // hasher
    CompareExpression // compare
  > ExpressionMap;
  private:
    ExpressionMap elements_;
    std::vector<Expression*> list_;
  protected:
    size_t hash_;
    Expression* duplicate_key_;
    void reset_hash() { hash_ = 0; }
    void reset_duplicate_key() { duplicate_key_ = 0; }
    virtual void adjust_after_pushing(std::pair<Expression*, Expression*> p) { }
  public:
    Hashed(size_t s = 0) : elements_(ExpressionMap(s)), list_(std::vector<Expression*>())
    { elements_.reserve(s); list_.reserve(s); reset_duplicate_key(); }
    virtual ~Hashed();
    size_t length() const                  { return list_.size(); }
    bool empty() const                     { return list_.empty(); }
    bool has(Expression* k) const          { return elements_.count(k) == 1; }
    Expression* at(Expression* k) const;
    bool has_duplicate_key() const         { return duplicate_key_ != 0; }
    Expression* get_duplicate_key() const  { return duplicate_key_; }
    const ExpressionMap elements() { return elements_; }
    Hashed& operator<<(std::pair<Expression*, Expression*> p)
    {
      reset_hash();

      if (!has(p.first)) list_.push_back(p.first);
      else if (!duplicate_key_) duplicate_key_ = p.first;

      elements_[p.first] = p.second;

      adjust_after_pushing(p);
      return *this;
    }
    Hashed& operator+=(Hashed* h)
    {
      if (length() == 0) {
        this->elements_ = h->elements_;
        this->list_ = h->list_;
        return *this;
      }

      for (auto key : h->keys()) {
        *this << std::make_pair(key, h->at(key));
      }

      reset_duplicate_key();
      return *this;
    }
    const ExpressionMap& pairs() const { return elements_; }
    const std::vector<Expression*>& keys() const { return list_; }

    std::unordered_map<Expression*, Expression*>::iterator end() { return elements_.end(); }
    std::unordered_map<Expression*, Expression*>::iterator begin() { return elements_.begin(); }
    std::unordered_map<Expression*, Expression*>::const_iterator end() const { return elements_.end(); }
    std::unordered_map<Expression*, Expression*>::const_iterator begin() const { return elements_.begin(); }

  };
  inline Hashed::~Hashed() { }


  /////////////////////////////////////////////////////////////////////////
  // Abstract base class for statements. This side of the AST hierarchy
  // represents elements in expansion contexts, which exist primarily to be
  // rewritten and macro-expanded.
  /////////////////////////////////////////////////////////////////////////
  class Statement : public AST_Node {
  public:
    enum Statement_Type {
      NONE,
      RULESET,
      MEDIA,
      DIRECTIVE,
      SUPPORTS,
      ATROOT,
      BUBBLE,
      CONTENT,
      KEYFRAMERULE,
      DECLARATION,
      ASSIGNMENT,
      IMPORT_STUB,
      IMPORT,
      COMMENT,
      WARNING,
      RETURN,
      EXTEND,
      ERROR,
      DEBUGSTMT,
      WHILE,
      EACH,
      FOR,
      IF
    };
  private:
    ADD_PROPERTY(Block*, block)
    ADD_PROPERTY(Statement_Type, statement_type)
    ADD_PROPERTY(size_t, tabs)
    ADD_PROPERTY(bool, group_end)
  public:
    Statement(ParserState pstate, Statement_Type st = NONE, size_t t = 0)
    : AST_Node(pstate), statement_type_(st), tabs_(t), group_end_(false)
     { }
    virtual ~Statement() = 0;
    // needed for rearranging nested rulesets during CSS emission
    virtual bool   is_hoistable() { return false; }
    virtual bool   is_invisible() const { return false; }
    virtual bool   bubbles() { return false; }
    virtual Block* block()  { return 0; }
    virtual bool has_content()
    {
      return statement_type_ == CONTENT;
    }
  };
  inline Statement::~Statement() { }

  ////////////////////////
  // Blocks of statements.
  ////////////////////////
  class Block : public Statement, public Vectorized<Statement*> {
    ADD_PROPERTY(bool, is_root)
    ADD_PROPERTY(bool, is_at_root);
    // needed for properly formatted CSS emission
    ADD_PROPERTY(bool, has_hoistable)
    ADD_PROPERTY(bool, has_non_hoistable)
  protected:
    void adjust_after_pushing(Statement* s)
    {
      if (s->is_hoistable()) has_hoistable_     = true;
      else                   has_non_hoistable_ = true;
    }
  public:
    Block(ParserState pstate, size_t s = 0, bool r = false)
    : Statement(pstate),
      Vectorized<Statement*>(s),
      is_root_(r),
      is_at_root_(false),
      has_hoistable_(false),
      has_non_hoistable_(false)
    { }
    virtual bool has_content()
    {
      for (size_t i = 0, L = elements().size(); i < L; ++i) {
        if (elements()[i]->has_content()) return true;
      }
      return Statement::has_content();
    }
    Block* block() { return this; }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////
  // Abstract base class for statements that contain blocks of statements.
  ////////////////////////////////////////////////////////////////////////
  class Has_Block : public Statement {
    ADD_PROPERTY(Block*, block)
  public:
    Has_Block(ParserState pstate, Block* b)
    : Statement(pstate), block_(b)
    { }
    virtual bool has_content()
    {
      return (block_ && block_->has_content()) || Statement::has_content();
    }
    virtual ~Has_Block() = 0;
  };
  inline Has_Block::~Has_Block() { }

  /////////////////////////////////////////////////////////////////////////////
  // Rulesets (i.e., sets of styles headed by a selector and containing a block
  // of style declarations.
  /////////////////////////////////////////////////////////////////////////////
  class Ruleset : public Has_Block {
    ADD_PROPERTY(Selector*, selector)
    ADD_PROPERTY(bool, at_root);
    ADD_PROPERTY(bool, is_root);
  public:
    Ruleset(ParserState pstate, Selector* s = 0, Block* b = 0)
    : Has_Block(pstate, b), selector_(s), at_root_(false), is_root_(false)
    { statement_type(RULESET); }
    bool is_invisible() const;
    // nested rulesets need to be hoisted out of their enclosing blocks
    bool is_hoistable() { return true; }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////////
  // Nested declaration sets (i.e., namespaced properties).
  /////////////////////////////////////////////////////////
  class Propset : public Has_Block {
    ADD_PROPERTY(String*, property_fragment)
  public:
    Propset(ParserState pstate, String* pf, Block* b = 0)
    : Has_Block(pstate, b), property_fragment_(pf)
    { }
    ATTACH_OPERATIONS()
  };

  /////////////////
  // Bubble.
  /////////////////
  class Bubble : public Statement {
    ADD_PROPERTY(Statement*, node)
    ADD_PROPERTY(bool, group_end)
  public:
    Bubble(ParserState pstate, Statement* n, Statement* g = 0, size_t t = 0)
    : Statement(pstate, Statement::BUBBLE, t), node_(n), group_end_(g == 0)
    { }
    bool bubbles() { return true; }
    ATTACH_OPERATIONS()
  };

  /////////////////
  // Media queries.
  /////////////////
  class Media_Block : public Has_Block {
    ADD_PROPERTY(List*, media_queries)
  public:
    Media_Block(ParserState pstate, List* mqs, Block* b)
    : Has_Block(pstate, b), media_queries_(mqs)
    { statement_type(MEDIA); }
    Media_Block(ParserState pstate, List* mqs, Block* b, Selector* s)
    : Has_Block(pstate, b), media_queries_(mqs)
    { statement_type(MEDIA); }
    bool bubbles() { return true; }
    bool is_hoistable() { return true; }
    bool is_invisible() const;
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////////////////////////////
  // At-rules -- arbitrary directives beginning with "@" that may have an
  // optional statement block.
  ///////////////////////////////////////////////////////////////////////
  class Directive : public Has_Block {
    ADD_PROPERTY(std::string, keyword)
    ADD_PROPERTY(Selector*, selector)
    ADD_PROPERTY(Expression*, value)
  public:
    Directive(ParserState pstate, std::string kwd, Selector* sel = 0, Block* b = 0, Expression* val = 0)
    : Has_Block(pstate, b), keyword_(kwd), selector_(sel), value_(val) // set value manually if needed
    { statement_type(DIRECTIVE); }
    bool bubbles() { return is_keyframes() || is_media(); }
    bool is_media() {
      return keyword_.compare("@-webkit-media") == 0 ||
             keyword_.compare("@-moz-media") == 0 ||
             keyword_.compare("@-o-media") == 0 ||
             keyword_.compare("@media") == 0;
    }
    bool is_keyframes() {
      return keyword_.compare("@-webkit-keyframes") == 0 ||
             keyword_.compare("@-moz-keyframes") == 0 ||
             keyword_.compare("@-o-keyframes") == 0 ||
             keyword_.compare("@keyframes") == 0;
    }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////////////////////////////
  // Keyframe-rules -- the child blocks of "@keyframes" nodes.
  ///////////////////////////////////////////////////////////////////////
  class Keyframe_Rule : public Has_Block {
    ADD_PROPERTY(Selector*, selector)
  public:
    Keyframe_Rule(ParserState pstate, Block* b)
    : Has_Block(pstate, b), selector_(0)
    { statement_type(KEYFRAMERULE); }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////
  // Declarations -- style rules consisting of a property name and values.
  ////////////////////////////////////////////////////////////////////////
  class Declaration : public Statement {
    ADD_PROPERTY(String*, property)
    ADD_PROPERTY(Expression*, value)
    ADD_PROPERTY(bool, is_important)
    ADD_PROPERTY(bool, is_indented)
  public:
    Declaration(ParserState pstate,
                String* prop, Expression* val, bool i = false)
    : Statement(pstate), property_(prop), value_(val), is_important_(i), is_indented_(false)
    { statement_type(DECLARATION); }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////
  // Assignments -- variable and value.
  /////////////////////////////////////
  class Assignment : public Statement {
    ADD_PROPERTY(std::string, variable)
    ADD_PROPERTY(Expression*, value)
    ADD_PROPERTY(bool, is_default)
    ADD_PROPERTY(bool, is_global)
  public:
    Assignment(ParserState pstate,
               std::string var, Expression* val,
               bool is_default = false,
               bool is_global = false)
    : Statement(pstate), variable_(var), value_(val), is_default_(is_default), is_global_(is_global)
    { statement_type(ASSIGNMENT); }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////////
  // Import directives. CSS and Sass import lists can be intermingled, so it's
  // necessary to store a list of each in an Import node.
  ////////////////////////////////////////////////////////////////////////////
  class Import : public Statement {
    std::vector<Expression*> urls_;
    std::vector<Include>     incs_;
    ADD_PROPERTY(List*,      media_queries);
  public:
    Import(ParserState pstate)
    : Statement(pstate),
      urls_(std::vector<Expression*>()),
      incs_(std::vector<Include>()),
      media_queries_(0)
    { statement_type(IMPORT); }
    std::vector<Expression*>& urls() { return urls_; }
    std::vector<Include>& incs() { return incs_; }
    ATTACH_OPERATIONS()
  };

  // not yet resolved single import
  // so far we only know requested name
  class Import_Stub : public Statement {
    Include resource_;
  public:
    std::string abs_path() { return resource_.abs_path; };
    std::string imp_path() { return resource_.imp_path; };
    Include resource() { return resource_; };

    Import_Stub(ParserState pstate, Include res)
    : Statement(pstate), resource_(res)
    { statement_type(IMPORT_STUB); }
    ATTACH_OPERATIONS()
  };

  //////////////////////////////
  // The Sass `@warn` directive.
  //////////////////////////////
  class Warning : public Statement {
    ADD_PROPERTY(Expression*, message)
  public:
    Warning(ParserState pstate, Expression* msg)
    : Statement(pstate), message_(msg)
    { statement_type(WARNING); }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////
  // The Sass `@error` directive.
  ///////////////////////////////
  class Error : public Statement {
    ADD_PROPERTY(Expression*, message)
  public:
    Error(ParserState pstate, Expression* msg)
    : Statement(pstate), message_(msg)
    { statement_type(ERROR); }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////
  // The Sass `@debug` directive.
  ///////////////////////////////
  class Debug : public Statement {
    ADD_PROPERTY(Expression*, value)
  public:
    Debug(ParserState pstate, Expression* val)
    : Statement(pstate), value_(val)
    { statement_type(DEBUGSTMT); }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////
  // CSS comments. These may be interpolated.
  ///////////////////////////////////////////
  class Comment : public Statement {
    ADD_PROPERTY(String*, text)
    ADD_PROPERTY(bool, is_important)
  public:
    Comment(ParserState pstate, String* txt, bool is_important)
    : Statement(pstate), text_(txt), is_important_(is_important)
    { statement_type(COMMENT); }
    virtual bool is_invisible() const
    { return /* is_important() == */ false; }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////
  // The Sass `@if` control directive.
  ////////////////////////////////////
  class If : public Has_Block {
    ADD_PROPERTY(Expression*, predicate)
    ADD_PROPERTY(Block*, alternative)
  public:
    If(ParserState pstate, Expression* pred, Block* con, Block* alt = 0)
    : Has_Block(pstate, con), predicate_(pred), alternative_(alt)
    { statement_type(IF); }
    virtual bool has_content()
    {
      return Has_Block::has_content() || (alternative_ && alternative_->has_content());
    }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////
  // The Sass `@for` control directive.
  /////////////////////////////////////
  class For : public Has_Block {
    ADD_PROPERTY(std::string, variable)
    ADD_PROPERTY(Expression*, lower_bound)
    ADD_PROPERTY(Expression*, upper_bound)
    ADD_PROPERTY(bool, is_inclusive)
  public:
    For(ParserState pstate,
        std::string var, Expression* lo, Expression* hi, Block* b, bool inc)
    : Has_Block(pstate, b),
      variable_(var), lower_bound_(lo), upper_bound_(hi), is_inclusive_(inc)
    { statement_type(FOR); }
    ATTACH_OPERATIONS()
  };

  //////////////////////////////////////
  // The Sass `@each` control directive.
  //////////////////////////////////////
  class Each : public Has_Block {
    ADD_PROPERTY(std::vector<std::string>, variables)
    ADD_PROPERTY(Expression*, list)
  public:
    Each(ParserState pstate, std::vector<std::string> vars, Expression* lst, Block* b)
    : Has_Block(pstate, b), variables_(vars), list_(lst)
    { statement_type(EACH); }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////
  // The Sass `@while` control directive.
  ///////////////////////////////////////
  class While : public Has_Block {
    ADD_PROPERTY(Expression*, predicate)
  public:
    While(ParserState pstate, Expression* pred, Block* b)
    : Has_Block(pstate, b), predicate_(pred)
    { statement_type(WHILE); }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////////////
  // The @return directive for use inside SassScript functions.
  /////////////////////////////////////////////////////////////
  class Return : public Statement {
    ADD_PROPERTY(Expression*, value)
  public:
    Return(ParserState pstate, Expression* val)
    : Statement(pstate), value_(val)
    { statement_type(RETURN); }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////
  // The Sass `@extend` directive.
  ////////////////////////////////
  class Extension : public Statement {
    ADD_PROPERTY(Selector*, selector)
  public:
    Extension(ParserState pstate, Selector* s)
    : Statement(pstate), selector_(s)
    { statement_type(EXTEND); }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////////////////////////////
  // Definitions for both mixins and functions. The two cases are distinguished
  // by a type tag.
  /////////////////////////////////////////////////////////////////////////////
  struct Backtrace;
  typedef Environment<AST_Node*> Env;
  typedef const char* Signature;
  typedef Expression* (*Native_Function)(Env&, Env&, Context&, Signature, ParserState, Backtrace*);
  typedef const char* Signature;
  class Definition : public Has_Block {
  public:
    enum Type { MIXIN, FUNCTION };
    ADD_PROPERTY(std::string, name)
    ADD_PROPERTY(Parameters*, parameters)
    ADD_PROPERTY(Env*, environment)
    ADD_PROPERTY(Type, type)
    ADD_PROPERTY(Native_Function, native_function)
    ADD_PROPERTY(Sass_Function_Entry, c_function)
    ADD_PROPERTY(void*, cookie)
    ADD_PROPERTY(bool, is_overload_stub)
    ADD_PROPERTY(Signature, signature)
  public:
    Definition(ParserState pstate,
               std::string n,
               Parameters* params,
               Block* b,
               Type t)
    : Has_Block(pstate, b),
      name_(n),
      parameters_(params),
      environment_(0),
      type_(t),
      native_function_(0),
      c_function_(0),
      cookie_(0),
      is_overload_stub_(false),
      signature_(0)
    { }
    Definition(ParserState pstate,
               Signature sig,
               std::string n,
               Parameters* params,
               Native_Function func_ptr,
               bool overload_stub = false)
    : Has_Block(pstate, 0),
      name_(n),
      parameters_(params),
      environment_(0),
      type_(FUNCTION),
      native_function_(func_ptr),
      c_function_(0),
      cookie_(0),
      is_overload_stub_(overload_stub),
      signature_(sig)
    { }
    Definition(ParserState pstate,
               Signature sig,
               std::string n,
               Parameters* params,
               Sass_Function_Entry c_func,
               bool whatever,
               bool whatever2)
    : Has_Block(pstate, 0),
      name_(n),
      parameters_(params),
      environment_(0),
      type_(FUNCTION),
      native_function_(0),
      c_function_(c_func),
      cookie_(sass_function_get_cookie(c_func)),
      is_overload_stub_(false),
      signature_(sig)
    { }
    ATTACH_OPERATIONS()
  };

  //////////////////////////////////////
  // Mixin calls (i.e., `@include ...`).
  //////////////////////////////////////
  class Mixin_Call : public Has_Block {
    ADD_PROPERTY(std::string, name)
    ADD_PROPERTY(Arguments*, arguments)
  public:
    Mixin_Call(ParserState pstate, std::string n, Arguments* args, Block* b = 0)
    : Has_Block(pstate, b), name_(n), arguments_(args)
    { }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////////
  // The @content directive for mixin content blocks.
  ///////////////////////////////////////////////////
  class Content : public Statement {
    ADD_PROPERTY(Media_Block*, media_block)
  public:
    Content(ParserState pstate) : Statement(pstate)
    { statement_type(CONTENT); }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////////////////////////////
  // Lists of values, both comma- and space-separated (distinguished by a
  // type-tag.) Also used to represent variable-length argument lists.
  ///////////////////////////////////////////////////////////////////////
  class List : public Value, public Vectorized<Expression*> {
    void adjust_after_pushing(Expression* e) { is_expanded(false); }
  private:
    ADD_PROPERTY(enum Sass_Separator, separator)
    ADD_PROPERTY(bool, is_arglist)
    ADD_PROPERTY(bool, from_selector)
  public:
    List(ParserState pstate,
         size_t size = 0, enum Sass_Separator sep = SASS_SPACE, bool argl = false)
    : Value(pstate),
      Vectorized<Expression*>(size),
      separator_(sep),
      is_arglist_(argl),
      from_selector_(false)
    { concrete_type(LIST); }
    std::string type() { return is_arglist_ ? "arglist" : "list"; }
    static std::string type_name() { return "list"; }
    const char* sep_string(bool compressed = false) const {
      return separator() == SASS_SPACE ?
        " " : (compressed ? "," : ", ");
    }
    bool is_invisible() const { return empty(); }
    Expression* value_at_index(size_t i);

    virtual size_t size() const;

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<std::string>()(sep_string());
        for (size_t i = 0, L = length(); i < L; ++i)
          hash_combine(hash_, (elements()[i])->hash());
      }
      return hash_;
    }

    virtual void set_delayed(bool delayed)
    {
      for (size_t i = 0, L = length(); i < L; ++i)
        (elements()[i])->set_delayed(delayed);
      is_delayed(delayed);
    }

    virtual bool operator== (const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////////////////////////////
  // Key value paris.
  ///////////////////////////////////////////////////////////////////////
  class Map : public Value, public Hashed {
    void adjust_after_pushing(std::pair<Expression*, Expression*> p) { is_expanded(false); }
  public:
    Map(ParserState pstate,
         size_t size = 0)
    : Value(pstate),
      Hashed(size)
    { concrete_type(MAP); }
    std::string type() { return "map"; }
    static std::string type_name() { return "map"; }
    bool is_invisible() const { return empty(); }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        for (auto key : keys()) {
          hash_combine(hash_, key->hash());
          hash_combine(hash_, at(key)->hash());
        }
      }

      return hash_;
    }

    virtual bool operator== (const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  inline static const std::string sass_op_to_name(enum Sass_OP op) {
    switch (op) {
      case AND: return "and"; break;
      case OR: return "or"; break;
      case EQ: return "eq"; break;
      case NEQ: return "neq"; break;
      case GT: return "gt"; break;
      case GTE: return "gte"; break;
      case LT: return "lt"; break;
      case LTE: return "lte"; break;
      case ADD: return "plus"; break;
      case SUB: return "sub"; break;
      case MUL: return "times"; break;
      case DIV: return "div"; break;
      case MOD: return "mod"; break;
      // this is only used internally!
      case NUM_OPS: return "[OPS]"; break;
      default: return "invalid"; break;
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // Binary expressions. Represents logical, relational, and arithmetic
  // operations. Templatized to avoid large switch statements and repetitive
  // subclassing.
  //////////////////////////////////////////////////////////////////////////
  class Binary_Expression : public PreValue {
  private:
    ADD_HASHED(Operand, op)
    ADD_HASHED(Expression*, left)
    ADD_HASHED(Expression*, right)
    size_t hash_;
  public:
    Binary_Expression(ParserState pstate,
                      Operand op, Expression* lhs, Expression* rhs)
    : PreValue(pstate), op_(op), left_(lhs), right_(rhs), hash_(0)
    { }
    const std::string type_name() {
      switch (type()) {
        case AND: return "and"; break;
        case OR: return "or"; break;
        case EQ: return "eq"; break;
        case NEQ: return "neq"; break;
        case GT: return "gt"; break;
        case GTE: return "gte"; break;
        case LT: return "lt"; break;
        case LTE: return "lte"; break;
        case ADD: return "add"; break;
        case SUB: return "sub"; break;
        case MUL: return "mul"; break;
        case DIV: return "div"; break;
        case MOD: return "mod"; break;
        // this is only used internally!
        case NUM_OPS: return "[OPS]"; break;
        default: return "invalid"; break;
      }
    }
    const std::string separator() {
      switch (type()) {
        case AND: return "&&"; break;
        case OR: return "||"; break;
        case EQ: return "=="; break;
        case NEQ: return "!="; break;
        case GT: return ">"; break;
        case GTE: return ">="; break;
        case LT: return "<"; break;
        case LTE: return "<="; break;
        case ADD: return "+"; break;
        case SUB: return "-"; break;
        case MUL: return "*"; break;
        case DIV: return "/"; break;
        case MOD: return "%"; break;
        // this is only used internally!
        case NUM_OPS: return "[OPS]"; break;
        default: return "invalid"; break;
      }
    }
    bool is_left_interpolant(void) const;
    bool is_right_interpolant(void) const;
    bool has_interpolant() const
    {
      return is_left_interpolant() ||
             is_right_interpolant();
    }
    virtual bool can_delay() const;
    void reset_whitespace()
    {
      op_.ws_before = false;
      op_.ws_after = false;
    }
    virtual void set_delayed(bool delayed)
    {
      right()->set_delayed(delayed);
      left()->set_delayed(delayed);
      is_delayed(delayed);
    }
    virtual bool operator==(const Expression& rhs) const
    {
      try
      {
        const Binary_Expression* m = dynamic_cast<const Binary_Expression*>(&rhs);
        if (m == 0) return false;
        return type() == m->type() &&
               left() == m->left() &&
               right() == m->right();
      }
      catch (std::bad_cast&)
      {
        return false;
      }
      catch (...) { throw; }
    }
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<size_t>()(type());
        hash_combine(hash_, left()->hash());
        hash_combine(hash_, right()->hash());
      }
      return hash_;
    }
    enum Sass_OP type() const { return op_.operand; }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////////
  // Arithmetic negation (logical negation is just an ordinary function call).
  ////////////////////////////////////////////////////////////////////////////
  class Unary_Expression : public Expression {
  public:
    enum Type { PLUS, MINUS, NOT };
  private:
    ADD_HASHED(Type, type)
    ADD_HASHED(Expression*, operand)
    size_t hash_;
  public:
    Unary_Expression(ParserState pstate, Type t, Expression* o)
    : Expression(pstate), type_(t), operand_(o), hash_(0)
    { }
    const std::string type_name() {
      switch (type_) {
        case PLUS: return "plus"; break;
        case MINUS: return "minus"; break;
        case NOT: return "not"; break;
        default: return "invalid"; break;
      }
    }
    virtual bool operator==(const Expression& rhs) const
    {
      try
      {
        const Unary_Expression* m = dynamic_cast<const Unary_Expression*>(&rhs);
        if (m == 0) return false;
        return type() == m->type() &&
               operand() == m->operand();
      }
      catch (std::bad_cast&)
      {
        return false;
      }
      catch (...) { throw; }
    }
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<size_t>()(type_);
        hash_combine(hash_, operand()->hash());
      };
      return hash_;
    }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////
  // Individual argument objects for mixin and function calls.
  ////////////////////////////////////////////////////////////
  class Argument : public Expression {
    ADD_HASHED(Expression*, value)
    ADD_HASHED(std::string, name)
    ADD_PROPERTY(bool, is_rest_argument)
    ADD_PROPERTY(bool, is_keyword_argument)
    size_t hash_;
  public:
    Argument(ParserState pstate, Expression* val, std::string n = "", bool rest = false, bool keyword = false)
    : Expression(pstate), value_(val), name_(n), is_rest_argument_(rest), is_keyword_argument_(keyword), hash_(0)
    {
      if (!name_.empty() && is_rest_argument_) {
        error("variable-length argument may not be passed by name", pstate);
      }
    }

    virtual bool operator==(const Expression& rhs) const
    {
      try
      {
        const Argument* m = dynamic_cast<const Argument*>(&rhs);
        if (!(m && name() == m->name())) return false;
        return *value() == *m->value();
      }
      catch (std::bad_cast&)
      {
        return false;
      }
      catch (...) { throw; }
    }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<std::string>()(name());
        hash_combine(hash_, value()->hash());
      }
      return hash_;
    }

    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////
  // Argument lists -- in their own class to facilitate context-sensitive
  // error checking (e.g., ensuring that all ordinal arguments precede all
  // named arguments).
  ////////////////////////////////////////////////////////////////////////
  class Arguments : public Expression, public Vectorized<Argument*> {
    ADD_PROPERTY(bool, has_named_arguments)
    ADD_PROPERTY(bool, has_rest_argument)
    ADD_PROPERTY(bool, has_keyword_argument)
  protected:
    void adjust_after_pushing(Argument* a);
  public:
    Arguments(ParserState pstate)
    : Expression(pstate),
      Vectorized<Argument*>(),
      has_named_arguments_(false),
      has_rest_argument_(false),
      has_keyword_argument_(false)
    { }

    Argument* get_rest_argument();
    Argument* get_keyword_argument();

    ATTACH_OPERATIONS()
  };

  //////////////////
  // Function calls.
  //////////////////
  class Function_Call : public PreValue {
    ADD_HASHED(std::string, name)
    ADD_HASHED(Arguments*, arguments)
    ADD_PROPERTY(void*, cookie)
    size_t hash_;
  public:
    Function_Call(ParserState pstate, std::string n, Arguments* args, void* cookie)
    : PreValue(pstate), name_(n), arguments_(args), cookie_(cookie), hash_(0)
    { concrete_type(STRING); }
    Function_Call(ParserState pstate, std::string n, Arguments* args)
    : PreValue(pstate), name_(n), arguments_(args), cookie_(0), hash_(0)
    { concrete_type(STRING); }

    virtual bool operator==(const Expression& rhs) const
    {
      try
      {
        const Function_Call* m = dynamic_cast<const Function_Call*>(&rhs);
        if (!(m && name() == m->name())) return false;
        if (!(m && arguments()->length() == m->arguments()->length())) return false;
        for (size_t i =0, L = arguments()->length(); i < L; ++i)
          if (!((*arguments())[i] == (*m->arguments())[i])) return false;
        return true;
      }
      catch (std::bad_cast&)
      {
        return false;
      }
      catch (...) { throw; }
    }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<std::string>()(name());
        for (auto argument : arguments()->elements())
          hash_combine(hash_, argument->hash());
      }
      return hash_;
    }

    ATTACH_OPERATIONS()
  };

  /////////////////////////
  // Function call schemas.
  /////////////////////////
  class Function_Call_Schema : public Expression {
    ADD_PROPERTY(String*, name)
    ADD_PROPERTY(Arguments*, arguments)
  public:
    Function_Call_Schema(ParserState pstate, String* n, Arguments* args)
    : Expression(pstate), name_(n), arguments_(args)
    { concrete_type(STRING); }
    ATTACH_OPERATIONS()
  };

  ///////////////////////
  // Variable references.
  ///////////////////////
  class Variable : public PreValue {
    ADD_PROPERTY(std::string, name)
  public:
    Variable(ParserState pstate, std::string n)
    : PreValue(pstate), name_(n)
    { }

    virtual bool operator==(const Expression& rhs) const
    {
      try
      {
        const Variable* e = dynamic_cast<const Variable*>(&rhs);
        return e && name() == e->name();
      }
      catch (std::bad_cast&)
      {
        return false;
      }
      catch (...) { throw; }
    }

    virtual size_t hash()
    {
      return std::hash<std::string>()(name());
    }

    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////////
  // Textual (i.e., unevaluated) numeric data. Variants are distinguished with
  // a type tag.
  ////////////////////////////////////////////////////////////////////////////
  class Textual : public Expression {
  public:
    enum Type { NUMBER, PERCENTAGE, DIMENSION, HEX };
  private:
    ADD_HASHED(Type, type)
    ADD_HASHED(std::string, value)
    size_t hash_;
  public:
    Textual(ParserState pstate, Type t, std::string val)
    : Expression(pstate, true), type_(t), value_(val),
      hash_(0)
    { }

    virtual bool operator==(const Expression& rhs) const
    {
      try
      {
        const Textual* e = dynamic_cast<const Textual*>(&rhs);
        return e && value() == e->value() && type() == e->type();
      }
      catch (std::bad_cast&)
      {
        return false;
      }
      catch (...) { throw; }
    }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<std::string>()(value_);
        hash_combine(hash_, std::hash<int>()(type_));
      }
      return hash_;
    }

    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////
  // Numbers, percentages, dimensions, and colors.
  ////////////////////////////////////////////////
  class Number : public Value {
    ADD_HASHED(double, value)
    ADD_PROPERTY(bool, zero)
    std::vector<std::string> numerator_units_;
    std::vector<std::string> denominator_units_;
    size_t hash_;
  public:
    Number(ParserState pstate, double val, std::string u = "", bool zero = true);
    bool zero() { return zero_; }
    bool is_valid_css_unit() const;
    std::vector<std::string>& numerator_units()   { return numerator_units_; }
    std::vector<std::string>& denominator_units() { return denominator_units_; }
    const std::vector<std::string>& numerator_units() const   { return numerator_units_; }
    const std::vector<std::string>& denominator_units() const { return denominator_units_; }
    std::string type() { return "number"; }
    static std::string type_name() { return "number"; }
    std::string unit() const;

    bool is_unitless() const;
    double convert_factor(const Number&) const;
    bool convert(const std::string& unit = "", bool strict = false);
    void normalize(const std::string& unit = "", bool strict = false);
    // useful for making one number compatible with another
    std::string find_convertible_unit() const;

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<double>()(value_);
        for (const auto numerator : numerator_units())
          hash_combine(hash_, std::hash<std::string>()(numerator));
        for (const auto denominator : denominator_units())
          hash_combine(hash_, std::hash<std::string>()(denominator));
      }
      return hash_;
    }

    virtual bool operator< (const Number& rhs) const;
    virtual bool operator== (const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  //////////
  // Colors.
  //////////
  class Color : public Value {
    ADD_HASHED(double, r)
    ADD_HASHED(double, g)
    ADD_HASHED(double, b)
    ADD_HASHED(double, a)
    ADD_PROPERTY(std::string, disp)
    size_t hash_;
  public:
    Color(ParserState pstate, double r, double g, double b, double a = 1, const std::string disp = "")
    : Value(pstate), r_(r), g_(g), b_(b), a_(a), disp_(disp),
      hash_(0)
    { concrete_type(COLOR); }
    std::string type() { return "color"; }
    static std::string type_name() { return "color"; }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<double>()(a_);
        hash_combine(hash_, std::hash<double>()(r_));
        hash_combine(hash_, std::hash<double>()(g_));
        hash_combine(hash_, std::hash<double>()(b_));
      }
      return hash_;
    }

    virtual bool operator== (const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  //////////////////////////////
  // Errors from Sass_Values.
  //////////////////////////////
  class Custom_Error : public Value {
    ADD_PROPERTY(std::string, message)
  public:
    Custom_Error(ParserState pstate, std::string msg)
    : Value(pstate), message_(msg)
    { concrete_type(C_ERROR); }
    virtual bool operator== (const Expression& rhs) const;
    ATTACH_OPERATIONS()
  };

  //////////////////////////////
  // Warnings from Sass_Values.
  //////////////////////////////
  class Custom_Warning : public Value {
    ADD_PROPERTY(std::string, message)
  public:
    Custom_Warning(ParserState pstate, std::string msg)
    : Value(pstate), message_(msg)
    { concrete_type(C_WARNING); }
    virtual bool operator== (const Expression& rhs) const;
    ATTACH_OPERATIONS()
  };

  ////////////
  // Booleans.
  ////////////
  class Boolean : public Value {
    ADD_HASHED(bool, value)
    size_t hash_;
  public:
    Boolean(ParserState pstate, bool val)
    : Value(pstate), value_(val),
      hash_(0)
    { concrete_type(BOOLEAN); }
    virtual operator bool() { return value_; }
    std::string type() { return "bool"; }
    static std::string type_name() { return "bool"; }
    virtual bool is_false() { return !value_; }

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<bool>()(value_);
      }
      return hash_;
    }

    virtual bool operator== (const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////
  // Abstract base class for Sass string values. Includes interpolated and
  // "flat" strings.
  ////////////////////////////////////////////////////////////////////////
  class String : public Value {
    ADD_PROPERTY(bool, sass_fix_1291)
  public:
    String(ParserState pstate, bool delayed = false, bool sass_fix_1291 = false)
    : Value(pstate, delayed), sass_fix_1291_(sass_fix_1291)
    { concrete_type(STRING); }
    static std::string type_name() { return "string"; }
    virtual ~String() = 0;
    virtual void rtrim() = 0;
    virtual void ltrim() = 0;
    virtual void trim() = 0;
    virtual bool operator==(const Expression& rhs) const = 0;
    ATTACH_OPERATIONS()
  };
  inline String::~String() { };

  ///////////////////////////////////////////////////////////////////////
  // Interpolated strings. Meant to be reduced to flat strings during the
  // evaluation phase.
  ///////////////////////////////////////////////////////////////////////
  class String_Schema : public String, public Vectorized<Expression*> {
    // ADD_PROPERTY(bool, has_interpolants)
    size_t hash_;
  public:
    String_Schema(ParserState pstate, size_t size = 0, bool has_interpolants = false)
    : String(pstate), Vectorized<Expression*>(size), hash_(0)
    { concrete_type(STRING); }
    std::string type() { return "string"; }
    static std::string type_name() { return "string"; }

    bool is_left_interpolant(void) const;
    bool is_right_interpolant(void) const;
    // void has_interpolants(bool tc) { }
    bool has_interpolants() {
      for (auto el : elements()) {
        if (el->is_interpolant()) return true;
      }
      return false;
    }
    virtual void rtrim();
    virtual void ltrim();
    virtual void trim();

    virtual size_t hash()
    {
      if (hash_ == 0) {
        for (auto string : elements())
          hash_combine(hash_, string->hash());
      }
      return hash_;
    }

    virtual bool operator==(const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////
  // Flat strings -- the lowest level of raw textual data.
  ////////////////////////////////////////////////////////
  class String_Constant : public String {
    ADD_PROPERTY(char, quote_mark)
    ADD_PROPERTY(bool, can_compress_whitespace)
    ADD_HASHED(std::string, value)
  protected:
    size_t hash_;
  public:
    String_Constant(ParserState pstate, std::string val)
    : String(pstate), quote_mark_(0), can_compress_whitespace_(false), value_(read_css_string(val)), hash_(0)
    { }
    String_Constant(ParserState pstate, const char* beg)
    : String(pstate), quote_mark_(0), can_compress_whitespace_(false), value_(read_css_string(std::string(beg))), hash_(0)
    { }
    String_Constant(ParserState pstate, const char* beg, const char* end)
    : String(pstate), quote_mark_(0), can_compress_whitespace_(false), value_(read_css_string(std::string(beg, end-beg))), hash_(0)
    { }
    String_Constant(ParserState pstate, const Token& tok)
    : String(pstate), quote_mark_(0), can_compress_whitespace_(false), value_(read_css_string(std::string(tok.begin, tok.end))), hash_(0)
    { }
    std::string type() { return "string"; }
    static std::string type_name() { return "string"; }
    virtual bool is_invisible() const;
    virtual void rtrim();
    virtual void ltrim();
    virtual void trim();

    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_ = std::hash<std::string>()(value_);
      }
      return hash_;
    }

    virtual bool operator==(const Expression& rhs) const;
    virtual std::string inspect() const; // quotes are forced on inspection

    // static char auto_quote() { return '*'; }
    static char double_quote() { return '"'; }
    static char single_quote() { return '\''; }

    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////
  // Possibly quoted string (unquote on instantiation)
  ////////////////////////////////////////////////////////
  class String_Quoted : public String_Constant {
  public:
    String_Quoted(ParserState pstate, std::string val, char q = 0, bool keep_utf8_escapes = false)
    : String_Constant(pstate, val)
    {
      value_ = unquote(value_, &quote_mark_, keep_utf8_escapes);
      if (q && quote_mark_) quote_mark_ = q;
    }
    virtual bool operator==(const Expression& rhs) const;
    virtual std::string inspect() const; // quotes are forced on inspection
    ATTACH_OPERATIONS()
  };

  /////////////////
  // Media queries.
  /////////////////
  class Media_Query : public Expression,
                      public Vectorized<Media_Query_Expression*> {
    ADD_PROPERTY(String*, media_type)
    ADD_PROPERTY(bool, is_negated)
    ADD_PROPERTY(bool, is_restricted)
  public:
    Media_Query(ParserState pstate,
                String* t = 0, size_t s = 0, bool n = false, bool r = false)
    : Expression(pstate), Vectorized<Media_Query_Expression*>(s),
      media_type_(t), is_negated_(n), is_restricted_(r)
    { }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////
  // Media expressions (for use inside media queries).
  ////////////////////////////////////////////////////
  class Media_Query_Expression : public Expression {
    ADD_PROPERTY(Expression*, feature)
    ADD_PROPERTY(Expression*, value)
    ADD_PROPERTY(bool, is_interpolated)
  public:
    Media_Query_Expression(ParserState pstate,
                           Expression* f, Expression* v, bool i = false)
    : Expression(pstate), feature_(f), value_(v), is_interpolated_(i)
    { }
    ATTACH_OPERATIONS()
  };

  ////////////////////
  // `@supports` rule.
  ////////////////////
  class Supports_Block : public Has_Block {
    ADD_PROPERTY(Supports_Condition*, condition)
  public:
    Supports_Block(ParserState pstate, Supports_Condition* condition, Block* block = 0)
    : Has_Block(pstate, block), condition_(condition)
    { statement_type(SUPPORTS); }
    bool is_hoistable() { return true; }
    bool bubbles() { return true; }
    ATTACH_OPERATIONS()
  };

  //////////////////////////////////////////////////////
  // The abstract superclass of all Supports conditions.
  //////////////////////////////////////////////////////
  class Supports_Condition : public Expression {
  public:
    Supports_Condition(ParserState pstate)
    : Expression(pstate)
    { }
    virtual bool needs_parens(Supports_Condition* cond) const { return false; }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////
  // An operator condition (e.g. `CONDITION1 and CONDITION2`).
  ////////////////////////////////////////////////////////////
  class Supports_Operator : public Supports_Condition {
  public:
    enum Operand { AND, OR };
  private:
    ADD_PROPERTY(Supports_Condition*, left);
    ADD_PROPERTY(Supports_Condition*, right);
    ADD_PROPERTY(Operand, operand);
  public:
    Supports_Operator(ParserState pstate, Supports_Condition* l, Supports_Condition* r, Operand o)
    : Supports_Condition(pstate), left_(l), right_(r), operand_(o)
    { }
    virtual bool needs_parens(Supports_Condition* cond) const;
    ATTACH_OPERATIONS()
  };

  //////////////////////////////////////////
  // A negation condition (`not CONDITION`).
  //////////////////////////////////////////
  class Supports_Negation : public Supports_Condition {
  private:
    ADD_PROPERTY(Supports_Condition*, condition);
  public:
    Supports_Negation(ParserState pstate, Supports_Condition* c)
    : Supports_Condition(pstate), condition_(c)
    { }
    virtual bool needs_parens(Supports_Condition* cond) const;
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////
  // A declaration condition (e.g. `(feature: value)`).
  /////////////////////////////////////////////////////
  class Supports_Declaration : public Supports_Condition {
  private:
    ADD_PROPERTY(Expression*, feature);
    ADD_PROPERTY(Expression*, value);
  public:
    Supports_Declaration(ParserState pstate, Expression* f, Expression* v)
    : Supports_Condition(pstate), feature_(f), value_(v)
    { }
    virtual bool needs_parens(Supports_Condition* cond) const { return false; }
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////
  // An interpolation condition (e.g. `#{$var}`).
  ///////////////////////////////////////////////
  class Supports_Interpolation : public Supports_Condition {
  private:
    ADD_PROPERTY(Expression*, value);
  public:
    Supports_Interpolation(ParserState pstate, Expression* v)
    : Supports_Condition(pstate), value_(v)
    { }
    virtual bool needs_parens(Supports_Condition* cond) const { return false; }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////
  // At root expressions (for use inside @at-root).
  /////////////////////////////////////////////////
  class At_Root_Query : public Expression {
  private:
    ADD_PROPERTY(Expression*, feature)
    ADD_PROPERTY(Expression*, value)
  public:
    At_Root_Query(ParserState pstate, Expression* f = 0, Expression* v = 0, bool i = false)
    : Expression(pstate), feature_(f), value_(v)
    { }
    bool exclude(std::string str);
    ATTACH_OPERATIONS()
  };

  ///////////
  // At-root.
  ///////////
  class At_Root_Block : public Has_Block {
    ADD_PROPERTY(At_Root_Query*, expression)
  public:
    At_Root_Block(ParserState pstate, Block* b = 0, At_Root_Query* e = 0)
    : Has_Block(pstate, b), expression_(e)
    { statement_type(ATROOT); }
    bool is_hoistable() { return true; }
    bool bubbles() { return true; }
    bool exclude_node(Statement* s) {
      if (s->statement_type() == Statement::DIRECTIVE)
      {
        return expression()->exclude(static_cast<Directive*>(s)->keyword().erase(0, 1));
      }
      if (s->statement_type() == Statement::MEDIA)
      {
        return expression()->exclude("media");
      }
      if (s->statement_type() == Statement::RULESET)
      {
        return expression()->exclude("rule");
      }
      if (s->statement_type() == Statement::SUPPORTS)
      {
        return expression()->exclude("supports");
      }
      if (static_cast<Directive*>(s)->is_keyframes())
      {
        return expression()->exclude("keyframes");
      }
      return false;
    }
    ATTACH_OPERATIONS()
  };

  //////////////////
  // The null value.
  //////////////////
  class Null : public Value {
  public:
    Null(ParserState pstate) : Value(pstate) { concrete_type(NULL_VAL); }
    std::string type() { return "null"; }
    static std::string type_name() { return "null"; }
    bool is_invisible() const { return true; }
    operator bool() { return false; }
    bool is_false() { return true; }

    virtual size_t hash()
    {
      return -1;
    }

    virtual bool operator== (const Expression& rhs) const;

    ATTACH_OPERATIONS()
  };

  /////////////////////////////////
  // Thunks for delayed evaluation.
  /////////////////////////////////
  class Thunk : public Expression {
    ADD_PROPERTY(Expression*, expression)
    ADD_PROPERTY(Env*, environment)
  public:
    Thunk(ParserState pstate, Expression* exp, Env* env = 0)
    : Expression(pstate), expression_(exp), environment_(env)
    { }
  };

  /////////////////////////////////////////////////////////
  // Individual parameter objects for mixins and functions.
  /////////////////////////////////////////////////////////
  class Parameter : public AST_Node {
    ADD_PROPERTY(std::string, name)
    ADD_PROPERTY(Expression*, default_value)
    ADD_PROPERTY(bool, is_rest_parameter)
  public:
    Parameter(ParserState pstate,
              std::string n, Expression* def = 0, bool rest = false)
    : AST_Node(pstate), name_(n), default_value_(def), is_rest_parameter_(rest)
    {
      if (default_value_ && is_rest_parameter_) {
        error("variable-length parameter may not have a default value", pstate);
      }
    }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////////////////////////
  // Parameter lists -- in their own class to facilitate context-sensitive
  // error checking (e.g., ensuring that all optional parameters follow all
  // required parameters).
  /////////////////////////////////////////////////////////////////////////
  class Parameters : public AST_Node, public Vectorized<Parameter*> {
    ADD_PROPERTY(bool, has_optional_parameters)
    ADD_PROPERTY(bool, has_rest_parameter)
  protected:
    void adjust_after_pushing(Parameter* p)
    {
      if (p->default_value()) {
        if (has_rest_parameter_) {
          error("optional parameters may not be combined with variable-length parameters", p->pstate());
        }
        has_optional_parameters_ = true;
      }
      else if (p->is_rest_parameter()) {
        if (has_rest_parameter_) {
          error("functions and mixins cannot have more than one variable-length parameter", p->pstate());
        }
        has_rest_parameter_ = true;
      }
      else {
        if (has_rest_parameter_) {
          error("required parameters must precede variable-length parameters", p->pstate());
        }
        if (has_optional_parameters_) {
          error("required parameters must precede optional parameters", p->pstate());
        }
      }
    }
  public:
    Parameters(ParserState pstate)
    : AST_Node(pstate),
      Vectorized<Parameter*>(),
      has_optional_parameters_(false),
      has_rest_parameter_(false)
    { }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////
  // Abstract base class for CSS selectors.
  /////////////////////////////////////////
  class Selector : public Expression {
    // ADD_PROPERTY(bool, has_reference)
    ADD_PROPERTY(bool, has_placeholder)
    // line break before list separator
    ADD_PROPERTY(bool, has_line_feed)
    // line break after list separator
    ADD_PROPERTY(bool, has_line_break)
    // maybe we have optional flag
    ADD_PROPERTY(bool, is_optional)
    // parent block pointers
    ADD_PROPERTY(Media_Block*, media_block)
  protected:
    size_t hash_;
  public:
    Selector(ParserState pstate, bool r = false, bool h = false)
    : Expression(pstate),
      // has_reference_(r),
      has_placeholder_(h),
      has_line_feed_(false),
      has_line_break_(false),
      is_optional_(false),
      media_block_(0),
      hash_(0)
    { concrete_type(SELECTOR); }
    virtual ~Selector() = 0;
    virtual size_t hash() = 0;
    virtual bool has_parent_ref() {
      return false;
    }
    virtual unsigned long specificity() {
      return Constants::Specificity_Universal;
    }
    virtual void set_media_block(Media_Block* mb) {
      media_block(mb);
    }
  };
  inline Selector::~Selector() { }

  /////////////////////////////////////////////////////////////////////////
  // Interpolated selectors -- the interpolated String will be expanded and
  // re-parsed into a normal selector class.
  /////////////////////////////////////////////////////////////////////////
  class Selector_Schema : public Selector {
    ADD_PROPERTY(String*, contents)
    ADD_PROPERTY(bool, at_root);
  public:
    Selector_Schema(ParserState pstate, String* c)
    : Selector(pstate), contents_(c), at_root_(false)
    { }
    virtual bool has_parent_ref();
    virtual size_t hash() {
      if (hash_ == 0) {
        hash_combine(hash_, contents_->hash());
      }
      return hash_;
    }
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////
  // Abstract base class for simple selectors.
  ////////////////////////////////////////////
  class Simple_Selector : public Selector {
    ADD_PROPERTY(std::string, ns);
    ADD_PROPERTY(std::string, name)
    ADD_PROPERTY(bool, has_ns)
  public:
    Simple_Selector(ParserState pstate, std::string n = "")
    : Selector(pstate), ns_(""), name_(n), has_ns_(false)
    {
      size_t pos = n.find('|');
      // found some namespace
      if (pos != std::string::npos) {
        has_ns_ = true;
        ns_ = n.substr(0, pos);
        name_ = n.substr(pos + 1);
      }
    }
    virtual std::string ns_name() const
    {
      std::string name("");
      if (has_ns_)
        name += ns_ + "|";
      return name + name_;
    }
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_combine(hash_, std::hash<int>()(SELECTOR));
        hash_combine(hash_, std::hash<std::string>()(ns()));
        hash_combine(hash_, std::hash<std::string>()(name()));
      }
      return hash_;
    }
    // namespace query functions
    bool is_universal_ns() const
    {
      return has_ns_ && ns_ == "*";
    }
    bool has_universal_ns() const
    {
      return !has_ns_ || ns_ == "*";
    }
    bool is_empty_ns() const
    {
      return !has_ns_ || ns_ == "";
    }
    bool has_empty_ns() const
    {
      return has_ns_ && ns_ == "";
    }
    bool has_qualified_ns() const
    {
      return has_ns_ && ns_ != "" && ns_ != "*";
    }
    // name query functions
    bool is_universal() const
    {
      return name_ == "*";
    }

    virtual ~Simple_Selector() = 0;
    virtual Compound_Selector* unify_with(Compound_Selector*, Context&);
    virtual bool has_parent_ref() { return false; };
    virtual bool is_pseudo_element() { return false; }
    virtual bool is_pseudo_class() { return false; }

    virtual bool is_superselector_of(Compound_Selector* sub) { return false; }

    bool operator==(const Simple_Selector& rhs) const;
    inline bool operator!=(const Simple_Selector& rhs) const { return !(*this == rhs); }

    bool operator<(const Simple_Selector& rhs) const;
    // default implementation should work for most of the simple selectors (otherwise overload)
    ATTACH_OPERATIONS();
  };
  inline Simple_Selector::~Simple_Selector() { }


  //////////////////////////////////
  // The Parent Selector Expression.
  //////////////////////////////////
  // parent selectors can occur in selectors but also
  // inside strings in declarations (Compound_Selector).
  // only one simple parent selector means the first case.
  class Parent_Selector : public Simple_Selector {
  public:
    Parent_Selector(ParserState pstate)
    : Simple_Selector(pstate, "&")
    { /* has_reference(true); */ }
    virtual bool has_parent_ref() { return true; };
    virtual unsigned long specificity()
    {
      return 0;
    }
    std::string type() { return "selector"; }
    static std::string type_name() { return "selector"; }
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////////////////////////
  // Placeholder selectors (e.g., "%foo") for use in extend-only selectors.
  /////////////////////////////////////////////////////////////////////////
  class Selector_Placeholder : public Simple_Selector {
  public:
    Selector_Placeholder(ParserState pstate, std::string n)
    : Simple_Selector(pstate, n)
    { has_placeholder(true); }
    // virtual Selector_Placeholder* find_placeholder();
    virtual ~Selector_Placeholder() {};
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////////////////////////
  // Type selectors (and the universal selector) -- e.g., div, span, *.
  /////////////////////////////////////////////////////////////////////
  class Type_Selector : public Simple_Selector {
  public:
    Type_Selector(ParserState pstate, std::string n)
    : Simple_Selector(pstate, n)
    { }
    virtual unsigned long specificity()
    {
      // ToDo: What is the specificity of the star selector?
      if (name() == "*") return Constants::Specificity_Universal;
      else               return Constants::Specificity_Type;
    }
    virtual Simple_Selector* unify_with(Simple_Selector*, Context&);
    virtual Compound_Selector* unify_with(Compound_Selector*, Context&);
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////
  // Selector qualifiers -- i.e., classes and ids.
  ////////////////////////////////////////////////
  class Selector_Qualifier : public Simple_Selector {
  public:
    Selector_Qualifier(ParserState pstate, std::string n)
    : Simple_Selector(pstate, n)
    { }
    virtual unsigned long specificity()
    {
      if (name()[0] == '#') return Constants::Specificity_ID;
      if (name()[0] == '.') return Constants::Specificity_Class;
      else                  return Constants::Specificity_Type;
    }
    virtual Compound_Selector* unify_with(Compound_Selector*, Context&);
    ATTACH_OPERATIONS()
  };

  ///////////////////////////////////////////////////
  // Attribute selectors -- e.g., [src*=".jpg"], etc.
  ///////////////////////////////////////////////////
  class Attribute_Selector : public Simple_Selector {
    ADD_PROPERTY(std::string, matcher)
    ADD_PROPERTY(String*, value) // might be interpolated
  public:
    Attribute_Selector(ParserState pstate, std::string n, std::string m, String* v)
    : Simple_Selector(pstate, n), matcher_(m), value_(v)
    { }
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_combine(hash_, Simple_Selector::hash());
        hash_combine(hash_, std::hash<std::string>()(matcher()));
        if (value_) hash_combine(hash_, value_->hash());
      }
      return hash_;
    }
    virtual unsigned long specificity()
    {
      return Constants::Specificity_Attr;
    }
    bool operator==(const Simple_Selector& rhs) const;
    bool operator==(const Attribute_Selector& rhs) const;
    bool operator<(const Simple_Selector& rhs) const;
    bool operator<(const Attribute_Selector& rhs) const;
    ATTACH_OPERATIONS()
  };

  //////////////////////////////////////////////////////////////////
  // Pseudo selectors -- e.g., :first-child, :nth-of-type(...), etc.
  //////////////////////////////////////////////////////////////////
  /* '::' starts a pseudo-element, ':' a pseudo-class */
  /* Except :first-line, :first-letter, :before and :after */
  /* Note that pseudo-elements are restricted to one per selector */
  /* and occur only in the last simple_selector_sequence. */
  inline bool is_pseudo_class_element(const std::string& name)
  {
    return name == ":before"       ||
           name == ":after"        ||
           name == ":first-line"   ||
           name == ":first-letter";
  }

  // Pseudo Selector cannot have any namespace?
  class Pseudo_Selector : public Simple_Selector {
    ADD_PROPERTY(String*, expression)
  public:
    Pseudo_Selector(ParserState pstate, std::string n, String* expr = 0)
    : Simple_Selector(pstate, n), expression_(expr)
    { }

    // A pseudo-class always consists of a "colon" (:) followed by the name
    // of the pseudo-class and optionally by a value between parentheses.
    virtual bool is_pseudo_class()
    {
      return (name_[0] == ':' && name_[1] != ':')
             && ! is_pseudo_class_element(name_);
    }

    // A pseudo-element is made of two colons (::) followed by the name.
    // The `::` notation is introduced by the current document in order to
    // establish a discrimination between pseudo-classes and pseudo-elements.
    // For compatibility with existing style sheets, user agents must also
    // accept the previous one-colon notation for pseudo-elements introduced
    // in CSS levels 1 and 2 (namely, :first-line, :first-letter, :before and
    // :after). This compatibility is not allowed for the new pseudo-elements
    // introduced in this specification.
    virtual bool is_pseudo_element()
    {
      return (name_[0] == ':' && name_[1] == ':')
             || is_pseudo_class_element(name_);
    }
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_combine(hash_, Simple_Selector::hash());
        if (expression_) hash_combine(hash_, expression_->hash());
      }
      return hash_;
    }
    virtual unsigned long specificity()
    {
      if (is_pseudo_element())
        return Constants::Specificity_Type;
      return Constants::Specificity_Pseudo;
    }
    bool operator==(const Simple_Selector& rhs) const;
    bool operator==(const Pseudo_Selector& rhs) const;
    bool operator<(const Simple_Selector& rhs) const;
    bool operator<(const Pseudo_Selector& rhs) const;
    virtual Compound_Selector* unify_with(Compound_Selector*, Context&);
    ATTACH_OPERATIONS()
  };

  /////////////////////////////////////////////////
  // Wrapped selector -- pseudo selector that takes a list of selectors as argument(s) e.g., :not(:first-of-type), :-moz-any(ol p.blah, ul, menu, dir)
  /////////////////////////////////////////////////
  class Wrapped_Selector : public Simple_Selector {
    ADD_PROPERTY(Selector*, selector)
  public:
    Wrapped_Selector(ParserState pstate, std::string n, Selector* sel)
    : Simple_Selector(pstate, n), selector_(sel)
    { }
    virtual bool has_parent_ref() {
      // if (has_reference()) return true;
      if (!selector()) return false;
      return selector()->has_parent_ref();
    }
    virtual bool is_superselector_of(Wrapped_Selector* sub);
    // Selectors inside the negation pseudo-class are counted like any
    // other, but the negation itself does not count as a pseudo-class.
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_combine(hash_, Simple_Selector::hash());
        if (selector_) hash_combine(hash_, selector_->hash());
      }
      return hash_;
    }
    virtual unsigned long specificity()
    {
      return selector_ ? selector_->specificity() : 0;
    }
    bool operator==(const Simple_Selector& rhs) const;
    bool operator==(const Wrapped_Selector& rhs) const;
    bool operator<(const Simple_Selector& rhs) const;
    bool operator<(const Wrapped_Selector& rhs) const;
    ATTACH_OPERATIONS()
  };

  struct Complex_Selector_Pointer_Compare {
    bool operator() (const Complex_Selector* const pLeft, const Complex_Selector* const pRight) const;
  };

  ////////////////////////////////////////////////////////////////////////////
  // Simple selector sequences. Maintains flags indicating whether it contains
  // any parent references or placeholders, to simplify expansion.
  ////////////////////////////////////////////////////////////////////////////
  typedef std::set<Complex_Selector*, Complex_Selector_Pointer_Compare> SourcesSet;
  class Compound_Selector : public Selector, public Vectorized<Simple_Selector*> {
  private:
    SourcesSet sources_;
    ADD_PROPERTY(bool, extended);
    ADD_PROPERTY(bool, has_parent_reference);
  protected:
    void adjust_after_pushing(Simple_Selector* s)
    {
      // if (s->has_reference())   has_reference(true);
      if (s->has_placeholder()) has_placeholder(true);
    }
  public:
    Compound_Selector(ParserState pstate, size_t s = 0)
    : Selector(pstate),
      Vectorized<Simple_Selector*>(s),
      extended_(false),
      has_parent_reference_(false)
    { }
    bool contains_placeholder() {
      for (size_t i = 0, L = length(); i < L; ++i) {
        if ((*this)[i]->has_placeholder()) return true;
      }
      return false;
    };

    bool is_universal() const
    {
      return length() == 1 && (*this)[0]->is_universal();
    }

    Complex_Selector* to_complex(Memory_Manager& mem);
    Compound_Selector* unify_with(Compound_Selector* rhs, Context& ctx);
    // virtual Selector_Placeholder* find_placeholder();
    virtual bool has_parent_ref();
    Simple_Selector* base()
    {
      // Implement non-const in terms of const. Safe to const_cast since this method is non-const
      return const_cast<Simple_Selector*>(static_cast<const Compound_Selector*>(this)->base());
    }
    const Simple_Selector* base() const {
      if (length() == 0) return 0;
      // ToDo: why is this needed?
      if (dynamic_cast<Type_Selector*>((*this)[0]))
        return (*this)[0];
      return 0;
    }
    virtual bool is_superselector_of(Compound_Selector* sub, std::string wrapped = "");
    virtual bool is_superselector_of(Complex_Selector* sub, std::string wrapped = "");
    virtual bool is_superselector_of(Selector_List* sub, std::string wrapped = "");
    virtual size_t hash()
    {
      if (Selector::hash_ == 0) {
        hash_combine(Selector::hash_, std::hash<int>()(SELECTOR));
        if (length()) hash_combine(Selector::hash_, Vectorized::hash());
      }
      return Selector::hash_;
    }
    virtual unsigned long specificity()
    {
      int sum = 0;
      for (size_t i = 0, L = length(); i < L; ++i)
      { sum += (*this)[i]->specificity(); }
      return sum;
    }

    bool is_empty_reference()
    {
      return length() == 1 &&
             dynamic_cast<Parent_Selector*>((*this)[0]);
    }
    std::vector<std::string> to_str_vec(); // sometimes need to convert to a flat "by-value" data structure

    bool operator<(const Compound_Selector& rhs) const;

    bool operator==(const Compound_Selector& rhs) const;
    inline bool operator!=(const Compound_Selector& rhs) const { return !(*this == rhs); }

    SourcesSet& sources() { return sources_; }
    void clearSources() { sources_.clear(); }
    void mergeSources(SourcesSet& sources, Context& ctx);

    Compound_Selector* clone(Context&) const; // does not clone the Simple_Selector*s

    Compound_Selector* minus(Compound_Selector* rhs, Context& ctx);
    ATTACH_OPERATIONS()
  };

  ////////////////////////////////////////////////////////////////////////////
  // General selectors -- i.e., simple sequences combined with one of the four
  // CSS selector combinators (">", "+", "~", and whitespace). Essentially a
  // linked list.
  ////////////////////////////////////////////////////////////////////////////
  class Complex_Selector : public Selector {
  public:
    enum Combinator { ANCESTOR_OF, PARENT_OF, PRECEDES, ADJACENT_TO, REFERENCE };
  private:
    ADD_PROPERTY(Combinator, combinator)
    ADD_PROPERTY(Compound_Selector*, head)
    ADD_PROPERTY(Complex_Selector*, tail)
    ADD_PROPERTY(String*, reference);
  public:
    bool contains_placeholder() {
      if (head() && head()->contains_placeholder()) return true;
      if (tail() && tail()->contains_placeholder()) return true;
      return false;
    };
    Complex_Selector(ParserState pstate,
                     Combinator c = ANCESTOR_OF,
                     Compound_Selector* h = 0,
                     Complex_Selector* t = 0,
                     String* r = 0)
    : Selector(pstate),
      combinator_(c),
      head_(h), tail_(t),
      reference_(r)
    {
      // if ((h && h->has_reference())   || (t && t->has_reference()))   has_reference(true);
      if ((h && h->has_placeholder()) || (t && t->has_placeholder())) has_placeholder(true);
    }
    virtual bool has_parent_ref();

    Complex_Selector* skip_empty_reference()
    {
      if ((!head_ || !head_->length() || head_->is_empty_reference()) &&
          combinator() == Combinator::ANCESTOR_OF)
      {
        if (!tail_) return 0;
        tail_->has_line_feed_ = this->has_line_feed_;
        // tail_->has_line_break_ = this->has_line_break_;
        return tail_->skip_empty_reference();
      }
      return this;
    }

    // can still have a tail
    bool is_empty_ancestor() const
    {
      return (!head() || head()->length() == 0) &&
             combinator() == Combinator::ANCESTOR_OF;
    }

    Complex_Selector* context(Context&);


    // front returns the first real tail
    // skips over parent and empty ones
    const Complex_Selector* first() const;

    // last returns the last real tail
    const Complex_Selector* last() const;

    Selector_List* tails(Context& ctx, Selector_List* tails);

    // unconstant accessors
    Complex_Selector* first();
    Complex_Selector* last();

    // some shortcuts that should be removed
    const Complex_Selector* innermost() const { return last(); };
    Complex_Selector* innermost() { return last(); };

    size_t length() const;
    Selector_List* parentize(Selector_List* parents, Context& ctx);
    virtual bool is_superselector_of(Compound_Selector* sub, std::string wrapping = "");
    virtual bool is_superselector_of(Complex_Selector* sub, std::string wrapping = "");
    virtual bool is_superselector_of(Selector_List* sub, std::string wrapping = "");
    // virtual Selector_Placeholder* find_placeholder();
    Selector_List* unify_with(Complex_Selector* rhs, Context& ctx);
    Combinator clear_innermost();
    void append(Context&, Complex_Selector*);
    void set_innermost(Complex_Selector*, Combinator);
    virtual size_t hash()
    {
      if (hash_ == 0) {
        hash_combine(hash_, std::hash<int>()(SELECTOR));
        hash_combine(hash_, std::hash<int>()(combinator_));
        if (head_) hash_combine(hash_, head_->hash());
        if (tail_) hash_combine(hash_, tail_->hash());
      }
      return hash_;
    }
    virtual unsigned long specificity() const
    {
      int sum = 0;
      if (head()) sum += head()->specificity();
      if (tail()) sum += tail()->specificity();
      return sum;
    }
    virtual void set_media_block(Media_Block* mb) {
      media_block(mb);
      if (tail_) tail_->set_media_block(mb);
      if (head_) head_->set_media_block(mb);
    }
    bool operator<(const Complex_Selector& rhs) const;
    bool operator==(const Complex_Selector& rhs) const;
    inline bool operator!=(const Complex_Selector& rhs) const { return !(*this == rhs); }
    SourcesSet sources()
    {
      //s = Set.new
      //seq.map {|sseq_or_op| s.merge sseq_or_op.sources if sseq_or_op.is_a?(SimpleSequence)}
      //s

      SourcesSet srcs;

      Compound_Selector* pHead = head();
      Complex_Selector*  pTail = tail();

      if (pHead) {
        SourcesSet& headSources = pHead->sources();
        srcs.insert(headSources.begin(), headSources.end());
      }

      if (pTail) {
        SourcesSet tailSources = pTail->sources();
        srcs.insert(tailSources.begin(), tailSources.end());
      }

      return srcs;
    }
    void addSources(SourcesSet& sources, Context& ctx) {
      // members.map! {|m| m.is_a?(SimpleSequence) ? m.with_more_sources(sources) : m}
      Complex_Selector* pIter = this;
      while (pIter) {
        Compound_Selector* pHead = pIter->head();

        if (pHead) {
          pHead->mergeSources(sources, ctx);
        }

        pIter = pIter->tail();
      }
    }
    void clearSources() {
      Complex_Selector* pIter = this;
      while (pIter) {
        Compound_Selector* pHead = pIter->head();

        if (pHead) {
          pHead->clearSources();
        }

        pIter = pIter->tail();
      }
    }
    Complex_Selector* clone(Context&) const;      // does not clone Compound_Selector*s
    Complex_Selector* cloneFully(Context&) const; // clones Compound_Selector*s
    // std::vector<Compound_Selector*> to_vector();
    ATTACH_OPERATIONS()
  };

  typedef std::deque<Complex_Selector*> ComplexSelectorDeque;
  typedef Subset_Map<std::string, std::pair<Complex_Selector*, Compound_Selector*> > ExtensionSubsetMap;

  ///////////////////////////////////
  // Comma-separated selector groups.
  ///////////////////////////////////
  class Selector_List : public Selector, public Vectorized<Complex_Selector*> {
    ADD_PROPERTY(std::vector<std::string>, wspace)
  protected:
    void adjust_after_pushing(Complex_Selector* c);
  public:
    Selector_List(ParserState pstate, size_t s = 0)
    : Selector(pstate), Vectorized<Complex_Selector*>(s), wspace_(0)
    { }
    std::string type() { return "list"; }
    // remove parent selector references
    // basically unwraps parsed selectors
    virtual bool has_parent_ref();
    void remove_parent_selectors();
    // virtual Selector_Placeholder* find_placeholder();
    Selector_List* parentize(Selector_List* parents, Context& ctx);
    virtual bool is_superselector_of(Compound_Selector* sub, std::string wrapping = "");
    virtual bool is_superselector_of(Complex_Selector* sub, std::string wrapping = "");
    virtual bool is_superselector_of(Selector_List* sub, std::string wrapping = "");
    Selector_List* unify_with(Selector_List*, Context&);
    void populate_extends(Selector_List*, Context&, ExtensionSubsetMap&);
    virtual size_t hash()
    {
      if (Selector::hash_ == 0) {
        hash_combine(Selector::hash_, std::hash<int>()(SELECTOR));
        hash_combine(Selector::hash_, Vectorized::hash());
      }
      return Selector::hash_;
    }
    virtual unsigned long specificity()
    {
      unsigned long sum = 0;
      unsigned long specificity = 0;
      for (size_t i = 0, L = length(); i < L; ++i)
      {
        specificity = (*this)[i]->specificity();
        if (sum < specificity) sum = specificity;
      }
      return sum;
    }
    virtual void set_media_block(Media_Block* mb) {
      media_block(mb);
      for (Complex_Selector* cs : elements()) {
        cs->set_media_block(mb);
      }
    }
    Selector_List* clone(Context&) const;      // does not clone Compound_Selector*s
    Selector_List* cloneFully(Context&) const; // clones Compound_Selector*s
    virtual bool operator==(const Selector& rhs) const;
    virtual bool operator==(const Selector_List& rhs) const;
    // Selector Lists can be compared to comma lists
    virtual bool operator==(const Expression& rhs) const;
    ATTACH_OPERATIONS()
  };

  template<typename SelectorType>
  bool selectors_equal(const SelectorType& one, const SelectorType& two, bool simpleSelectorOrderDependent) {
    // Test for equality among selectors while differentiating between checks that demand the underlying Simple_Selector
    // ordering to be the same or not. This works because operator< (which doesn't make a whole lot of sense for selectors, but
    // is required for proper stl collection ordering) is implemented using string comparision. This gives stable sorting
    // behavior, and can be used to determine if the selectors would have exactly idential output. operator== matches the
    // ruby sass implementations for eql, which sometimes perform order independent comparisions (like set comparisons of the
    // members of a SimpleSequence (Compound_Selector)).
    //
    // Due to the reliance on operator== and operater< behavior, this templated method is currently only intended for
    // use with Compound_Selector and Complex_Selector objects.
    if (simpleSelectorOrderDependent) {
      return !(one < two) && !(two < one);
    } else {
      return one == two;
    }
  }

  // compare function for sorting and probably other other uses
  struct cmp_complex_selector { inline bool operator() (const Complex_Selector* l, const Complex_Selector* r) { return (*l < *r); } };
  struct cmp_compound_selector { inline bool operator() (const Compound_Selector* l, const Compound_Selector* r) { return (*l < *r); } };
  struct cmp_simple_selector { inline bool operator() (const Simple_Selector* l, const Simple_Selector* r) { return (*l < *r); } };

}

#ifdef __clang__

#pragma clang diagnostic pop

#endif

#endif
