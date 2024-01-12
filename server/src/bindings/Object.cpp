#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 7);

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

    V8_ARG_TO_UINT_OPT(info.Length() <= 4 ? 4 : 7, streamingDistance, 0);

    IObject* object = alt::ICore::Instance().CreateObject(model, pos, rot, alpha, textureVariation, lodDistance, streamingDistance);
    V8_BIND_BASE_OBJECT(object, "Failed to create object");
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllObjects());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::OBJECT).size());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::OBJECT, id);

    if(entity)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

// clang-format off
extern V8Class v8Entity;
extern V8Class v8Object("Object", v8Entity, Constructor, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);
	V8Helpers::SetStaticMethod(isolate, tpl, "getByID", &StaticGetByID);

    V8Helpers::SetMethod<alt::IObject, &alt::IObject::ActivatePhysics>(isolate, tpl, "activatePhysics");
    V8Helpers::SetMethod<alt::IObject, &alt::IObject::PlaceOnGroundProperly>(isolate, tpl, "placeOnGroundProperly");

    V8Helpers::SetAccessor<alt::IObject, uint8_t, &alt::IObject::GetAlpha, &alt::IObject::SetAlpha>(isolate, tpl, "alpha");
    V8Helpers::SetAccessor<alt::IObject, uint8_t, &alt::IObject::GetTextureVariation, &alt::IObject::SetTextureVariation>(isolate, tpl, "textureVariation");
    V8Helpers::SetAccessor<alt::IObject, uint16_t, &alt::IObject::GetLodDistance, &alt::IObject::SetLodDistance>(isolate, tpl, "lodDistance");
});
