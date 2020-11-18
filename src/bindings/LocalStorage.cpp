
#include "../helpers/V8Class.h"
#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/SDK.h"

static void StaticGet(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_RETURN(static_cast<CV8ResourceImpl*>(resource)->GetLocalStorage());
}

static void Get(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN(V8Helpers::MValueToV8(resource->GetLocalStorage()->Get(key)));
}

static void Set(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, val);
	
	resource->GetLocalStorage()->Set(key, val);
}

static void Delete(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	resource->GetLocalStorage()->Delete(key);
}

static void Clear(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	resource->GetLocalStorage()->Clear();
}

static void Save(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK(resource->GetLocalStorage()->Save(), "exceeded max local storage size (4MB)");
}

extern V8Class v8LocalStorage("LocalStorage", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();

		V8::SetStaticMethod(isolate, tpl, "get", &StaticGet);

		V8::SetMethod(isolate, tpl, "get", &Get);
		V8::SetMethod(isolate, tpl, "set", &Set);
		V8::SetMethod(isolate, tpl, "delete", &Delete);
		V8::SetMethod(isolate, tpl, "deleteAll", &Clear);
		V8::SetMethod(isolate, tpl, "clear",  &Clear);
		V8::SetMethod(isolate, tpl, "save", &Save);
	});
