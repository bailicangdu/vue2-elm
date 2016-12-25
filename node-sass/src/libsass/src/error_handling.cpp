#include "sass.hpp"
#include "ast.hpp"
#include "prelexer.hpp"
#include "backtrace.hpp"
#include "error_handling.hpp"

#include <iostream>

namespace Sass {

  namespace Exception {

    Base::Base(ParserState pstate, std::string msg, std::vector<Sass_Import_Entry>* import_stack)
    : std::runtime_error(msg), msg(msg),
      prefix("Error"), pstate(pstate),
      import_stack(import_stack)
    { }

    InvalidSass::InvalidSass(ParserState pstate, std::string msg)
    : Base(pstate, msg)
    { }


    InvalidParent::InvalidParent(Selector* parent, Selector* selector)
    : Base(selector->pstate()), parent(parent), selector(selector)
    {
      msg = "Invalid parent selector for \"";
      msg += selector->to_string(Sass_Inspect_Options());
      msg += "\": \"";
      msg += parent->to_string(Sass_Inspect_Options());
      msg += "\"";
    }

    InvalidArgumentType::InvalidArgumentType(ParserState pstate, std::string fn, std::string arg, std::string type, const Value* value)
    : Base(pstate), fn(fn), arg(arg), type(type), value(value)
    {
      msg  = arg + ": \"";
      msg += value->to_string(Sass_Inspect_Options());
      msg += "\" is not a " + type;
      msg += " for `" + fn + "'";
    }

    InvalidSyntax::InvalidSyntax(ParserState pstate, std::string msg, std::vector<Sass_Import_Entry>* import_stack)
    : Base(pstate, msg, import_stack)
    { }

    UndefinedOperation::UndefinedOperation(const Expression* lhs, const Expression* rhs, const std::string& op)
    : lhs(lhs), rhs(rhs), op(op)
    {
      msg  = def_op_msg + ": \"";
      msg += lhs->to_string({ NESTED, 5 });
      msg += " " + op + " ";
      msg += rhs->to_string({ TO_SASS, 5 });
      msg += "\".";
    }

    InvalidNullOperation::InvalidNullOperation(const Expression* lhs, const Expression* rhs, const std::string& op)
    : UndefinedOperation(lhs, rhs, op)
    {
      msg  = def_op_null_msg + ": \"";
      msg += lhs->inspect();
      msg += " " + op + " ";
      msg += rhs->inspect();
      msg += "\".";
    }

    ZeroDivisionError::ZeroDivisionError(const Expression& lhs, const Expression& rhs)
    : lhs(lhs), rhs(rhs)
    {
      msg  = "divided by 0";
    }

    DuplicateKeyError::DuplicateKeyError(const Map& dup, const Expression& org)
    : Base(org.pstate()), dup(dup), org(org)
    {
      msg  = "Duplicate key ";
      dup.get_duplicate_key()->is_delayed(false);
      msg += dup.get_duplicate_key()->inspect();
      msg += " in map (";
      msg += org.inspect();
      msg += ").";
    }

    TypeMismatch::TypeMismatch(const Expression& var, const std::string type)
    : Base(var.pstate()), var(var), type(type)
    {
      msg  = var.to_string();
      msg += " is not an ";
      msg += type;
      msg += ".";
    }

    InvalidValue::InvalidValue(const Expression& val)
    : Base(val.pstate()), val(val)
    {
      msg  = val.to_string();
      msg += " isn't a valid CSS value.";
    }

    IncompatibleUnits::IncompatibleUnits(const Number& lhs, const Number& rhs)
    : lhs(lhs), rhs(rhs)
    {
      msg  = "Incompatible units: '";
      msg += rhs.unit();
      msg += "' and '";
      msg += lhs.unit();
      msg += "'.";
    }

    AlphaChannelsNotEqual::AlphaChannelsNotEqual(const Expression* lhs, const Expression* rhs, const std::string& op)
    : lhs(lhs), rhs(rhs), op(op)
    {
      msg  = "Alpha channels must be equal: ";
      msg += lhs->to_string({ NESTED, 5 });
      msg += " " + op + " ";
      msg += rhs->to_string({ NESTED, 5 });
      msg += ".";
    }


    SassValueError::SassValueError(ParserState pstate, OperationError& err)
    : Base(pstate, err.what())
    {
      msg = err.what();
      prefix = err.errtype();
    }

  }


  void warn(std::string msg, ParserState pstate)
  {
    std::cerr << "Warning: " << msg<< std::endl;
  }

  void warn(std::string msg, ParserState pstate, Backtrace* bt)
  {
    Backtrace top(bt, pstate, "");
    msg += top.to_string();
    warn(msg, pstate);
  }

  void deprecated_function(std::string msg, ParserState pstate)
  {
    std::string cwd(Sass::File::get_cwd());
    std::string abs_path(Sass::File::rel2abs(pstate.path, cwd, cwd));
    std::string rel_path(Sass::File::abs2rel(pstate.path, cwd, cwd));
    std::string output_path(Sass::File::path_for_console(rel_path, abs_path, pstate.path));

    std::cerr << "DEPRECATION WARNING: " << msg << std::endl;
    std::cerr << "will be an error in future versions of Sass." << std::endl;
    std::cerr << "        on line " << pstate.line+1 << " of " << output_path << std::endl;
  }

  void deprecated(std::string msg, std::string msg2, ParserState pstate)
  {
    std::string cwd(Sass::File::get_cwd());
    std::string abs_path(Sass::File::rel2abs(pstate.path, cwd, cwd));
    std::string rel_path(Sass::File::abs2rel(pstate.path, cwd, cwd));
    std::string output_path(Sass::File::path_for_console(rel_path, pstate.path, pstate.path));

    std::cerr << "DEPRECATION WARNING on line " << pstate.line + 1;
    if (output_path.length()) std::cerr << " of " << output_path;
    std::cerr << ":" << std::endl;
    std::cerr << msg << " and will be an error in future versions of Sass." << std::endl;
    if (msg2.length()) std::cerr << msg2 << std::endl;
    std::cerr << std::endl;
  }

  void deprecated_bind(std::string msg, ParserState pstate)
  {
    std::string cwd(Sass::File::get_cwd());
    std::string abs_path(Sass::File::rel2abs(pstate.path, cwd, cwd));
    std::string rel_path(Sass::File::abs2rel(pstate.path, cwd, cwd));
    std::string output_path(Sass::File::path_for_console(rel_path, abs_path, pstate.path));

    std::cerr << "WARNING: " << msg << std::endl;
    std::cerr << "        on line " << pstate.line+1 << " of " << output_path << std::endl;
    std::cerr << "This will be an error in future versions of Sass." << std::endl;
  }

  void error(std::string msg, ParserState pstate)
  {
    throw Exception::InvalidSyntax(pstate, msg);
  }

  void error(std::string msg, ParserState pstate, Backtrace* bt)
  {
    Backtrace top(bt, pstate, "");
    msg += "\n" + top.to_string();
    error(msg, pstate);
  }

}
