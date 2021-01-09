#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8BindHelpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void IsEntityIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IColShape> cs = _this->GetHandle().As<IColShape>();

	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IEntity> ent = targetEntity->GetHandle().As<IEntity>();
	bool entityIn = cs->IsEntityIn(ent);

	info.GetReturnValue().Set(v8::Boolean::New(isolate, entityIn));
}

static void IsPointIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IColShape);

	V8_CHECK_ARGS_LEN(1);
	V8_CHECK(info[0]->IsObject(), "object expected");

	v8::Local<v8::Object> pos = info[0].As<v8::Object>();

	v8::Local<v8::Number> x = V8::Get(ctx, pos, "x")->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> y = V8::Get(ctx, pos, "y")->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> z = V8::Get(ctx, pos, "z")->ToNumber(ctx).ToLocalChecked();

	V8_RETURN_BOOLEAN(_this->IsPointIn({ x->Value(), y->Value(), z->Value() }));
}

extern V8Class v8WorldObject;
extern V8Class v8Colshape("Colshape", v8WorldObject, nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor<IColShape, IColShape::ColShapeType, &IColShape::GetColshapeType>(isolate, tpl, "colshapeType");
	V8::SetAccessor<IColShape, bool, &IColShape::IsPlayersOnly, &IColShape::SetPlayersOnly>(isolate, tpl, "playersOnly");

	V8::SetMethod(isolate, tpl, "isEntityIn", IsEntityIn);
	V8::SetMethod(isolate, tpl, "isPointIn", IsPointIn);
});

extern V8Class v8ColshapeCylinder("ColshapeCylinder", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK(info.IsConstructCall(), "ColshapeCylinder constructor is not a function");
	V8_CHECK_ARGS_LEN(5);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, radius);
	V8_ARG_TO_NUMBER(5, height);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeCylinder({ x, y, z }, radius, height);

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeCylinder");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeSphere("ColshapeSphere", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK(info.IsConstructCall(), "ColshapeSphere constructor is not a function");
	V8_CHECK_ARGS_LEN(4);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, radius);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeSphere({ x, y, z }, radius);

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeSphere");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeCircle("ColshapeCircle", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK(info.IsConstructCall(), "ColshapeCircle constructor is not a function");
	V8_CHECK_ARGS_LEN(3);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, radius);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeCircle({ x, y, 0 }, radius);

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeCircle");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeCuboid("ColshapeCuboid", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK(info.IsConstructCall(), "ColshapeCuboid constructor is not a function");
	V8_CHECK_ARGS_LEN(6);

	V8_ARG_TO_NUMBER(1, x1);
	V8_ARG_TO_NUMBER(2, y1);
	V8_ARG_TO_NUMBER(3, z1);
	V8_ARG_TO_NUMBER(4, x2);
	V8_ARG_TO_NUMBER(5, y2);
	V8_ARG_TO_NUMBER(6, z2);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeCube({ x1, y1, z1 }, { x2, y2, z2 });

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeCuboid");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeRectangle("ColshapeRectangle", v8Colshape, [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK(info.IsConstructCall(), "ColshapeRectangle constructor is not a function");
	V8_CHECK_ARGS_LEN(4);

	V8_ARG_TO_NUMBER(1, x1);
	V8_ARG_TO_NUMBER(2, y1);
	V8_ARG_TO_NUMBER(3, x2);
	V8_ARG_TO_NUMBER(4, y2);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeRectangle(x1, y1, x2, y2, 0);

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeRectangle");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});
