#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"
#include "V8FastFunction.h"

namespace V8Helpers
{
    void RegisterFunc(v8::Local<v8::Object> exports, const std::string& _name, v8::FunctionCallback cb, void* data = nullptr);

    void FunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

    void SetAccessor(v8::Local<v8::Template> tpl, v8::Isolate* isolate, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

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

#ifdef ALT_CLIENT
    // Only specify the last parameter when creating a class method
    template<typename FastFunc>
    void SetFastFunction(
      v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> target, const char* name, v8::FunctionCallback slowCb, FastFunc&& fastCb, const char* className = "")
    {
        V8FastFunction* func = V8FastFunction::GetOrCreate(name, className, slowCb, fastCb);
        target->Set(ctx, v8::String::NewFromUtf8(isolate, name), func->GetTemplate(isolate)->GetFunction(ctx).ToLocalChecked());
    }
#endif
}  // namespace V8Helpers
