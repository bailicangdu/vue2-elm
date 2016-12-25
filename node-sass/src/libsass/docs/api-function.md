Sass functions are used to define new custom functions callable by Sass code. They are also used to overload debug or error statements. You can also define a fallback function, which is called for every unknown function found in the Sass code. Functions get passed zero or more `Sass_Values` (a `Sass_List` value) and they must also return a `Sass_Value`. Return a `Sass_Error` if you want to signal an error.

## Special signatures

- `*` - Fallback implementation
- `@warn` - Overload warn statements
- `@error` - Overload error statements
- `@debug` - Overload debug statements

Note: The fallback implementation will be given the name of the called function as the first argument, before all the original function arguments. These features are pretty new and should be considered experimental.

### Basic Usage

```C
#include "sass/functions.h"
```

## Sass Function API

```C
// Forward declaration
struct Sass_Compiler;
struct Sass_Function;

// Typedef helpers for custom functions lists
typedef struct Sass_Function (*Sass_Function_Entry);
typedef struct Sass_Function* (*Sass_Function_List);
// Typedef defining function signature and return type
typedef union Sass_Value* (*Sass_Function_Fn)
  (const union Sass_Value*, Sass_Function_Entry cb, struct Sass_Compiler* compiler);

// Creators for sass function list and function descriptors
ADDAPI Sass_Function_List ADDCALL sass_make_function_list (size_t length);
ADDAPI Sass_Function_Entry ADDCALL sass_make_function (const char* signature, Sass_Function_Fn cb, void* cookie);

// Setters and getters for callbacks on function lists
ADDAPI Sass_Function_Entry ADDCALL sass_function_get_list_entry(Sass_Function_List list, size_t pos);
ADDAPI void ADDCALL sass_function_set_list_entry(Sass_Function_List list, size_t pos, Sass_Function_Entry cb);

// Getters for custom function descriptors
ADDAPI const char* ADDCALL sass_function_get_signature (Sass_Function_Entry cb);
ADDAPI Sass_Function_Fn ADDCALL sass_function_get_function (Sass_Function_Entry cb);
ADDAPI void* ADDCALL sass_function_get_cookie (Sass_Function_Entry cb);
```

### More links

- [Sass Function Example](api-function-example.md)
- [Sass Function Internal](api-function-internal.md)

