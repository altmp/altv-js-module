#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/ILocalObject.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 9);

    uint32_t weaponHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, weaponhash);
        weaponHash = alt::ICore::Instance().Hash(weaponhash);
    }
    else
    {
        V8_ARG_TO_UINT(1, _weaponhash);
        weaponHash = _weaponhash;
    }

    V8_ARG_TO_VECTOR3(2, pos);
    V8_ARG_TO_VECTOR3(3, rot);

    if(info.Length() < 4)
    {
        auto obj = alt::ICore::Instance().CreateWeaponObject(pos, rot, weaponHash, 0, 100, true, 1.f, false, 0, resource->GetResource());
        V8_BIND_BASE_OBJECT(obj, "Failed to create weaponObject");
    }
    else
    {
        uint32_t modelHash;
        if(info[3]->IsString())
        {
            V8_ARG_TO_STRING(4, modelhash);
            modelHash = alt::ICore::Instance().Hash(modelhash);
        }
        else
        {
            V8_ARG_TO_UINT(4, _modelHash);
            modelHash = _modelHash;
        }

        V8_ARG_TO_INT_OPT(5, numAmmo, 100);
        V8_ARG_TO_BOOLEAN_OPT(6, createDefaultComponents, true);
        V8_ARG_TO_NUMBER_OPT(7, scale, 1);
        V8_ARG_TO_BOOLEAN_OPT(8, useStreaming, false);
        V8_ARG_TO_UINT_OPT(9, streamingDistance, 0);

        auto obj =
          alt::ICore::Instance().CreateWeaponObject(pos, rot, weaponHash, modelHash, numAmmo, createDefaultComponents, scale, useStreaming, streamingDistance, resource->GetResource());
        V8_BIND_BASE_OBJECT(obj, "Failed to create weaponObject");
    }
}

static void SetComponentTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_INT(1, componentType);
    V8_ARG_TO_INT(2, tintIndex);

    object->SetComponentTintIndex(componentType, tintIndex);
}

static void GetComponentTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, componentType);

    V8_RETURN_INT(object->GetComponentTintIndex(componentType));
}

static void GiveComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, componentType);

    object->GiveComponent(componentType);
}

static void RemoveComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, componentType);

    object->RemoveComponent(componentType);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_RETURN(resource->GetAllWeaponObjects());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetWeaponObjects().size());
}

extern V8Class v8LocalObject;
extern V8Class v8WeaponObject("WeaponObject",
                              v8LocalObject,
                              Constructor,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  using namespace alt;
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                                  V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

                                  V8Helpers::SetAccessor<ILocalObject, int, &ILocalObject::GetTintIndex, &ILocalObject::SetTintIndex>(isolate, tpl, "tintIndex");
                                  V8Helpers::SetMethod(isolate, tpl, "setComponentTintIndex", &GetComponentTintIndex);
                                  V8Helpers::SetMethod(isolate, tpl, "getComponentTintIndex", &SetComponentTintIndex);
                                  V8Helpers::SetMethod(isolate, tpl, "giveComponent", &GiveComponent);
                                  V8Helpers::SetMethod(isolate, tpl, "removeComponent", &RemoveComponent);
                              });
