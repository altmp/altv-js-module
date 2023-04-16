#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 6);

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

    uint8_t alpha = 255;
    if(info[3]->IsNumber())
    {
        V8_ARG_TO_UINT(4, alphaVal);
        alpha = alphaVal;
    }

    uint8_t textureVariation = 0;
    if(info[4]->IsNumber())
    {
        V8_ARG_TO_UINT(5, textureVariationVal);
        textureVariation = textureVariationVal;
    }

    uint16_t lodDistance = 100;
    if(info[5]->IsNumber())
    {
        V8_ARG_TO_UINT(6, lodDistanceVal);
        lodDistance = lodDistanceVal;
    }

    INetworkObject* object = alt::ICore::Instance().CreateNetworkObject(model, pos, rot, alpha, textureVariation, lodDistance);
    V8_CHECK(object, "Failed to create object");
    resource->BindEntity(info.This(), object);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllNetworkObjects());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetNetworkObjects().size());
}

// clang-format off
extern V8Class v8Entity;
extern V8Class v8NetworkObject("NetworkObject", v8Entity, Constructor, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

    V8Helpers::SetMethod<alt::INetworkObject, &alt::INetworkObject::ActivatePhysics>(isolate, tpl, "activatePhysics");
    V8Helpers::SetMethod<alt::INetworkObject, &alt::INetworkObject::PlaceOnGroundProperly>(isolate, tpl, "placeOnGroundProperly");

    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetAlpha, &alt::INetworkObject::SetAlpha>(isolate, tpl, "alpha");
    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetTextureVariation, &alt::INetworkObject::SetTextureVariation>(isolate, tpl, "textureVariation");
    V8Helpers::SetAccessor<alt::INetworkObject, uint16_t, &alt::INetworkObject::GetLodDistance, &alt::INetworkObject::SetLodDistance>(isolate, tpl, "lodDistance");
});
