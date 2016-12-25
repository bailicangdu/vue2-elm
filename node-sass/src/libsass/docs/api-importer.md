By using custom importers, Sass stylesheets can be implemented in any possible way, such as by being loaded via a remote server. Please note: this feature is experimental and is implemented differently than importers in Ruby Sass. Imports must be relative to the parent import context and therefore we need to pass this information to the importer callback. This is currently done by passing the complete import string/path of the previous import context.

## Return Imports

You actually have to return a list of imports, since some importers may want to import multiple files from one import statement (ie. a glob/star importer).  The memory you pass with source and srcmap is taken over by LibSass and freed automatically when the import is done. You are also allowed to return `0` instead of a list, which will tell LibSass to handle the import by itself (as if no custom importer was in use).

```C
struct Sass_Import** rv = sass_make_import_list(1);
rv[0] = sass_make_import(rel, abs, source, srcmap);
```

Every import will then be included in LibSass. You are allowed to only return a file path without any loaded source. This way you can ie. implement rewrite rules for import paths and leave the loading part for LibSass.

### Basic Usage

```C
#include "sass/functions.h"
```

## Sass Importer API

```C
// Forward declaration
struct Sass_Import;

// Forward declaration
struct Sass_C_Import_Descriptor;

// Typedef defining the custom importer callback
typedef struct Sass_C_Import_Descriptor (*Sass_C_Import_Callback);
// Typedef defining the importer c function prototype
typedef struct Sass_Import** (*Sass_C_Import_Fn) (const char* url, const char* prev, void* cookie);

// Creators for custom importer callback (with some additional pointer)
// The pointer is mostly used to store the callback into the actual function
Sass_C_Import_Callback sass_make_importer (Sass_C_Import_Fn, void* cookie);

// Getters for import function descriptors
Sass_C_Import_Fn sass_import_get_function (Sass_C_Import_Callback fn);
void* sass_import_get_cookie (Sass_C_Import_Callback fn);

// Deallocator for associated memory
void sass_delete_importer (Sass_C_Import_Callback fn);

// Creator for sass custom importer return argument list
struct Sass_Import** sass_make_import_list (size_t length);
// Creator for a single import entry returned by the custom importer inside the list
struct Sass_Import* sass_make_import_entry (const char* path, char* source, char* srcmap);
struct Sass_Import* sass_make_import (const char* rel, const char* abs, char* source, char* srcmap);

// set error message to abort import and to print out a message (path from existing object is used in output)
struct Sass_Import* sass_import_set_error(struct Sass_Import* import, const char* message, size_t line, size_t col);

// Setters to insert an entry into the import list (you may also use [] access directly)
// Since we are dealing with pointers they should have a guaranteed and fixed size
void sass_import_set_list_entry (struct Sass_Import** list, size_t idx, struct Sass_Import* entry);
struct Sass_Import* sass_import_get_list_entry (struct Sass_Import** list, size_t idx);

// Getters for import entry
const char* sass_import_get_rel_path (struct Sass_Import*);
const char* sass_import_get_abs_path (struct Sass_Import*);
const char* sass_import_get_source (struct Sass_Import*);
const char* sass_import_get_srcmap (struct Sass_Import*);
// Explicit functions to take ownership of these items
// The property on our struct will be reset to NULL
char* sass_import_take_source (struct Sass_Import*);
char* sass_import_take_srcmap (struct Sass_Import*);

// Getters for import error entries
size_t sass_import_get_error_line (struct Sass_Import*);
size_t sass_import_get_error_column (struct Sass_Import*);
const char* sass_import_get_error_message (struct Sass_Import*);

// Deallocator for associated memory (incl. entries)
void sass_delete_import_list (struct Sass_Import**);
// Just in case we have some stray import structs
void sass_delete_import (struct Sass_Import*);
```

### More links

- [Sass Importer Example](api-importer-example.md)
- [Sass Importer Internal](api-importer-internal.md)

