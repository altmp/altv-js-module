
#include "V8Class.h"
#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/SDK.h"

static void StaticGet(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    alt::IResource* iresource = V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext());
    V8_ARG_TO_STRING(1, key);
    V8_RETURN(V8Helpers::MValueToV8(iresource->GetLocalStorage()->Get(key)));
}

static void StaticSet(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, val);

    resource->GetLocalStorage()->Set(key, val);
}

static void StaticDelete(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    resource->GetLocalStorage()->Delete(key);
}

static void StaticClear(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    resource->GetLocalStorage()->Clear();
}

static void StaticSave(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK(resource->GetLocalStorage()->Save(), "exceeded max local storage size (4MB)");
}

extern V8Class v8LocalStorage("LocalStorage", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticMethod(isolate, tpl, "get", &StaticGet);
    V8Helpers::SetStaticMethod(isolate, tpl, "set", &StaticSet);
    V8Helpers::SetStaticMethod(isolate, tpl, "delete", &StaticDelete);
    V8Helpers::SetStaticMethod(isolate, tpl, "deleteAll", &StaticClear);
    V8Helpers::SetStaticMethod(isolate, tpl, "clear", &StaticClear);
    V8Helpers::SetStaticMethod(isolate, tpl, "save", &StaticSave);
});
