#include "stdafx.h"

#include "CNodeScriptRuntime.h"

bool CNodeScriptRuntime::Init()
{
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

    auto* tracing_agent = node::CreateAgent();
    // auto* tracing_controller = tracing_agent->GetTracingController();
    node::tracing::TraceEventHelper::SetAgent(tracing_agent);
    platform.reset(node::CreatePlatform(4, node::tracing::TraceEventHelper::GetTracingController()));

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
    platform->CancelPendingDelayedTasks(isolate);
    platform->UnregisterIsolate(isolate);

    isolate->Dispose();
    v8::V8::Dispose();
    platform.release();
#endif

    // node::FreePlatform(platform.release());
}

std::vector<std::string> CNodeScriptRuntime::GetNodeArgs()
{
    std::vector<std::string> args = { "alt-server", "--experimental-modules", "--es-module-specifier-resolution=node", "--trace-warnings" };

    alt::config::Node moduleConfig = alt::ICore::Instance().GetServerConfig()["js-module"];
    if(!moduleConfig.IsDict()) return args;

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

    return args;
}
