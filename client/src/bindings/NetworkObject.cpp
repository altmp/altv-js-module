#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

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
extern V8Class v8NetworkObject("NetworkObject", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);
    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetAlpha>(isolate, tpl, "alpha");
    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetTextureVariation>(isolate, tpl, "textureVariation");
    V8Helpers::SetAccessor<alt::INetworkObject, uint16_t, &alt::INetworkObject::GetLodDistance>(isolate, tpl, "lodDistance");
});
