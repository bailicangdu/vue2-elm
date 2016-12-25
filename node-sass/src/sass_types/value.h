#ifndef SASS_TYPES_VALUE_H
#define SASS_TYPES_VALUE_H

#include <nan.h>
#include <sass/values.h>

namespace SassTypes
{
  // This is the interface that all sass values must comply with
  class Value {
    public:
      virtual Sass_Value* get_sass_value() =0;
      virtual v8::Local<v8::Object> get_js_object() =0;
  };
}

#endif
