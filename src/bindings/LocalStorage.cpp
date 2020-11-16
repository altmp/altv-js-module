
#include "../helpers/V8Class.h"
#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/SDK.h"

static void StaticGet(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	info.GetReturnValue().Set(static_cast<CV8ResourceImpl *>(resource)->GetLocalStorage());
}

static void Get(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	alt::MValueConst val = resource->GetLocalStorage()->Get(key);

	info.GetReturnValue().Set(V8Helpers::MValueToV8(val));
}

static void Set(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 2, "2 args expected");

	alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	alt::MValue val = V8Helpers::V8ToMValue(info[1]);

	resource->GetLocalStorage()->Set(key, val);
}

static void Delete(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	resource->GetLocalStorage()->Delete(key);
}

static void Clear(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	resource->GetLocalStorage()->Clear();
}

static void Save(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	V8_CHECK(resource->GetLocalStorage()->Save(), "exceeded max local storage size (4MB)");
}

extern V8Class v8LocalStorage(
	"LocalStorage", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();

		tpl->Set(isolate, "get", v8::FunctionTemplate::New(isolate, &StaticGet));

		v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

		proto->Set(isolate, "get", v8::FunctionTemplate::New(isolate, &Get));
		proto->Set(isolate, "set", v8::FunctionTemplate::New(isolate, &Set));
		proto->Set(isolate, "delete", v8::FunctionTemplate::New(isolate, &Delete));
		proto->Set(isolate, "deleteAll", v8::FunctionTemplate::New(isolate, &Clear));
		proto->Set(isolate, "clear", v8::FunctionTemplate::New(isolate, &Clear));
		proto->Set(isolate, "save", v8::FunctionTemplate::New(isolate, &Save));
	});
