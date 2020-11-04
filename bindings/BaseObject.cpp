
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"

static void TypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IBaseObject> obj = _this->GetHandle();
	uint32_t type = static_cast<uint32_t>(obj->GetType());

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(info.GetIsolate(), type));
}

static void ValidGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	info.GetReturnValue().Set(v8::Boolean::New(isolate, _this != nullptr));
}

static void HasMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IBaseObject> obj = _this->GetHandle();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, obj->HasMetaData(*v8::String::Utf8Value(isolate, key))));
}

static void GetMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IBaseObject> obj = _this->GetHandle();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();

	alt::MValueConst val = obj->GetMetaData(*v8::String::Utf8Value(isolate, key));
	info.GetReturnValue().Set(V8Helpers::MValueToV8(val));
}

static void SetMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IBaseObject> obj = _this->GetHandle();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	v8::Local<v8::Value> value = info[1];

	obj->SetMetaData(*v8::String::Utf8Value(isolate, key), V8Helpers::V8ToMValue(value));
}

static void DeleteMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IBaseObject> obj = _this->GetHandle();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();

	obj->DeleteMetaData(*v8::String::Utf8Value(isolate, key));
}

static void Destroy(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::ICore::Instance().DestroyBaseObject(_this->GetHandle());
}

extern V8Class v8BaseObject("BaseObject", [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor(isolate, tpl, "type", &TypeGetter);
	V8::SetAccessor(isolate, tpl, "valid", &ValidGetter);

	V8::SetMethod(isolate, tpl, "hasMeta", HasMeta);
	V8::SetMethod(isolate, tpl, "getMeta", GetMeta);
	V8::SetMethod(isolate, tpl, "setMeta", SetMeta);
	V8::SetMethod(isolate, tpl, "deleteMeta", DeleteMeta);
	V8::SetMethod(isolate, tpl, "destroy", Destroy);
});
