#include "V8Helpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

#include "cpp-sdk/script-objects/ILocalPed.h"
using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(6);

    uint32_t modelHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, modelName);
        modelHash = alt::ICore::Instance().Hash(modelName);
    }
    else
    {
        V8_ARG_TO_UINT(1, _modelHash);
        modelHash = _modelHash;
    }
    V8_ARG_TO_INT32(2, dimension);
    V8_ARG_TO_VECTOR3(3, pos);
    V8_ARG_TO_VECTOR3(4, rot);
    V8_ARG_TO_BOOLEAN_OPT(5, useStreaming, false);
    V8_ARG_TO_UINT_OPT(6, streamingDistance, 0);

    auto ped = alt::ICore::Instance().CreateLocalPed(modelHash, dimension, pos, rot, useStreaming, streamingDistance, resource->GetResource());
    V8_BIND_BASE_OBJECT(ped, "Failed to create localPed");
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::LOCAL_PED, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::LOCAL_PED)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);

    alt::IWorldObject* entity = alt::ICore::Instance().GetWorldObjectByScriptID(scriptGuid);

    if(entity && (entity->GetType() == alt::IWorldObject::Type::LOCAL_PED))
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void ModelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ped, alt::ILocalPed);

    if(val->IsNumber())
    {
        V8_TO_INTEGER(val, model);
        ped->SetModel(model);
    }
    else
    {
        V8_TO_STRING(val, model);
        ped->SetModel(alt::ICore::Instance().Hash(model));
    }
}

static void ModelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ped, alt::ILocalPed);
    V8_RETURN_UINT(ped->GetModel());
}

extern V8Class v8Ped;
extern V8Class v8LocalPed("LocalPed",
                          v8Ped,
                          Constructor,
                          [](v8::Local<v8::FunctionTemplate> tpl)
                          {
                              v8::Isolate* isolate = v8::Isolate::GetCurrent();

                              V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                              V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

                              V8Helpers::SetAccessor(isolate, tpl, "model", &ModelGetter, &ModelSetter);
                              V8Helpers::SetAccessor<ILocalPed, uint32_t, &ILocalPed::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                              V8Helpers::SetAccessor<ILocalPed, bool, &ILocalPed::IsVisible, &ILocalPed::SetVisible>(isolate, tpl, "visible");
                              V8Helpers::SetAccessor<ILocalPed, uint32_t, &ILocalPed::GetScriptID>(isolate, tpl, "scriptID");

                              V8Helpers::SetAccessor<ILocalPed, bool, &ILocalPed::IsStreamedIn>(isolate, tpl, "isStreamedIn");
                          });
