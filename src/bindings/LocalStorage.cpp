
#include "../helpers/V8Class.h"
#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/SDK.h"

static void StaticGet(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	if(info.Length() == 0) 
	{
		V8_RETURN(static_cast<CV8ResourceImpl*>(resource)->GetLocalStorage());

		Log::Warning << "Using the local storage instance methods is deprecated. Use the static methods instead." << Log::Endl;
	}
	else
	{
		alt::IResource *iresource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
		V8_ARG_TO_STRING(1, key);
		V8_RETURN(V8Helpers::MValueToV8(iresource->GetLocalStorage()->Get(key)));
	}
}

static void StaticSet(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, val);
	
	resource->GetLocalStorage()->Set(key, val);
}

static void StaticDelete(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	resource->GetLocalStorage()->Delete(key);
}

static void StaticClear(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	resource->GetLocalStorage()->Clear();
}

static void StaticSave(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK(resource->GetLocalStorage()->Save(), "exceeded max local storage size (4MB)");
}

static void Get(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN(V8Helpers::MValueToV8(resource->GetLocalStorage()->Get(key)));

	Log::Warning << "Using the local storage instance methods is deprecated. Use the static methods instead." << Log::Endl;
}

static void Set(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, val);
	
	resource->GetLocalStorage()->Set(key, val);

	Log::Warning << "Using the local storage instance methods is deprecated. Use the static methods instead." << Log::Endl;
}

static void Delete(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	resource->GetLocalStorage()->Delete(key);

	Log::Warning << "Using the local storage instance methods is deprecated. Use the static methods instead." << Log::Endl;
}

static void Clear(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	resource->GetLocalStorage()->Clear();

	Log::Warning << "Using the local storage instance methods is deprecated. Use the static methods instead." << Log::Endl;
}

static void Save(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_IRESOURCE();

	V8_CHECK(resource->GetLocalStorage()->Save(), "exceeded max local storage size (4MB)");

	Log::Warning << "Using the local storage instance methods is deprecated. Use the static methods instead." << Log::Endl;
}

extern V8Class v8LocalStorage("LocalStorage", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticMethod(isolate, tpl, "get", &StaticGet);
	V8::SetStaticMethod(isolate, tpl, "set", &StaticSet);
	V8::SetStaticMethod(isolate, tpl, "delete", &StaticDelete);
	V8::SetStaticMethod(isolate, tpl, "deleteAll", &StaticClear);
	V8::SetStaticMethod(isolate, tpl, "clear",  &StaticClear);
	V8::SetStaticMethod(isolate, tpl, "save", &StaticSave);

	V8::SetMethod(isolate, tpl, "get", &Get);
	V8::SetMethod(isolate, tpl, "set", &Set);
	V8::SetMethod(isolate, tpl, "delete", &Delete);
	V8::SetMethod(isolate, tpl, "deleteAll", &Clear);
	V8::SetMethod(isolate, tpl, "clear",  &Clear);
	V8::SetMethod(isolate, tpl, "save", &Save);
});
