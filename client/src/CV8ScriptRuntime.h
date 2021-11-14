#pragma once

#include "v8.h"
#include "v8-inspector.h"
#include "libplatform/libplatform.h"

#include "cpp-sdk/IScriptRuntime.h"
#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

#include "CV8Resource.h"

#include "v8-profiler.h"

class CV8ScriptRuntime : public alt::IScriptRuntime
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

    std::unordered_map<uint16_t, alt::Ref<alt::IPlayer>> streamedInPlayers;
    std::unordered_map<uint16_t, alt::Ref<alt::IVehicle>> streamedInVehicles;

public:
    CV8ScriptRuntime();

    static CV8ScriptRuntime& Instance()
    {
        static CV8ScriptRuntime instance;
        return instance;
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

    void HeapBenchmark()
    {
        v8::HeapStatistics heapStats;
        isolate->GetHeapStatistics(&heapStats);
        Log::Info << "================ Heap benchmark info =================" << Log::Endl;
        Log::Info << "total_heap_size = " << FormatBytes(heapStats.total_heap_size()) << Log::Endl;
        Log::Info << "total_heap_size_executable = " << FormatBytes(heapStats.total_heap_size_executable()) << Log::Endl;
        Log::Info << "total_physical_size = " << FormatBytes(heapStats.total_physical_size()) << Log::Endl;
        Log::Info << "total_available_size = " << FormatBytes(heapStats.total_available_size()) << Log::Endl;
        Log::Info << "used_heap_size = " << FormatBytes(heapStats.used_heap_size()) << Log::Endl;
        Log::Info << "heap_size_limit = " << FormatBytes(heapStats.heap_size_limit()) << Log::Endl;
        Log::Info << "malloced_memory = " << FormatBytes(heapStats.malloced_memory()) << Log::Endl;
        Log::Info << "external_memory = " << FormatBytes(heapStats.external_memory()) << Log::Endl;
        Log::Info << "peak_malloced_memory = " << FormatBytes(heapStats.peak_malloced_memory()) << Log::Endl;
        Log::Info << "number_of_native_contexts = " << heapStats.number_of_native_contexts() << Log::Endl;
        Log::Info << "number_of_detached_contexts = " << heapStats.number_of_detached_contexts() << Log::Endl;
        Log::Info << "======================================================" << Log::Endl;
    }

    ~CV8ScriptRuntime()
    {
        while(isolate->IsInUse()) isolate->Exit();
        isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete create_params.array_buffer_allocator;
    }

    static v8::MaybeLocal<v8::Module> ResolveModule(v8::Local<v8::Context> ctx, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> importAssertions, v8::Local<v8::Module> referrer);

    static std::string FormatBytes(uint64_t bytes)
    {
        static std::string result = "";
        const char* sizes[5] = { "bytes", "KB", "MB", "GB", "TB" };

        if(bytes == 0)
        {
            result = "0 bytes";
            return result;
        }
        else if(bytes == 1)
        {
            result = "1 byte";
            return result;
        }

        uint64_t left = std::floor(std::log(bytes) / std::log(1024));

        if(left == 0)
        {
            std::stringstream ss;
            ss << bytes + " bytes";
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

    void OnEntityStreamIn(alt::Ref<alt::IEntity> entity);
    void OnEntityStreamOut(alt::Ref<alt::IEntity> entity);

    auto GetStreamedInPlayers()
    {
        return streamedInPlayers;
    }
    auto GetStreamedInVehicles()
    {
        return streamedInVehicles;
    }
};
