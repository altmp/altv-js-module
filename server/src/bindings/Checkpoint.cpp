#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"

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

extern V8Class v8Colshape;
extern V8Class v8Checkpoint("Checkpoint", v8Colshape, Constructor, [](v8::Local<v8::FunctionTemplate> tpl) { v8::Isolate* isolate = v8::Isolate::GetCurrent(); });
