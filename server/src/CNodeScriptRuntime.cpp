#include "stdafx.h"

#include "CNodeScriptRuntime.h"
#include "CProfiler.h"

bool CNodeScriptRuntime::Init()
{
    ProcessConfigOptions();
    std::vector<std::string> argv = GetNodeArgs();
    std::vector<std::string> execArgv;
    std::vector<std::string> errors;
    node::InitializeNodeWithArgs(&argv, &execArgv, &errors);
    if(errors.size() > 0)
    {
        for(std::string& error : errors)
        {
            Log::Error << "Error while initializing node: " << error << Log::Endl;
        }
        return false;
    }

    platform.reset(node::CreatePlatform(4, (v8::TracingController*)nullptr));

    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    isolate = v8::Isolate::Allocate();

    platform->RegisterIsolate(isolate, uv_default_loop());

    v8::Isolate::CreateParams params;
    params.array_buffer_allocator = node::CreateArrayBufferAllocator();

    v8::Isolate::Initialize(isolate, params);

    // IsWorker data slot
    isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 1, new bool(false));

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);

        V8Class::LoadAll(isolate);
    }

    return true;
}

alt::IResource::Impl* CNodeScriptRuntime::CreateImpl(alt::IResource* resource)
{
    auto res = new CNodeResourceImpl{ this, isolate, resource };
    resources.insert(res);
    return res;
}

void CNodeScriptRuntime::OnTick()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::SealHandleScope seal(isolate);

    platform->DrainTasks(isolate);
}

void CNodeScriptRuntime::OnDispose()
{
    /*{
            v8::SealHandleScope seal(isolate);

            do {
                    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
                    platform->DrainTasks(isolate);
            } while (uv_loop_alive(uv_default_loop()));
    }*/
#ifdef WIN32
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
#else
    platform->DrainTasks(isolate);
    platform->UnregisterIsolate(isolate);

    isolate->Dispose();
    v8::V8::Dispose();
    platform.release();
#endif

    // node::FreePlatform(platform.release());

    if(CProfiler::Instance().IsEnabled()) CProfiler::Instance().Dump("./");
}

std::vector<std::string> CNodeScriptRuntime::GetNodeArgs()
{
    std::vector<std::string> args = { "alt-server", "--experimental-modules", "--es-module-specifier-resolution=node", "--trace-warnings" };

    alt::config::Node moduleConfig = alt::ICore::Instance().GetServerConfig()["js-module"];
    if(!moduleConfig.IsDict()) return args;

    // https://nodejs.org/api/cli.html#--inspecthostport
    alt::config::Node inspector = moduleConfig["inspector"];
    if(!inspector.IsNone())
    {
        std::string inspectorHost = "127.0.0.1";
        alt::config::Node host = inspector["host"];
        if(host.IsScalar()) inspectorHost = host.ToString();

        std::string inspectorPort = "9229";
        alt::config::Node port = inspector["port"];
        if(port.IsScalar()) inspectorPort = port.ToString();

        args.push_back("--inspect=" + inspectorHost + ":" + inspectorPort);
    }

    // https://nodejs.org/api/cli.html#--enable-source-maps
    alt::config::Node enableSourceMaps = moduleConfig["source-maps"];
    if(!enableSourceMaps.IsNone())
    {
        try
        {
            if(enableSourceMaps.ToBool()) args.push_back("--enable-source-maps");
        }
        catch(alt::config::Error&)
        {
            Log::Error << "Invalid value for 'source-maps' config option" << Log::Endl;
        }
    }

    // https://nodejs.org/api/cli.html#--heap-prof
    alt::config::Node enableHeapProfiler = moduleConfig["heap-profiler"];
    if(!enableHeapProfiler.IsNone())
    {
        try
        {
            if(enableHeapProfiler.ToBool()) args.push_back("--heap-prof");
        }
        catch(alt::config::Error&)
        {
            Log::Error << "Invalid value for 'heap-profiler' config option" << Log::Endl;
        }
    }

    // https://nodejs.org/api/cli.html#--experimental-fetch
    alt::config::Node enableGlobalFetch = moduleConfig["global-fetch"];
    if(!enableGlobalFetch.IsNone())
    {
        try
        {
            if(enableGlobalFetch.ToBool()) args.push_back("--experimental-fetch");
        }
        catch(alt::config::Error&)
        {
            Log::Error << "Invalid value for 'global-fetch' config option" << Log::Endl;
        }
    }

    // https://nodejs.org/api/cli.html#--experimental-global-webcrypto
    alt::config::Node enableGlobalWebcrypto = moduleConfig["global-webcrypto"];
    if(!enableGlobalWebcrypto.IsNone())
    {
        try
        {
            if(enableGlobalWebcrypto.ToBool()) args.push_back("--experimental-global-webcrypto");
        }
        catch(alt::config::Error&)
        {
            Log::Error << "Invalid value for 'global-webcrypto' config option" << Log::Endl;
        }
    }

    // https://nodejs.org/api/cli.html#--experimental-network-imports
    alt::config::Node enableNetworkImports = moduleConfig["network-imports"];
    if(!enableNetworkImports.IsNone())
    {
        try
        {
            if(enableNetworkImports.ToBool()) args.push_back("--experimental-network-imports");
        }
        catch(alt::config::Error&)
        {
            Log::Error << "Invalid value for 'network-imports' config option" << Log::Endl;
        }
    }

    return args;
}

void CNodeScriptRuntime::ProcessConfigOptions()
{
    alt::config::Node moduleConfig = alt::ICore::Instance().GetServerConfig()["js-module"];
    if(!moduleConfig.IsDict()) return;

    alt::config::Node profiler = moduleConfig["profiler"];
    if(!profiler.IsNone())
    {
        try
        {
            bool result = profiler.ToBool();
            CProfiler::Instance().SetIsEnabled(result);
        }
        catch(alt::config::Error&)
        {
            Log::Error << "Invalid value for 'profiler' config option" << Log::Endl;
        }
    }
}
