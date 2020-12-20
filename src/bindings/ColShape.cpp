#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void IsEntityIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	V8_GET_THIS_BASE_OBJECT(checkpoint, ICheckpoint);
	V8_ARG_TO_BASE_OBJECT(1, entity, IEntity, "Entity");

	V8_RETURN_BOOLEAN(checkpoint->IsEntityIn(entity));
}

static void IsPointIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IColShape);
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_OBJECT(1, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	V8_RETURN_BOOLEAN(_this->IsPointIn({ x, y, z }));
}

static void ColshapeTypeGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IColShape);

	V8_RETURN_INTEGER((int32_t)_this->GetColshapeType());
}

static void PlayersOnlyGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IColShape);

	V8_RETURN_BOOLEAN(_this->IsPlayersOnly());
}

static void PlayersOnlySetter(v8::Local<v8::String> name, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IColShape);
	V8_TO_BOOLEAN(val, playersOnly);

	_this->SetPlayersOnly(playersOnly);
}

extern V8Class v8WorldObject;
extern V8Class v8Colshape("Colshape", v8WorldObject, nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor(isolate, tpl, "colshapeType", ColshapeTypeGetter);
	V8::SetAccessor(isolate, tpl, "playersOnly", PlayersOnlyGetter, PlayersOnlySetter);
	V8::SetMethod(isolate, tpl, "isEntityIn", IsEntityIn);
	V8::SetMethod(isolate, tpl, "isPointIn", IsPointIn);
});

extern V8Class v8ColshapeCylinder("ColshapeCylinder", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(5);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, radius);
	V8_ARG_TO_NUMBER(5, height);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeCylinder({ x, y, z }, radius, height);

	V8_CHECK(cs, "Failed to create ColshapeCylinder");
	resource->BindEntity(info.This(), cs.Get());
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeSphere("ColshapeSphere", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(4);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, radius);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeSphere({ x, y, z }, radius);

	V8_CHECK(cs, "Failed to create ColshapeSphere");
	resource->BindEntity(info.This(), cs.Get());
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeCircle("ColshapeCircle", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(3);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, radius);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeCircle({ x, y, 0 }, radius);

	V8_CHECK(cs, "Failed to create ColshapeCircle");
	resource->BindEntity(info.This(), cs.Get());
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeCuboid("ColshapeCuboid", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(6);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, x2);
	V8_ARG_TO_NUMBER(5, y2);
	V8_ARG_TO_NUMBER(6, z2);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeCube({ x, y, z }, { x2, y2, z2 });

	V8_CHECK(cs, "Failed to create ColshapeCuboid");
	resource->BindEntity(info.This(), cs.Get());
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeRectangle("ColshapeRectangle", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(5);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, x2);
	V8_ARG_TO_NUMBER(4, y2);
	V8_ARG_TO_NUMBER(5, z);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeRectangle(x, y, x2, y2, z);

	V8_CHECK(cs, "Failed to create ColshapeRectangle");
	resource->BindEntity(info.This(), cs.Get());
}, [](v8::Local<v8::FunctionTemplate> tpl) {});
