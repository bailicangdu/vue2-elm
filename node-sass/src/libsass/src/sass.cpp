#include "sass.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>

#include "sass.h"
#include "file.hpp"
#include "util.hpp"

extern "C" {
  using namespace Sass;

  // Allocate libsass heap memory
  // Don't forget string termination!
  void* ADDCALL sass_alloc_memory(size_t size)
  {
    void* ptr = malloc(size);
    if (ptr == NULL)
      out_of_memory();
    return ptr;
  }

  char* ADDCALL sass_copy_c_string(const char* str)
  {
    size_t len = strlen(str) + 1;
    char* cpy = (char*) sass_alloc_memory(len);
    std::memcpy(cpy, str, len);
    return cpy;
  }

  // Deallocate libsass heap memory
  void ADDCALL sass_free_memory(void* ptr)
  {
    if (ptr) free (ptr);
  }

  // caller must free the returned memory
  char* ADDCALL sass_string_quote (const char *str, const char quote_mark)
  {
    std::string quoted = quote(str, quote_mark);
    return sass_copy_c_string(quoted.c_str());
  }

  // caller must free the returned memory
  char* ADDCALL sass_string_unquote (const char *str)
  {
    std::string unquoted = unquote(str);
    return sass_copy_c_string(unquoted.c_str());
  }

  // Make sure to free the returned value!
  // Incs array has to be null terminated!
  char* ADDCALL sass_resolve_file (const char* file, const char* paths[])
  {
    std::string resolved(File::find_file(file, paths));
    return sass_copy_c_string(resolved.c_str());
  }

  // Get compiled libsass version
  const char* ADDCALL libsass_version(void)
  {
    return LIBSASS_VERSION;
  }

  // Get compiled libsass version
  const char* ADDCALL libsass_language_version(void)
  {
    return LIBSASS_LANGUAGE_VERSION;
  }

}
