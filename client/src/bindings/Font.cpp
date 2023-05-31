#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/ICustomTexture.h"

using namespace alt;

//static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
//{
//    V8_GET_ISOLATE_CONTEXT();
//
//    auto font = info.This();
//
//    std::ostringstream ss;
//    ss << "Font{ }";
//
//    V8_RETURN_STRING(ss.str());
//}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto& core = alt::ICore::Instance();

    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(3);

    V8_ARG_TO_STRING(1, txd);
    V8_ARG_TO_STRING(2, name);

    std::shared_ptr<v8::BackingStore> buffer;
    if(info[0]->IsArrayBufferView())
    {
        V8_ARG_TO_ARRAY_BUFFER_VIEW(3, arrayBufferView);
        buffer = arrayBufferView->Buffer()->GetBackingStore();
    }
    else
    {
        V8_ARG_TO_ARRAY_BUFFER(3, arrayBuffer);
        buffer = arrayBuffer->GetBackingStore();
    }

    ICustomTexture* texture = core.CreateCustomTextureFromImage(txd, name, (uint8_t*)buffer->Data(), buffer->ByteLength(), resource->GetResource());
    V8_BIND_BASE_OBJECT(texture, "Failed to create CustomTexture");
}

static void StaticRegister(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, path);

    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    auto font = ICore::Instance().RegisterFont(resource->GetResource(), path, origin);

    if (!font)
    {
        return;
    }

    V8_RETURN(resource->GetOrCreateEntity(font.get(), "Font")->GetJSVal(isolate));
}

extern V8Class v8BaseObject;
extern V8Class v8Font("Font",
                         v8BaseObject,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            V8Helpers::SetStaticMethod(isolate, tpl, "register", StaticRegister);
                         });
