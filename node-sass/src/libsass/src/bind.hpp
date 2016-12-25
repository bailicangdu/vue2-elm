#ifndef SASS_BIND_H
#define SASS_BIND_H

#include <string>
#include "listize.hpp"
#include "environment.hpp"

namespace Sass {
  class   AST_Node;
  class   Parameters;
  class   Arguments;
  class   Context;
  class   Eval;
  typedef Environment<AST_Node*> Env;

  void bind(std::string type, std::string name, Parameters*, Arguments*, Context*, Env*, Eval*);
}

#endif
