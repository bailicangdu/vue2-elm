#ifndef SASS_TYPES_SASS_VALUE_WRAPPER_H
#define SASS_TYPES_SASS_VALUE_WRAPPER_H

#include <stdexcept>
#include <vector>
#include <nan.h>
#include "value.h"
#include "factory.h"

namespace SassTypes
{
  // Include this in any SassTypes::Value subclasses to handle all the heavy lifting of constructing JS
  // objects and wrapping sass values inside them
  template <class T>
  class SassValueWrapper : public SassTypes::Value {
    public:
      static char const* get_constructor_name() { return "SassValue"; }

      SassValueWrapper(Sass_Value*);
      virtual ~SassValueWrapper();

      Sass_Value* get_sass_value();
      v8::Local<v8::Object> get_js_object();

      static v8::Local<v8::Function> get_constructor();
      static v8::Local<v8::FunctionTemplate> get_constructor_template();
      static NAN_METHOD(New);
      static Sass_Value *fail(const char *, Sass_Value **);

    protected:
      Sass_Value* value;
      static T* unwrap(v8::Local<v8::Object>);

    private:
      static Nan::Persistent<v8::Function> constructor;
      Nan::Persistent<v8::Object> js_object;
  };

  template <class T>
  Nan::Persistent<v8::Function> SassValueWrapper<T>::constructor;

  template <class T>
  SassValueWrapper<T>::SassValueWrapper(Sass_Value* v) {
    this->value = sass_clone_value(v);
  }

  template <class T>
  SassValueWrapper<T>::~SassValueWrapper() {
    this->js_object.Reset();
    sass_delete_value(this->value);
  }

  template <class T>
  Sass_Value* SassValueWrapper<T>::get_sass_value() {
    return sass_clone_value(this->value);
  }

  template <class T>
  v8::Local<v8::Object> SassValueWrapper<T>::get_js_object() {
    if (this->js_object.IsEmpty()) {
      v8::Local<v8::Object> wrapper = Nan::NewInstance(T::get_constructor()).ToLocalChecked();
      delete static_cast<T*>(Nan::GetInternalFieldPointer(wrapper, 0));
      Nan::SetInternalFieldPointer(wrapper, 0, this);
      this->js_object.Reset(wrapper);
    }

    return Nan::New(this->js_object);
  }

  template <class T>
  v8::Local<v8::FunctionTemplate> SassValueWrapper<T>::get_constructor_template() {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>(T::get_constructor_name()).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    T::initPrototype(tpl);

    return scope.Escape(tpl);
  }

  template <class T>
  v8::Local<v8::Function> SassValueWrapper<T>::get_constructor() {
    if (constructor.IsEmpty()) {
      constructor.Reset(Nan::GetFunction(T::get_constructor_template()).ToLocalChecked());
    }

    return Nan::New(constructor);
  }

  template <class T>
  NAN_METHOD(SassValueWrapper<T>::New) {
    std::vector<v8::Local<v8::Value>> localArgs(info.Length());

    for (auto i = 0; i < info.Length(); ++i) {
      localArgs[i] = info[i];
    }
    if (info.IsConstructCall()) {
      Sass_Value* value;
      if (T::construct(localArgs, &value) != NULL) {
        T* obj = new T(value);
        sass_delete_value(value);

        Nan::SetInternalFieldPointer(info.This(), 0, obj);
        obj->js_object.Reset(info.This());
      } else {
        return Nan::ThrowError(Nan::New<v8::String>(sass_error_get_message(value)).ToLocalChecked());
      }
    } else {
      v8::Local<v8::Function> cons = T::get_constructor();
      v8::Local<v8::Object> inst;
      if (Nan::NewInstance(cons, info.Length(), &localArgs[0]).ToLocal(&inst)) {
        info.GetReturnValue().Set(inst);
      } else {
        info.GetReturnValue().Set(Nan::Undefined());
      }
    }
  }

  template <class T>
  T* SassValueWrapper<T>::unwrap(v8::Local<v8::Object> obj) {
    /* This maybe NULL */
    return static_cast<T*>(Factory::unwrap(obj));
  }

  template <class T>
  Sass_Value *SassValueWrapper<T>::fail(const char *reason, Sass_Value **out) {
    *out = sass_make_error(reason);
    return NULL;
  }
}


#endif
