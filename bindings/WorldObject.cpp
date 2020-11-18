
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"

using namespace alt;

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_BASE_OBJECT(obj, alt::IWorldObject);

	V8_RETURN(resource->CreateVector3(obj->GetPosition()));
}

static void PositionSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_BASE_OBJECT(obj, alt::IWorldObject);

	V8_TO_OBJECT(val, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	obj->SetPosition({ x, y, z });
}

#ifdef ALT_SERVER_API
static void DimensionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(obj, alt::IWorldObject);

	V8_RETURN_INTEGER(obj->GetDimension());
}

static void DimensionSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(obj, alt::IWorldObject);

	V8_TO_INTEGER(val, dim);

	obj->SetDimension(dim);
}
#endif // ALT_SERVER_API

extern V8Class v8BaseObject;
extern V8Class v8WorldObject("WorldObject", v8BaseObject, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor(isolate, tpl, "pos", PositionGetter, PositionSetter);

#ifdef ALT_SERVER_API
	V8::SetAccessor(isolate, tpl, "dimension", DimensionGetter, DimensionSetter);
#endif // ALT_SERVER_API
});