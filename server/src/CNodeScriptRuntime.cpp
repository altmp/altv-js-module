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

    platform = node::MultiIsolatePlatform::Create(4);
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    isolate = node::NewIsolate(node::CreateArrayBufferAllocator(), uv_default_loop(), platform.get());

    // IsWorker data slot
    isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 1, new bool(false));

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);

        V8Class::LoadAll(isolate);
    }

    IRuntimeEventHandler::Start();

    RegisterMetrics();

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

    UpdateMetrics();
}

void CNodeScriptRuntime::OnDispose()
{
    /*{
            v8::SealHandleScope seal(isolate);

            do {
                    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
                    platform->DrainTasks(isolate);
            } while (uv_loop_alive(uv_default_loop()));
    }
    platform->UnregisterIsolate(isolate);
    isolate->Dispose();
    node::FreePlatform(platform.release());
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    */

    if(CProfiler::Instance().IsEnabled()) CProfiler::Instance().Dump("./");
}

std::vector<std::string> CNodeScriptRuntime::GetNodeArgs()
{
    // https://nodejs.org/docs/latest-v17.x/api/cli.html#options
    std::vector<std::string> args = { "alt-server", "--no-warnings" };

    Config::Value::ValuePtr moduleConfig = alt::ICore::Instance().GetServerConfig()["js-module"];
    if(!moduleConfig->IsDict()) return args;

    // https://nodejs.org/api/cli.html#--inspecthostport
    Config::Value::ValuePtr inspector = moduleConfig["inspector"];
    if(!inspector->IsNone())
    {
        std::string inspectorHost = inspector["host"]->AsString("127.0.0.1");
        int inspectorPort = inspector["port"]->AsNumber(9229);
        args.push_back("--inspect=" + inspectorHost + ":" + std::to_string(inspectorPort));
    }

    // https://nodejs.org/api/cli.html#--enable-source-maps
    Config::Value::ValuePtr enableSourceMaps = moduleConfig["source-maps"];
    if(enableSourceMaps->AsBool(false)) args.push_back("--enable-source-maps");

    // https://nodejs.org/api/cli.html#--heap-prof
    Config::Value::ValuePtr enableHeapProfiler = moduleConfig["heap-profiler"];
    if(enableHeapProfiler->AsBool(false)) args.push_back("--heap-prof");

    // https://nodejs.org/api/cli.html#--experimental-global-webcrypto
    Config::Value::ValuePtr enableGlobalWebcrypto = moduleConfig["global-webcrypto"];
    if(enableGlobalWebcrypto->AsBool(false)) args.push_back("--experimental-global-webcrypto");

    // https://nodejs.org/api/cli.html#--experimental-network-imports
    Config::Value::ValuePtr enableNetworkImports = moduleConfig["network-imports"];
    if(enableNetworkImports->AsBool(false)) args.push_back("--experimental-network-imports");

    Config::Value::ValuePtr extraCliArgs = moduleConfig["extra-cli-args"];
    for(auto arg : extraCliArgs->AsList())
    {
        args.push_back(arg->AsString());
    }

    return args;
}

void CNodeScriptRuntime::ProcessConfigOptions()
{
    Config::Value::ValuePtr moduleConfig = alt::ICore::Instance().GetServerConfig()["js-module"];
    if(!moduleConfig->IsDict()) return;

    Config::Value::ValuePtr profiler = moduleConfig["profiler"];
    if(profiler->IsDict())
    {
        CProfiler::Instance().SetIsEnabled(true);
        CProfiler::Instance().SetLogsEnabled(profiler["logs"]->AsBool(false));
    }
}

void CNodeScriptRuntime::RegisterMetrics()
{
    alt::ICore& core = alt::ICore::Instance();
    static auto registerMetric = [&](Metric metric, const char* name, alt::Metric::Type metricType) { metrics[metric] = core.RegisterMetric(name, metricType); };

    registerMetric(Metric::HEAP_SIZE, "node_heap_size", alt::Metric::Type::METRIC_TYPE_GAUGE);
    registerMetric(Metric::HEAP_LIMIT, "node_heap_limit", alt::Metric::Type::METRIC_TYPE_GAUGE);
    registerMetric(Metric::PHYSICAL_SIZE, "node_physical_size", alt::Metric::Type::METRIC_TYPE_GAUGE);
    registerMetric(Metric::PHYSICAL_LIMIT, "node_physical_limit", alt::Metric::Type::METRIC_TYPE_GAUGE);
    registerMetric(Metric::GLOBAL_HANDLES_SIZE, "node_global_handles_size", alt::Metric::Type::METRIC_TYPE_GAUGE);
    registerMetric(Metric::GLOBAL_HANDLES_LIMIT, "node_global_handles_limit", alt::Metric::Type::METRIC_TYPE_GAUGE);
}

void CNodeScriptRuntime::UpdateMetrics()
{
    alt::ICore& core = alt::ICore::Instance();
    static auto updateMetric = [&](Metric metric, uint64_t value) { metrics[metric]->SetValue(value); };

    v8::HeapStatistics heapStats;
    isolate->GetHeapStatistics(&heapStats);

    updateMetric(Metric::HEAP_SIZE, heapStats.used_heap_size());
    updateMetric(Metric::HEAP_LIMIT, heapStats.total_heap_size());
    updateMetric(Metric::PHYSICAL_SIZE, heapStats.total_physical_size());
    updateMetric(Metric::PHYSICAL_LIMIT, heapStats.total_available_size());
    updateMetric(Metric::GLOBAL_HANDLES_SIZE, heapStats.used_global_handles_size());
    updateMetric(Metric::GLOBAL_HANDLES_LIMIT, heapStats.total_global_handles_size());
}
