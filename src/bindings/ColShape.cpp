#include "stdafx.h"

#include "helpers/V8Helpers.h"
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
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 3, "3 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IColShape> cs = _this->GetHandle().As<IColShape>();

	v8::Local<v8::Number> x = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[2]->ToNumber(isolate);

	alt::Position pos(x->Value(), y->Value(), z->Value());
	bool pointIn = cs->IsPointIn(pos);

	info.GetReturnValue().Set(v8::Boolean::New(isolate, pointIn));
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

static V8Class v8Colshape("Colshape", "WorldObject", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor(isolate, tpl, "colshapeType", ColshapeTypeGetter);
	V8::SetAccessor(isolate, tpl, "playersOnly", PlayersOnlyGetter, PlayersOnlySetter);
	V8::SetMethod(isolate, tpl, "isEntityIn", IsEntityIn);
	V8::SetMethod(isolate, tpl, "isPointIn", IsPointIn);
});

static V8Class v8ColshapeCylinder("ColshapeCylinder", "Colshape", [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.IsConstructCall(), "ColshapeCylinder constructor is not a function");

	V8_CHECK(info.Length() == 5, "5 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Number> x = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> radius = info[3]->ToNumber(isolate);
	v8::Local<v8::Number> height = info[4]->ToNumber(isolate);

	Ref<IColShape> cs = ICore::Instance().CreateColShapeCylinder({ x->Value(), y->Value(), z->Value() }, radius->Value(), height->Value());

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeCylinder");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

static V8Class v8ColshapeSphere("ColshapeSphere", "Colshape", [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.IsConstructCall(), "ColshapeSphere constructor is not a function");
	V8_CHECK(info.Length() == 4, "4 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Number> x = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> radius = info[3]->ToNumber(isolate);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeSphere({ x->Value(), y->Value(), z->Value() }, radius->Value());

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeSphere");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

static V8Class v8ColshapeCircle("ColshapeCircle", "Colshape", [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.IsConstructCall(), "ColshapeCircle constructor is not a function");
	V8_CHECK(info.Length() == 3, "3 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Number> x = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> radius = info[2]->ToNumber(isolate);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeCircle({ x->Value(), y->Value(), 0 }, radius->Value());

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeCircle");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

static V8Class v8ColshapeCuboid("ColshapeCuboid", "Colshape", [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.IsConstructCall(), "ColshapeCuboid constructor is not a function");
	V8_CHECK(info.Length() == 6, "6 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Number> x1 = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y1 = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> z1 = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> x2 = info[3]->ToNumber(isolate);
	v8::Local<v8::Number> y2 = info[4]->ToNumber(isolate);
	v8::Local<v8::Number> z2 = info[5]->ToNumber(isolate);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeCube({ x1->Value(), y1->Value(), z1->Value() }, { x2->Value(), y2->Value(), z2->Value() });

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeCuboid");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});

static V8Class v8ColshapeRectangle("ColshapeRectangle", "Colshape", [](const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.IsConstructCall(), "ColshapeRectangle constructor is not a function");
	V8_CHECK(info.Length() == 4, "4 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Number> x1 = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y1 = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> x2 = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> y2 = info[3]->ToNumber(isolate);

	Ref<IColShape> cs = alt::ICore::Instance().CreateColShapeRectangle(x1->Value(), y1->Value(), x2->Value(), y2->Value(), 0);

	if (cs)
		resource->BindEntity(info.This(), cs.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create ColshapeRectangle");
}, [](v8::Local<v8::FunctionTemplate> tpl) {});
