#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"

namespace V8Helpers
{
    void RegisterFunc(v8::Local<v8::Object> exports, const std::string& _name, v8::FunctionCallback cb, void* data = nullptr);

    void FunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

    void SetAccessor(v8::Local<v8::Template> tpl, v8::Isolate* isolate, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

}  // namespace V8Helpers

namespace V8
{
    void DefineOwnProperty(v8::Isolate* isolate,
                           v8::Local<v8::Context> ctx,
                           v8::Local<v8::Object> val,
                           const char* name,
                           v8::Local<v8::Value> value,
                           v8::PropertyAttribute attributes = v8::PropertyAttribute::None);

    void DefineOwnProperty(v8::Isolate* isolate,
                           v8::Local<v8::Context> ctx,
                           v8::Local<v8::Object> val,
                           v8::Local<v8::String> name,
                           v8::Local<v8::Value> value,
                           v8::PropertyAttribute attributes = v8::PropertyAttribute::None);

    void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

    void SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback);

    void SetStaticAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

    void SetStaticMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback);

    void SetFunction(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> target, const char* name, v8::FunctionCallback cb, void* userData = nullptr);
}  // namespace V8
