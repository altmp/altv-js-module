#include "cpp-sdk/SDK.h"
#include "CV8ScriptRuntime.h"
#include "helpers/Log.h"

#ifdef ALTV_JS_SHARED
#define ALTV_JS_EXPORT extern "C" __declspec(dllexport)
#else
#define ALTV_JS_EXPORT extern "C"
#endif

void StaticInitialization()
{
    extern V8Class v8Vector3;
}

ALTV_JS_EXPORT alt::IScriptRuntime *CreateJSScriptRuntime(alt::ICore *core)
{
    alt::ICore::SetInstance(core);
    auto ret = new CV8ScriptRuntime();
    return ret;
}

ALTV_JS_EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}