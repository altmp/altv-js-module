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
    auto resources = static_cast<CV8ScriptRuntime*>(runtime)->GetResources();
    Log::Info << "================ Timer info =================" << Log::Endl;
    for(auto resource : resources)
    {
        Log::Info << resource->GetResource()->GetName() << ": " << resource->GetTimersCount() << " running timers";
    }
    Log::Info << "======================================================" << Log::Endl;
}

ALTV_JS_EXPORT void CreateScriptRuntime(alt::ICore *core)
{
    alt::ICore::SetInstance(core);
    auto runtime = new CV8ScriptRuntime();
    core->RegisterScriptRuntime("js", runtime);

    // Commands
    core->SubscribeCommand("heap", HeapCommand, runtime);
    core->SubscribeCommand("timers", TimersCommand, runtime);
}

ALTV_JS_EXPORT const char* GetType()
{
    return "js";
}

ALTV_JS_EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}
