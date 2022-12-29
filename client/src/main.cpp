#include "cpp-sdk/SDK.h"
#ifdef DYNAMIC_BUILD
    #include "cpp-sdk/version/version.h"
#else
    #define ALT_SDK_VERSION ""
#endif
#include "CV8ScriptRuntime.h"
#include "Log.h"

#ifdef ALTV_JS_SHARED
    #define ALTV_JS_EXPORT extern "C" __declspec(dllexport)
#else
    #define ALTV_JS_EXPORT extern "C"
#endif

static void HeapCommand(const std::vector<std::string>&)
{
    v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
    v8::HeapStatistics heapStats;
    isolate->GetHeapStatistics(&heapStats);
    Log::Info << "================ Heap benchmark info =================" << Log::Endl;
    Log::Info << "  Total heap size: " << CV8ScriptRuntime::CV8ScriptRuntime::FormatBytes(heapStats.total_heap_size()) << Log::Endl;
    Log::Info << "  Total heap executable size: " << CV8ScriptRuntime::FormatBytes(heapStats.total_heap_size_executable()) << Log::Endl;
    Log::Info << "  Total physical size: " << CV8ScriptRuntime::FormatBytes(heapStats.total_physical_size()) << Log::Endl;
    Log::Info << "  Total available size: " << CV8ScriptRuntime::FormatBytes(heapStats.total_available_size()) << Log::Endl;
    Log::Info << "  Used heap size: " << CV8ScriptRuntime::FormatBytes(heapStats.used_heap_size()) << Log::Endl;
    Log::Info << "  Heap size limit: " << CV8ScriptRuntime::FormatBytes(heapStats.heap_size_limit()) << Log::Endl;
    Log::Info << "  Malloced memory: " << CV8ScriptRuntime::FormatBytes(heapStats.malloced_memory()) << Log::Endl;
    Log::Info << "  Peak malloced memory: " << CV8ScriptRuntime::FormatBytes(heapStats.peak_malloced_memory()) << Log::Endl;
    Log::Info << "  External memory: " << CV8ScriptRuntime::FormatBytes(heapStats.external_memory()) << Log::Endl;
    Log::Info << "  Native context count: " << heapStats.number_of_native_contexts() << Log::Endl;
    Log::Info << "  Detached context count: " << heapStats.number_of_detached_contexts() << Log::Endl;
    Log::Info << "  Total global handle size: " << CV8ScriptRuntime::FormatBytes(heapStats.total_global_handles_size()) << Log::Endl;
    Log::Info << "  Used global handle size: " << CV8ScriptRuntime::FormatBytes(heapStats.used_global_handles_size()) << Log::Endl;
    Log::Info << "======================================================" << Log::Endl;
}

static void TimersCommand(const std::vector<std::string>&)
{
    auto resources = CV8ScriptRuntime::Instance().GetResources();
    Log::Info << "================ Timer info =================" << Log::Endl;
    for(auto resource : resources)
    {
        resource->TimerBenchmark();
    }
    Log::Info << "======================================================" << Log::Endl;
}

static void HeapSpacesCommand(const std::vector<std::string>&)
{
    v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
    size_t heapSpacesCount = isolate->NumberOfHeapSpaces();
    Log::Info << "================ Heap spaces info =================" << Log::Endl;
    for(size_t i = 0; i < heapSpacesCount; i++)
    {
        v8::HeapSpaceStatistics space;
        if(!isolate->GetHeapSpaceStatistics(&space, i)) continue;
        Log::Info << "Heap space #" << i << ": " << space.space_name() << Log::Endl;
        Log::Info << "  Size: " << CV8ScriptRuntime::FormatBytes(space.space_size()) << Log::Endl;
        Log::Info << "  Used: " << CV8ScriptRuntime::FormatBytes(space.space_used_size()) << Log::Endl;
        Log::Info << "  Available: " << CV8ScriptRuntime::FormatBytes(space.space_available_size()) << Log::Endl;
        Log::Info << "  Committed: " << CV8ScriptRuntime::FormatBytes(space.physical_space_size()) << Log::Endl;
    }
    Log::Info << "======================================================" << Log::Endl;
}

static void ClientJSCommand(const std::vector<std::string>& args)
{
    if(args.size() == 0)
    {
        Log::Colored << "~y~Usage: ~w~js-module [options]" << Log::Endl;
        Log::Colored << "  Use: ~ly~\"js-module --help\" ~w~for more info" << Log::Endl;
    }
    else if(args[0] == "--version")
    {
        Log::Colored << "~ly~cpp-sdk: #" << ALT_SDK_VERSION << Log::Endl;
        Log::Colored << "~ly~"
                     << "Copyright | 2020 altMP team." << Log::Endl;

        Log::Colored << "~ly~v8: v" << V8_MAJOR_VERSION << "." << V8_MINOR_VERSION << Log::Endl;
        Log::Colored << "~ly~"
                     << "Copyright | 2014 The V8 project authors." << Log::Endl;
    }
    else if(args[0] == "--help")
    {
        Log::Colored << "~y~Usage: ~w~js-module [options]" << Log::Endl;
        Log::Colored << "~y~Options:" << Log::Endl;
        Log::Colored << "  ~ly~--help    ~w~- this message." << Log::Endl;
        Log::Colored << "  ~ly~--version ~w~- version info." << Log::Endl;
    }
}

ALTV_JS_EXPORT alt::IScriptRuntime* CreateScriptRuntime(alt::ICore* core)
{
    alt::ICore::SetInstance(core);

    // Commands
    core->SubscribeCommand("heap", &HeapCommand);
    core->SubscribeCommand("heapspaces", &HeapSpacesCommand);
    core->SubscribeCommand("timers", &TimersCommand);
    core->SubscribeCommand("js-module", &ClientJSCommand);

    CV8ScriptRuntime* runtime = new CV8ScriptRuntime;
    CV8ScriptRuntime::SetInstance(runtime);
    return runtime;
}

ALTV_JS_EXPORT const char* GetType()
{
    return "js";
}

ALTV_JS_EXPORT const char* GetSDKHash()
{
    return ALT_SDK_VERSION;
}
