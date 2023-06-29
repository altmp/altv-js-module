#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/ICustomTexture.h"

using namespace alt;

static void StaticRegister(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, path);

    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    auto font = ICore::Instance().RegisterFont(resource->GetResource(), path, origin);

    if(!font)
    {
        V8_RETURN_NULL();
        return;
    }

    V8_RETURN(resource->GetOrCreateEntity(font, "Font")->GetJSVal(isolate));
}

extern V8Class v8BaseObject;
extern V8Class v8Font("Font",
                      v8BaseObject,
                      [](v8::Local<v8::FunctionTemplate> tpl)
                      {
                          v8::Isolate* isolate = v8::Isolate::GetCurrent();

                          V8Helpers::SetStaticMethod(isolate, tpl, "register", StaticRegister);
                      });
