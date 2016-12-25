#ifndef SASS_AST_DEF_MACROS_H
#define SASS_AST_DEF_MACROS_H

// Helper class to switch a flag and revert once we go out of scope
template <class T>
class LocalOption {
  private:
    T* var; // pointer to original variable
    T orig; // copy of the original option
  public:
    LocalOption(T& var)
    {
      this->var = &var;
      this->orig = var;
    }
    LocalOption(T& var, T orig)
    {
      this->var = &var;
      this->orig = var;
      *(this->var) = orig;
    }
    ~LocalOption() {
      *(this->var) = this->orig;
    }
};

#define LOCAL_FLAG(name,opt) LocalOption<bool> flag_##name(name, opt)

#define ATTACH_OPERATIONS()\
virtual void perform(Operation<void>* op) { (*op)(this); }\
virtual AST_Node* perform(Operation<AST_Node*>* op) { return (*op)(this); }\
virtual Statement* perform(Operation<Statement*>* op) { return (*op)(this); }\
virtual Expression* perform(Operation<Expression*>* op) { return (*op)(this); }\
virtual Selector* perform(Operation<Selector*>* op) { return (*op)(this); }\
virtual std::string perform(Operation<std::string>* op) { return (*op)(this); }\
virtual union Sass_Value* perform(Operation<union Sass_Value*>* op) { return (*op)(this); }\
virtual Value* perform(Operation<Value*>* op) { return (*op)(this); }

#define ADD_PROPERTY(type, name)\
protected:\
  type name##_;\
public:\
  type name() const        { return name##_; }\
  type name(type name##__) { return name##_ = name##__; }\
private:

#define ADD_HASHED(type, name)\
protected:\
  type name##_;\
public:\
  type name() const        { return name##_; }\
  type name(type name##__) { hash_ = 0; return name##_ = name##__; }\
private:

#endif
