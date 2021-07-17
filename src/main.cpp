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
        size_t total, everyTick, interval, timeout;
        resource->GetTimersCount(&total, &everyTick, &interval, &timeout);
        Log::Info << resource->GetResource()->GetName() << ": " << total << " running timers (" 
                  << everyTick << " EveryTick, " << interval << " Interval, " << timeout << " Timeout"
                  << ")" << Log::Endl;
    }
    Log::Info << "======================================================" << Log::Endl;
}

static void ClientJSCommand(alt::Array<alt::StringView> args, void*)
{
    if (args.GetSize() > 0 && args[0] == "--version")
	{
		Log::Colored << "~ly~cpp-sdk: v" << alt::ICore::SDK_VERSION << Log::Endl;
		Log::Colored << "~ly~" << u8"Copyright © 2020 altMP team." << Log::Endl;

		Log::Colored << "~ly~v8: v" << V8_MAJOR_VERSION << "." << V8_MINOR_VERSION << Log::Endl;
		Log::Colored << "~ly~" << u8"Copyright © 2014 The V8 project authors." << Log::Endl;
	}
	else if (args.GetSize() > 0 && args[0] == "--help")
	{
		Log::Colored << "~y~Usage: ~w~js-module [options]" << Log::Endl;
		Log::Colored << "~y~Options:" << Log::Endl;
		Log::Colored << "  ~ly~--help    ~w~- this message." << Log::Endl;
		Log::Colored << "  ~ly~--version ~w~- version info." << Log::Endl;
	}
	else
	{
		Log::Colored << "~y~Usage: ~w~js-module [options]" << Log::Endl;
		Log::Colored << "  Use: ~ly~\"js-module --help\" ~w~for more info" << Log::Endl;
	}
}

ALTV_JS_EXPORT void CreateScriptRuntime(alt::ICore *core)
{
    alt::ICore::SetInstance(core);
    auto& runtime = CV8ScriptRuntime::Instance();
    core->RegisterScriptRuntime("js", &runtime);

    // Commands
    core->SubscribeCommand("heap", &HeapCommand, &runtime);
    core->SubscribeCommand("timers", &TimersCommand, &runtime);
    core->SubscribeCommand("js-module", &ClientJSCommand);
}

ALTV_JS_EXPORT const char* GetType()
{
    return "js";
}

ALTV_JS_EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}
