`Sass_Values` are used to pass values and their types between the implementer and LibSass. Sass knows various different value types (including nested arrays and hash-maps). If you implement a binding to another programming language, you have to find a way to convert `Sass_Values` between the targeted language and C. `Sass_Values` are currently only used by custom functions.

### Basic Usage

```C
#include "sass/values.h"
```

```C
// Type for Sass values
enum Sass_Tag {
  SASS_BOOLEAN,
  SASS_NUMBER,
  SASS_COLOR,
  SASS_STRING,
  SASS_LIST,
  SASS_MAP,
  SASS_NULL,
  SASS_ERROR,
  SASS_WARNING
};

// Tags for denoting Sass list separators
enum Sass_Separator {
  SASS_COMMA,
  SASS_SPACE
};
```

### Sass Value API

```C
// Forward declaration
union Sass_Value;

// Return the sass tag for a generic sass value
// Check is needed before accessing specific values!
enum Sass_Tag sass_value_get_tag (const union Sass_Value* v);

// Check value to be of a specific type
// Can also be used before accessing properties!
bool sass_value_is_null (const union Sass_Value* v);
bool sass_value_is_number (const union Sass_Value* v);
bool sass_value_is_string (const union Sass_Value* v);
bool sass_value_is_boolean (const union Sass_Value* v);
bool sass_value_is_color (const union Sass_Value* v);
bool sass_value_is_list (const union Sass_Value* v);
bool sass_value_is_map (const union Sass_Value* v);
bool sass_value_is_error (const union Sass_Value* v);
bool sass_value_is_warning (const union Sass_Value* v);

// Getters and setters for Sass_Number
double sass_number_get_value (const union Sass_Value* v);
void sass_number_set_value (union Sass_Value* v, double value);
const char* sass_number_get_unit (const union Sass_Value* v);
void sass_number_set_unit (union Sass_Value* v, char* unit);

// Getters and setters for Sass_String
const char* sass_string_get_value (const union Sass_Value* v);
void sass_string_set_value (union Sass_Value* v, char* value);

// Getters and setters for Sass_Boolean
bool sass_boolean_get_value (const union Sass_Value* v);
void sass_boolean_set_value (union Sass_Value* v, bool value);

// Getters and setters for Sass_Color
double sass_color_get_r (const union Sass_Value* v);
void sass_color_set_r (union Sass_Value* v, double r);
double sass_color_get_g (const union Sass_Value* v);
void sass_color_set_g (union Sass_Value* v, double g);
double sass_color_get_b (const union Sass_Value* v);
void sass_color_set_b (union Sass_Value* v, double b);
double sass_color_get_a (const union Sass_Value* v);
void sass_color_set_a (union Sass_Value* v, double a);

// Getter for the number of items in list
size_t sass_list_get_length (const union Sass_Value* v);
// Getters and setters for Sass_List
enum Sass_Separator sass_list_get_separator (const union Sass_Value* v);
void sass_list_set_separator (union Sass_Value* v, enum Sass_Separator value);
// Getters and setters for Sass_List values
union Sass_Value* sass_list_get_value (const union Sass_Value* v, size_t i);
void sass_list_set_value (union Sass_Value* v, size_t i, union Sass_Value* value);

// Getter for the number of items in map
size_t sass_map_get_length (const union Sass_Value* v);
// Getters and setters for Sass_List keys and values
union Sass_Value* sass_map_get_key (const union Sass_Value* v, size_t i);
void sass_map_set_key (union Sass_Value* v, size_t i, union Sass_Value*);
union Sass_Value* sass_map_get_value (const union Sass_Value* v, size_t i);
void sass_map_set_value (union Sass_Value* v, size_t i, union Sass_Value*);

// Getters and setters for Sass_Error
char* sass_error_get_message (const union Sass_Value* v);
void sass_error_set_message (union Sass_Value* v, char* msg);

// Getters and setters for Sass_Warning
char* sass_warning_get_message (const union Sass_Value* v);
void sass_warning_set_message (union Sass_Value* v, char* msg);

// Creator functions for all value types
union Sass_Value* sass_make_null    (void);
union Sass_Value* sass_make_boolean (bool val);
union Sass_Value* sass_make_string  (const char* val);
union Sass_Value* sass_make_number  (double val, const char* unit);
union Sass_Value* sass_make_color   (double r, double g, double b, double a);
union Sass_Value* sass_make_list    (size_t len, enum Sass_Separator sep);
union Sass_Value* sass_make_map     (size_t len);
union Sass_Value* sass_make_error   (const char* msg);
union Sass_Value* sass_make_warning (const char* msg);

// Generic destructor function for all types
// Will release memory of all associated Sass_Values
// Means we will delete recursively for lists and maps
void sass_delete_value (union Sass_Value* val);

// Make a deep cloned copy of the given sass value
union Sass_Value* sass_clone_value (const union Sass_Value* val);
```

### More links

- [Sass Value Example](api-value-example.md)
- [Sass Value Internal](api-value-internal.md)

