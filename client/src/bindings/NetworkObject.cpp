#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllNetworkObjects());
}

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(networkObject, alt::INetworkObject);

    std::ostringstream ss;
    ss << "NetworkObject{ id: " << std::to_string(networkObject->GetID()) << "}";

    V8_RETURN_STRING(ss.str());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::NETWORK_OBJECT).size());
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);
    alt::IWorldObject* entity = alt::ICore::Instance().GetWorldObjectByScriptID(scriptGuid);

    if(entity && (entity->GetType() == alt::IEntity::Type::NETWORK_OBJECT))
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::NETWORK_OBJECT, id);

    if(entity)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByRemoteId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT32(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByRemoteID(alt::IBaseObject::Type::NETWORK_OBJECT, id);

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
extern V8Class v8NetworkObject("NetworkObject", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

    V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);
    V8Helpers::SetStaticMethod(isolate, tpl, "getByRemoteID", StaticGetByRemoteId);

    V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetAlpha>(isolate, tpl, "alpha");
    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetTextureVariation>(isolate, tpl, "textureVariation");
    V8Helpers::SetAccessor<alt::INetworkObject, uint16_t, &alt::INetworkObject::GetLodDistance>(isolate, tpl, "lodDistance");
});
