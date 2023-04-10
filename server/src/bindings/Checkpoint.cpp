#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(5, 10);

    alt::Position pos;
    alt::RGBA color;
    float radius, height;

    V8_ARG_TO_INT(1, type);
    if(info.Length() == 5)
    {
        V8_ARG_TO_VECTOR3(2, posVal);
        V8_ARG_TO_NUMBER(3, radiusVal);
        V8_ARG_TO_NUMBER(4, heightVal);
        V8_ARG_TO_RGBA(5, colorVal);

        pos = posVal;
        color = colorVal;
        radius = radiusVal;
        height = heightVal;
    }
    else
    {
        V8_ARG_TO_NUMBER(2, x);
        V8_ARG_TO_NUMBER(3, y);
        V8_ARG_TO_NUMBER(4, z);
        V8_ARG_TO_NUMBER(5, radiusVal);
        V8_ARG_TO_NUMBER(6, heightVal);
        V8_ARG_TO_INT(7, r);
        V8_ARG_TO_INT(8, g);
        V8_ARG_TO_INT(9, b);
        V8_ARG_TO_INT(10, a);

        pos = { x, y, z };
        color = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
        radius = radiusVal;
        height = heightVal;
    }

    ICheckpoint* cp = ICore::Instance().CreateCheckpoint(type, pos, radius, height, color);

    if(cp) resource->BindEntity(info.This(), cp);
    else
        V8Helpers::Throw(isolate, "Failed to create Checkpoint");
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllCheckpoints());
}

static void HasStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(checkpoint, alt::ICheckpoint);

    V8_RETURN_BOOLEAN(checkpoint->HasStreamSyncedMetaData(key));
}

static void GetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(checkpoint, alt::ICheckpoint);

    V8_RETURN_MVALUE(checkpoint->GetStreamSyncedMetaData(key));
}

static void GetStreamSyncedMetaDataKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(checkpoint, alt::ICheckpoint);

    const std::vector<std::string> list = checkpoint->GetStreamSyncedMetaDataKeys();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

static void SetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, value);

    V8_GET_THIS_BASE_OBJECT(checkpoint, alt::ICheckpoint);

    checkpoint->SetStreamSyncedMetaData(key, value);
}

static void DeleteStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(checkpoint, alt::ICheckpoint);

    checkpoint->DeleteStreamSyncedMetaData(key);
}

extern V8Class v8Colshape;
extern V8Class v8Checkpoint("Checkpoint",
                            v8Colshape,
                            Constructor,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                                V8Helpers::SetAccessor<ICheckpoint, uint32_t, &ICheckpoint::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                                V8Helpers::SetAccessor<ICheckpoint, uint8_t, &ICheckpoint::GetCheckpointType, &ICheckpoint::SetCheckpointType>(isolate, tpl, "checkpointType");
                                V8Helpers::SetAccessor<ICheckpoint, float, &ICheckpoint::GetRadius, &ICheckpoint::SetRadius>(isolate, tpl, "radius");
                                V8Helpers::SetAccessor<ICheckpoint, float, &ICheckpoint::GetHeight, &ICheckpoint::SetHeight>(isolate, tpl, "height");
                                V8Helpers::SetAccessor<ICheckpoint, RGBA, &ICheckpoint::GetColor, &ICheckpoint::SetColor>(isolate, tpl, "color");
                                V8Helpers::SetAccessor<ICheckpoint, Position, &ICheckpoint::GetNextPosition, &ICheckpoint::SetNextPosition>(isolate, tpl, "nextPos");

                                V8Helpers::SetMethod(isolate, tpl, "hasStreamSyncedMeta", HasStreamSyncedMeta);
                                V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMeta", GetStreamSyncedMeta);
                                V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMetaKeys", GetStreamSyncedMetaDataKeys);
                                V8Helpers::SetMethod(isolate, tpl, "setStreamSyncedMeta", SetStreamSyncedMeta);
                                V8Helpers::SetMethod(isolate, tpl, "deleteStreamSyncedMeta", DeleteStreamSyncedMeta);
                            });
