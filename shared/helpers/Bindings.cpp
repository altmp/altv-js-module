#include "Bindings.h"
#include "Serialization.h"
#include "V8ResourceImpl.h"

void V8Helpers::RegisterFunc(v8::Local<v8::Object> exports, const std::string& _name, v8::FunctionCallback cb, void* data)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    v8::Local<v8::String> name = V8Helpers::JSValue(_name);

    v8::Local<v8::Function> fn = v8::Function::New(ctx, cb, v8::External::New(isolate, data)).ToLocalChecked();
    fn->SetName(name);

    exports->Set(ctx, name, fn);
}

void V8Helpers::FunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto fn = static_cast<alt::MValueFunction*>(info.Data().As<v8::External>()->Value());

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);

    alt::MValueArgs args;

    for(uint64_t i = 0; i < info.Length(); ++i) args.emplace_back(V8Helpers::V8ToMValue(info[i]));

    alt::MValue res = (*fn)->Call(args);

    info.GetReturnValue().Set(V8Helpers::MValueToV8(res));
}

void V8Helpers::SetAccessor(v8::Local<v8::Template> tpl, v8::Isolate* isolate, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter)
{
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), getter, setter);
}

void V8Helpers::DefineOwnProperty(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> val, const char* name, v8::Local<v8::Value> value, v8::PropertyAttribute attributes)
{
    val->DefineOwnProperty(ctx, v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), value, attributes);
}

void V8Helpers::DefineOwnProperty(
  v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> val, v8::Local<v8::String> name, v8::Local<v8::Value> value, v8::PropertyAttribute attributes)
{
    val->DefineOwnProperty(ctx, name, value, attributes);
}

void V8Helpers::SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter)
{
    tpl->PrototypeTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(),
                                          getter,
                                          setter,
                                          v8::Local<v8::Value>(),
                                          v8::AccessControl::DEFAULT,
                                          setter != nullptr ? v8::PropertyAttribute::None : v8::PropertyAttribute::ReadOnly);
}

void V8Helpers::SetLazyAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorNameGetterCallback getter)
{
    tpl->PrototypeTemplate()->SetLazyDataProperty(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), getter);
}

void V8Helpers::SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback)
{
    tpl->PrototypeTemplate()->Set(isolate, name, v8::FunctionTemplate::New(isolate, callback));
}

void V8Helpers::SetStaticAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter)
{
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), getter, setter);
}

void V8Helpers::SetStaticMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback)
{
    tpl->Set(isolate, name, v8::FunctionTemplate::New(isolate, callback));
}

void V8Helpers::SetFunction(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> target, const char* name, v8::FunctionCallback cb, void* userData)
{
    v8::Local<v8::String> _name = v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked();

    v8::Local<v8::Function> fn = v8::Function::New(ctx, cb, v8::External::New(isolate, userData)).ToLocalChecked();
    fn->SetName(_name);

    target->Set(ctx, _name, fn);
}
