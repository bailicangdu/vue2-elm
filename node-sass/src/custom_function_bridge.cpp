#include <nan.h>
#include <stdexcept>
#include "custom_function_bridge.h"
#include "sass_types/factory.h"
#include "sass_types/value.h"

Sass_Value* CustomFunctionBridge::post_process_return_value(v8::Local<v8::Value> val) const {
  SassTypes::Value *v_;
  if ((v_ = SassTypes::Factory::unwrap(val))) {
    return v_->get_sass_value();
  } else {
    return sass_make_error("A SassValue object was expected.");
  }
}

std::vector<v8::Local<v8::Value>> CustomFunctionBridge::pre_process_args(std::vector<void*> in) const {
  std::vector<v8::Local<v8::Value>> argv = std::vector<v8::Local<v8::Value>>();

  for (void* value : in) {
    argv.push_back(SassTypes::Factory::create(static_cast<Sass_Value*>(value))->get_js_object());
  }

  return argv;
}
