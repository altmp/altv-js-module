#include "stdafx.h"

#include "cpp-sdk/version/version.h"

#include "V8Module.h"
#include "CNodeScriptRuntime.h"

/*static void NodeStop()
{
        {
                //v8::Isolate::Scope isolateScope(isolate);
                //v8::SealHandleScope seal(isolate);
                //v8::platform::PumpMessageLoop(platform, isolate);
                //node::EmitBeforeExit(m_env);

                //if (uv_loop_alive(m_env->event_loop())) {
                //	uv_run(m_env->event_loop(), UV_RUN_NOWAIT);
                //}
        }

        //node::EmitExit(m_env);
        //node::RunAtExit(m_env);
        //node::FreeEnvironment(m_env);

        //isolate->Dispose();
        //v8::V8::Dispose();
}*/

extern V8Module v8Alt;
namespace main
{
    static void Initialize(v8::Local<v8::Object> exports, v8::Local<v8::Value> module, v8::Local<v8::Context> context, void*)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        v8Alt.Register(isolate, context, exports);
    }
    NODE_MODULE_LINKED(alt, Initialize)
}  // namespace main

extern V8Module sharedModule;
namespace shared
{
    static void InitializeShared(v8::Local<v8::Object> exports, v8::Local<v8::Value> module, v8::Local<v8::Context> context, void*)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        sharedModule.Register(isolate, context, exports);
    }
    NODE_MODULE_LINKED(altShared, InitializeShared)
}  // namespace shared

static void CommandHandler(const std::vector<std::string>& args)
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
                     << "Copyright | 2022 altMP team." << Log::Endl;

        Log::Colored << "~ly~js-module: " << JS_MODULE_VERSION << Log::Endl;
        Log::Colored << "~ly~" JS_MODULE_COPYRIGHT << Log::Endl;

        Log::Colored << "~ly~nodejs: " << NODE_MAJOR_VERSION << "." << NODE_MINOR_VERSION << "." << NODE_PATCH_VERSION << Log::Endl;
        Log::Colored << "~ly~" NODEJS_COPYRIGHT << Log::Endl;
    }
    else if(args[0] == "--help")
    {
        Log::Colored << "~y~Usage: ~w~js-module [options]" << Log::Endl;
        Log::Colored << "~y~Options:" << Log::Endl;
        Log::Colored << "  ~ly~--help    ~w~- this message." << Log::Endl;
        Log::Colored << "  ~ly~--version ~w~- version info." << Log::Endl;
    }
}

static void TimersCommand(const std::vector<std::string>&)
{
    auto resources = CNodeScriptRuntime::Instance().GetResources();
    Log::Info << "================ Timer info =================" << Log::Endl;
    for(auto resource : resources)
    {
        resource->TimerBenchmark();
    }
    Log::Info << "======================================================" << Log::Endl;
}

EXPORT bool altMain(alt::ICore* core)
{
    alt::ICore::SetInstance(core);

    auto& runtime = CNodeScriptRuntime::Instance();
    if(!runtime.Init()) return false;

    core->RegisterScriptRuntime("js", &runtime);
    core->SubscribeCommand("js-module", &CommandHandler);
    core->SubscribeCommand("timers", &TimersCommand);

    return true;
}

EXPORT const char* GetSDKHash()
{
    return ALT_SDK_VERSION;
}
