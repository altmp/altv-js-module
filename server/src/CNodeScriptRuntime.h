#pragma once

#include "cpp-sdk/IScriptRuntime.h"

#include "V8Helpers.h"
#include "CNodeResourceImpl.h"

#include "IRuntimeEventHandler.h"

class CNodeScriptRuntime : public alt::IScriptRuntime, public IRuntimeEventHandler
{
    v8::Isolate* isolate;
    V8Helpers::CPersistent<v8::Context> context;
    node::Environment* parentEnv;

    std::unique_ptr<node::MultiIsolatePlatform> platform;
    std::unordered_set<CNodeResourceImpl*> resources;

    enum class Metric : uint8_t
    {
        HEAP_SIZE,
        HEAP_LIMIT,
        PHYSICAL_SIZE,
        PHYSICAL_LIMIT,
        GLOBAL_HANDLES_SIZE,
        GLOBAL_HANDLES_LIMIT,

        SIZE
    };

    std::unordered_map<Metric, alt::Metric*> metrics;

    void RegisterMetrics();
    void UpdateMetrics();

public:
    CNodeScriptRuntime() = default;
    bool Init();

    v8::Isolate* GetIsolate()
    {
        return isolate;
    }

    node::Environment* GetParentEnv() const
    {
        return parentEnv;
    }

    v8::Local<v8::Context> GetContext()
    {
        return context.Get(isolate);
    }

    alt::IResource::Impl* CreateImpl(alt::IResource* resource) override;

    void DestroyImpl(alt::IResource::Impl* impl) override
    {
        auto res = static_cast<CNodeResourceImpl*>(impl);
        resources.erase(res);
        delete res;
    }

    void OnTick() override;
    void OnDispose() override;

    std::vector<std::string> GetNodeArgs();
    void ProcessConfigOptions();

    node::MultiIsolatePlatform* GetPlatform() const
    {
        return platform.get();
    }

    std::unordered_set<CNodeResourceImpl*> GetResources()
    {
        return resources;
    }

    static CNodeScriptRuntime& Instance()
    {
        static CNodeScriptRuntime _Instance;
        return _Instance;
    }
};
