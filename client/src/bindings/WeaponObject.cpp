#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/IObject.h"

static void SetComponentTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_INT(1, componentType);
    V8_ARG_TO_INT(2, tintIndex);

    object->SetComponentTintIndex(componentType, tintIndex);
}

static void GetComponentTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, componentType);

    V8_RETURN_INT(object->GetComponentTintIndex(componentType));
}

static void GiveComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, componentType);

    object->GiveComponent(componentType);
}

static void RemoveComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, componentType);

    object->RemoveComponent(componentType);
}

extern V8Class v8Object;
extern V8Class v8WeaponObject("WeaponObject",
                              v8Object,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  using namespace alt;
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  V8Helpers::SetAccessor<IObject, bool, &IObject::IsWeaponObject>(isolate, tpl, "isWeaponObject");

                                  V8Helpers::SetAccessor<IObject, int, &IObject::GetTintIndex, &IObject::SetTintIndex>(isolate, tpl, "tintIndex");
                                  V8Helpers::SetMethod(isolate, tpl, "setComponentTintIndex", &GetComponentTintIndex);
                                  V8Helpers::SetMethod(isolate, tpl, "getComponentTintIndex", &SetComponentTintIndex);
                                  V8Helpers::SetMethod(isolate, tpl, "giveComponent", &GiveComponent);
                                  V8Helpers::SetMethod(isolate, tpl, "removeComponent", &RemoveComponent);
                              });
