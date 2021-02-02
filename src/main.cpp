#include "cpp-sdk/SDK.h"
#include "CV8ScriptRuntime.h"
#include "helpers/Log.h"

#ifdef ALTV_JS_SHARED
#define ALTV_JS_EXPORT extern "C" __declspec(dllexport)
#else
#define ALTV_JS_EXPORT extern "C"
#endif

static void HeapCommand(alt::Array<alt::StringView>, void* runtime)
{
    static_cast<CV8ScriptRuntime*>(runtime)->HeapBenchmark();
}

static void TimersCommand(alt::Array<alt::StringView>, void* runtime)
{
    // TODO: Add timers command
}

ALTV_JS_EXPORT alt::IScriptRuntime *CreateJSScriptRuntime(alt::ICore *core)
{
    alt::ICore::SetInstance(core);
    auto ret = new CV8ScriptRuntime();

    // Commands
    core->SubscribeCommand("heap", HeapCommand, ret);
    core->SubscribeCommand("timers", TimersCommand, ret);

    return ret;
}

ALTV_JS_EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}