
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"

using namespace alt;

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IWorldObject> obj = _this->GetHandle().As<alt::IWorldObject>();

	alt::Position _pos = obj->GetPosition();
	info.GetReturnValue().Set(resource->CreateVector3(_pos));
}

static void PositionSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(val->IsObject(), "object expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IWorldObject> obj = _this->GetHandle().As<alt::IWorldObject>();

	v8::Local<v8::Object> pos = val.As<v8::Object>();

	v8::Local<v8::Number> x = V8::Get(ctx, pos, "x")->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> y = V8::Get(ctx, pos, "y")->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> z = V8::Get(ctx, pos, "z")->ToNumber(ctx).ToLocalChecked();

	obj->SetPosition({float(x->Value()), float(y->Value()), float(z->Value())});
}

#ifdef ALT_SERVER_API
static void DimensionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IWorldObject> obj = _this->GetHandle().As<alt::IWorldObject>();

	info.GetReturnValue().Set(v8::Integer::New(isolate, obj->GetDimension()));
}

static void DimensionSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IWorldObject> obj = _this->GetHandle().As<alt::IWorldObject>();

	obj->SetDimension(val->ToInteger(isolate)->Value());
}
#endif // ALT_SERVER_API

static V8Class v8worldObject("WorldObject", "BaseObject", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor(isolate, tpl, "pos", PositionGetter, PositionSetter);

#ifdef ALT_SERVER_API
	V8::SetAccessor(isolate, tpl, "dimension", DimensionGetter, DimensionSetter);
#endif // ALT_SERVER_API
});