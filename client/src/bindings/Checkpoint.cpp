#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/ICheckpoint.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();

    if(info.Length() == 7)
    {
        V8_ARG_TO_INT(1, type);
        V8_ARG_TO_OBJECT(2, pos);
        V8_ARG_TO_OBJECT(3, nextPos);
        V8_ARG_TO_NUMBER(4, radius);
        V8_ARG_TO_NUMBER(5, height);
        V8_ARG_TO_OBJECT(6, color);
        V8_ARG_TO_UINT(7, streamingDistance)

        V8_OBJECT_GET_NUMBER(pos, "x", x);
        V8_OBJECT_GET_NUMBER(pos, "y", y);
        V8_OBJECT_GET_NUMBER(pos, "z", z);

        V8_OBJECT_GET_NUMBER(nextPos, "x", x2);
        V8_OBJECT_GET_NUMBER(nextPos, "y", y2);
        V8_OBJECT_GET_NUMBER(nextPos, "z", z2);

        V8_OBJECT_GET_INT(color, "r", r);
        V8_OBJECT_GET_INT(color, "g", g);
        V8_OBJECT_GET_INT(color, "b", b);
        V8_OBJECT_GET_INT(color, "a", a);

        alt::ICheckpoint* cp = alt::ICore::Instance().CreateCheckpoint(
          type, { x, y, z }, { x2, y2, z2 }, radius, height, { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a }, streamingDistance, resource->GetResource());
        V8_BIND_BASE_OBJECT(cp, "Failed to create Checkpoint");
    }
    else if(info.Length() == 11)
    {
        V8_ARG_TO_INT(1, type);
        V8_ARG_TO_NUMBER(2, x);
        V8_ARG_TO_NUMBER(3, y);
        V8_ARG_TO_NUMBER(4, z);
        V8_ARG_TO_NUMBER(5, x2);
        V8_ARG_TO_NUMBER(6, y2);
        V8_ARG_TO_NUMBER(7, z2);
        V8_ARG_TO_NUMBER(8, radius);
        V8_ARG_TO_NUMBER(9, height);
        V8_ARG_TO_OBJECT(10, color);
        V8_ARG_TO_UINT(11, streamingDistance)

        V8_OBJECT_GET_INT(color, "r", r);
        V8_OBJECT_GET_INT(color, "g", g);
        V8_OBJECT_GET_INT(color, "b", b);
        V8_OBJECT_GET_INT(color, "a", a);

        alt::ICheckpoint* cp = alt::ICore::Instance().CreateCheckpoint(
          type, { x, y, z }, { x2, y2, z2 }, radius, height, { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a }, streamingDistance, resource->GetResource());
        V8_BIND_BASE_OBJECT(cp, "Failed to create Checkpoint");
    }
    else
        V8Helpers::Throw(isolate, "6 or 10 arguments expected");
}

static void IsEntityIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(cp, alt::ICheckpoint);

    V8_ARG_TO_BASE_OBJECT(1, ent, alt::IEntity, "IEntity");

    V8_RETURN_BOOLEAN(cp->IsEntityIn(ent));
}

static void IsPointIn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(cp, alt::ICheckpoint);

    V8_ARG_TO_OBJECT(1, pos);
    V8_OBJECT_GET_NUMBER(pos, "x", x);
    V8_OBJECT_GET_NUMBER(pos, "y", y);
    V8_OBJECT_GET_NUMBER(pos, "z", z);

    V8_RETURN_BOOLEAN(cp->IsPointIn({ x, y, z }));
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllCheckpoints());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetCheckpoints().size());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::CHECKPOINT, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::CHECKPOINT)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

extern V8Class v8WorldObject;
extern V8Class v8Checkpoint("Checkpoint",
                            v8WorldObject,
                            Constructor,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                using namespace alt;
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                                V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);
                                V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

                                V8Helpers::SetAccessor<ICheckpoint, uint8_t, &ICheckpoint::GetCheckpointType, &ICheckpoint::SetCheckpointType>(isolate, tpl, "checkpointType");
                                V8Helpers::SetAccessor<ICheckpoint, float, &ICheckpoint::GetRadius, &ICheckpoint::SetRadius>(isolate, tpl, "radius");
                                V8Helpers::SetAccessor<ICheckpoint, float, &ICheckpoint::GetHeight, &ICheckpoint::SetHeight>(isolate, tpl, "height");
                                V8Helpers::SetAccessor<ICheckpoint, RGBA, &ICheckpoint::GetColor, &ICheckpoint::SetColor>(isolate, tpl, "color");
                                V8Helpers::SetAccessor<ICheckpoint, Position, &ICheckpoint::GetNextPosition, &ICheckpoint::SetNextPosition>(isolate, tpl, "nextPos");
                                V8Helpers::SetAccessor<ICheckpoint, uint32_t, &ICheckpoint::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                                V8Helpers::SetAccessor<ICheckpoint, bool, &ICheckpoint::IsStreamedIn>(isolate, tpl, "isStreamedIn");

                                V8Helpers::SetAccessor<ICheckpoint, bool, &ICheckpoint::IsVisible, &ICheckpoint::SetVisible>(isolate, tpl, "visible");
                                V8Helpers::SetMethod(isolate, tpl, "isEntityIn", &IsEntityIn);
                                V8Helpers::SetMethod(isolate, tpl, "isPointIn", &IsPointIn);
                            });
