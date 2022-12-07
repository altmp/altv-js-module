#include "stdafx.h"

#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"

using namespace alt;

static void IsEntityIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(_this, IColShape);

    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, entity);
        V8_RETURN_BOOLEAN(_this->IsEntityIdIn(entity));
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, entity, IEntity, "Entity");
        V8_RETURN_BOOLEAN(_this->IsEntityIn(entity));
    }
}

static void IsPointIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(_this, IColShape);
    V8_ARG_TO_VECTOR3(1, point);

    V8_RETURN_BOOLEAN(_this->IsPointIn(point));
}

extern V8Class v8WorldObject;
extern V8Class v8Colshape("Colshape",
                          v8WorldObject,
                          nullptr,
                          [](v8::Local<v8::FunctionTemplate> tpl)
                          {
                              v8::Isolate* isolate = v8::Isolate::GetCurrent();

                              V8Helpers::SetAccessor<IColShape, IColShape::ColShapeType, &IColShape::GetColshapeType>(isolate, tpl, "colshapeType");
                              V8Helpers::SetAccessor<IColShape, bool, &IColShape::IsPlayersOnly, &IColShape::SetPlayersOnly>(isolate, tpl, "playersOnly");

                              V8Helpers::SetMethod(isolate, tpl, "isEntityIn", IsEntityIn);
                              V8Helpers::SetMethod(isolate, tpl, "isPointIn", IsPointIn);
                          });

extern V8Class v8ColshapeCylinder(
  "ColshapeCylinder",
  v8Colshape,
  [](const v8::FunctionCallbackInfo<v8::Value>& info)
  {
      V8_GET_ISOLATE_CONTEXT_RESOURCE();

      V8_CHECK(info.IsConstructCall(), "ColshapeCylinder constructor is not a function");
      V8_CHECK_ARGS_LEN(5);

      V8_ARG_TO_NUMBER(1, x);
      V8_ARG_TO_NUMBER(2, y);
      V8_ARG_TO_NUMBER(3, z);
      V8_ARG_TO_NUMBER(4, radius);
      V8_ARG_TO_NUMBER(5, height);

      IColShape* cs = ICore::Instance().CreateColShapeCylinder({ x, y, z }, radius, height);
      V8_CHECK(cs, "Failed to create ColshapeCylinder");

      resource->BindEntity(info.This(), cs);
  },
  [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeSphere(
  "ColshapeSphere",
  v8Colshape,
  [](const v8::FunctionCallbackInfo<v8::Value>& info)
  {
      V8_GET_ISOLATE_CONTEXT_RESOURCE();

      V8_CHECK(info.IsConstructCall(), "ColshapeSphere constructor is not a function");
      V8_CHECK_ARGS_LEN(4);

      V8_ARG_TO_NUMBER(1, x);
      V8_ARG_TO_NUMBER(2, y);
      V8_ARG_TO_NUMBER(3, z);
      V8_ARG_TO_NUMBER(4, radius);

      IColShape* cs = alt::ICore::Instance().CreateColShapeSphere({ x, y, z }, radius);
      V8_CHECK(cs, "Failed to create ColshapeSphere");

      resource->BindEntity(info.This(), cs);
  },
  [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeCircle(
  "ColshapeCircle",
  v8Colshape,
  [](const v8::FunctionCallbackInfo<v8::Value>& info)
  {
      V8_GET_ISOLATE_CONTEXT_RESOURCE();

      V8_CHECK(info.IsConstructCall(), "ColshapeCircle constructor is not a function");
      V8_CHECK_ARGS_LEN(3);

      V8_ARG_TO_NUMBER(1, x);
      V8_ARG_TO_NUMBER(2, y);
      V8_ARG_TO_NUMBER(3, radius);

      IColShape* cs = alt::ICore::Instance().CreateColShapeCircle({ x, y, 0 }, radius);
      V8_CHECK(cs, "Failed to create ColshapeCircle");

      resource->BindEntity(info.This(), cs);
  },
  [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeCuboid(
  "ColshapeCuboid",
  v8Colshape,
  [](const v8::FunctionCallbackInfo<v8::Value>& info)
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

      IColShape* cs = alt::ICore::Instance().CreateColShapeCube({ x1, y1, z1 }, { x2, y2, z2 });
      V8_CHECK(cs, "Failed to create ColshapeCuboid");

      resource->BindEntity(info.This(), cs);
  },
  [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapeRectangle(
  "ColshapeRectangle",
  v8Colshape,
  [](const v8::FunctionCallbackInfo<v8::Value>& info)
  {
      V8_GET_ISOLATE_CONTEXT_RESOURCE();

      V8_CHECK(info.IsConstructCall(), "ColshapeRectangle constructor is not a function");
      V8_CHECK_ARGS_LEN(4);

      V8_ARG_TO_NUMBER(1, x1);
      V8_ARG_TO_NUMBER(2, y1);
      V8_ARG_TO_NUMBER(3, x2);
      V8_ARG_TO_NUMBER(4, y2);

      IColShape* cs = alt::ICore::Instance().CreateColShapeRectangle(x1, y1, x2, y2, 0);
      V8_CHECK(cs, "Failed to create ColshapeRectangle");

      resource->BindEntity(info.This(), cs);
  },
  [](v8::Local<v8::FunctionTemplate> tpl) {});

extern V8Class v8ColshapePolygon(
  "ColshapePolygon",
  v8Colshape,
  [](const v8::FunctionCallbackInfo<v8::Value>& info)
  {
      V8_GET_ISOLATE_CONTEXT_RESOURCE();
      V8_CHECK_CONSTRUCTOR();
      V8_CHECK_ARGS_LEN(3);

      V8_ARG_TO_NUMBER(1, minZ);
      V8_ARG_TO_NUMBER(2, maxZ);

      V8_CHECK(info[2]->IsArray(), "Argument 3 is not an array");
      v8::Local<v8::Array> arr = info[2].As<v8::Array>();
      std::vector<Vector2f> points;
      uint32_t len = arr->Length();
      for(uint32_t i = 0; i < len; ++i)
      {
          v8::MaybeLocal<v8::Value> maybeVal = arr->Get(ctx, i);
          if(maybeVal.IsEmpty()) continue;
          v8::Local<v8::Value> val = maybeVal.ToLocalChecked();
          V8_TO_VECTOR2(val, point);
          points.push_back(point);
      }

      IColShape* cs = alt::ICore::Instance().CreateColShapePolygon(minZ, maxZ, points);
      V8_CHECK(cs, "Failed to create ColShapePolygon");

      resource->BindEntity(info.This(), cs);
  },
  [](v8::Local<v8::FunctionTemplate> tpl) {});
