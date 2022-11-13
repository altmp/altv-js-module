#pragma once

#include "v8.h"
#include "v8-inspector.h"
#include "libplatform/libplatform.h"
#include "v8-profiler.h"

#include "cpp-sdk/IScriptRuntime.h"
#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

#include "CV8Resource.h"
#include "IRuntimeEventHandler.h"

class CV8ScriptRuntime : public alt::IScriptRuntime, public IRuntimeEventHandler
{
    static constexpr char inspectorViewStr[] = "alt:V Multiplayer";

    std::unordered_set<CV8ResourceImpl*> resources;
    std::unique_ptr<v8::Platform> platform;
    v8::Isolate::CreateParams create_params;
    v8::Isolate* isolate;
    std::unique_ptr<v8_inspector::V8InspectorClient> inspectorClient;
    std::unique_ptr<v8_inspector::V8Inspector::Channel> inspectorChannel;
    std::unique_ptr<v8_inspector::V8Inspector> inspector;
    std::unique_ptr<v8_inspector::V8InspectorSession> inspectorSession;
    v8::CpuProfiler* profiler;
    uint32_t profilerSamplingInterval = 100;

    std::unordered_map<uint16_t, alt::IPlayer*> streamedInPlayers;
    std::unordered_map<uint16_t, alt::IVehicle*> streamedInVehicles;

    uint32_t activeWorkers = 0;

    static CV8ScriptRuntime*& _instance()
    {
        static CV8ScriptRuntime* instance = nullptr;
        return instance;
    }

public:
    CV8ScriptRuntime();

    void ProcessConfigOptions();

    void OnDispose() override;

    void Init() override;

    static void SetInstance(CV8ScriptRuntime* runtime)
    {
        _instance() = runtime;
    }

    static CV8ScriptRuntime& Instance()
    {
        return *_instance();
    }

    v8::Isolate* GetIsolate() const
    {
        return isolate;
    }

    v8_inspector::V8Inspector* GetInspector() const
    {
        return inspector.get();
    }

    v8::CpuProfiler* GetProfiler()
    {
        return profiler;
    }
    uint32_t GetProfilerSamplingInterval()
    {
        return profilerSamplingInterval;
    }
    void SetProfilerSamplingInterval(uint32_t interval)
    {
        profilerSamplingInterval = interval;
        profiler->SetSamplingInterval(interval);
    }

    uint32_t GetActiveWorkerCount() const
    {
        return activeWorkers;
    }
    void AddActiveWorker()
    {
        activeWorkers++;
    }
    void RemoveActiveWorker()
    {
        activeWorkers--;
    }

    v8::Platform* GetPlatform()
    {
        return platform.get();
    }

    alt::IResource::Impl* CreateImpl(alt::IResource* resource) override
    {
        auto res = new CV8ResourceImpl(resource, isolate);
        resources.insert(res);
        return res;
    }

    void DestroyImpl(alt::IResource::Impl* impl) override
    {
        auto res = static_cast<CV8ResourceImpl*>(impl);
        resources.erase(res);
        delete res;
    }

    void OnTick() override
    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);

        v8::platform::PumpMessageLoop(platform.get(), isolate);
    }

    std::unordered_set<CV8ResourceImpl*> GetResources()
    {
        return resources;
    }

    static v8::MaybeLocal<v8::Module> ResolveModule(v8::Local<v8::Context> ctx, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> importAssertions, v8::Local<v8::Module> referrer);

    static std::string FormatBytes(uint64_t bytes)
    {
        static std::string result = "";
        const char* sizes[5] = { "B", "KB", "MB", "GB", "TB" };

        if(bytes == 0)
        {
            result = "0 B";
            return result;
        }
        else if(bytes == 1)
        {
            result = "1 B";
            return result;
        }
        // todo: idk why it breaks with 80 but it apparently does, fix it properly
        else if(bytes == 80)
        {
            result = "80 B";
            return result;
        }

        uint64_t left = std::floor(std::log(bytes) / std::log(1024));

        if(left == 0)
        {
            std::stringstream ss;
            ss << bytes + " B";
            result = ss.str();
            return result;
        }
        else
        {
            std::stringstream ss;
            ss << std::setprecision(2) << std::fixed << (bytes / std::pow(1024, left)) << " " << sizes[left];
            result = ss.str();
            return result;
        }
    };

    // using DynamicImportReadyCallback = void (*)(v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier, const void* promise);

    // class DynamicImportReadyResult
    // {
    // public:
    // 	DynamicImportReadyResult(v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier, const void* promise, DynamicImportReadyCallback cb)
    // 	{
    // 		_referrer = referrer;
    // 		_specifier = specifier;
    // 		_promise = promise;
    // 		_callback = cb;
    // 	}

    // 	v8::Local<v8::ScriptOrModule> _referrer;
    // 	v8::Local<v8::String> _specifier;
    // 	DynamicImportReadyCallback _callback;
    // 	const void* _promise;

    // 	void call()
    // 	{
    // 		_callback(_referrer, _specifier, _promise);
    // 	}
    // };

    // std::list<DynamicImportReadyResult> onDynamicImportReadyCallbacks;
    // void OnDynamicImportReady(DynamicImportReadyResult result)
    // {
    // 	if (_allResourcesLoaded) result.call();
    // 	else onDynamicImportReadyCallbacks.emplace_back(result);
    // };

    bool resourcesLoaded = false;

    void OnEntityStreamIn(alt::IEntity* entity);
    void OnEntityStreamOut(alt::IEntity* entity);

    auto GetStreamedInPlayers()
    {
        return streamedInPlayers;
    }
    auto GetStreamedInVehicles()
    {
        return streamedInVehicles;
    }
};
