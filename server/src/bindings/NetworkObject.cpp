#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(3);

    uint32_t model;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, modelStr);
        model = alt::ICore::Instance().Hash(modelStr);
    }
    else if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, modelHash);
        model = modelHash;
    }

    V8_ARG_TO_VECTOR3(2, pos);
    V8_ARG_TO_VECTOR3(3, rot);

    INetworkObject* object = alt::ICore::Instance().CreateNetworkObject(model, pos, rot);
    V8_CHECK(object, "Failed to create object");
    resource->BindEntity(info.This(), object);
}

// clang-format off
extern V8Class v8Entity;
extern V8Class v8NetworkObject("NetworkObject", v8Entity, Constructor, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
});
