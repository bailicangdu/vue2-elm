#ifndef SASS_VALUES_H
#define SASS_VALUES_H

#include "ast.hpp"

namespace Sass {

  union Sass_Value* ast_node_to_sass_value (const Expression* val);
  Value* sass_value_to_ast_node (Memory_Manager& mem, const union Sass_Value* val);

}
#endif
