#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

// clang-format off
extern V8Class v8Entity;
extern V8Class v8NetworkObject("NetworkObject", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetAlpha>(isolate, tpl, "alpha");
    V8Helpers::SetAccessor<alt::INetworkObject, uint8_t, &alt::INetworkObject::GetTextureVariation>(isolate, tpl, "textureVariation");
    V8Helpers::SetAccessor<alt::INetworkObject, uint16_t, &alt::INetworkObject::GetLodDistance>(isolate, tpl, "lodDistance");
});
