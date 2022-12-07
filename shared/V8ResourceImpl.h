#pragma once

#include <chrono>
#include <filesystem>

#include "cpp-sdk/types/MValue.h"
#include "cpp-sdk/IResource.h"
#include "cpp-sdk/objects/IBaseObject.h"

#include "V8Entity.h"
#include "V8Timer.h"

#include "IRuntimeEventHandler.h"

class V8ResourceImpl : public alt::IResource::Impl
{
public:
    class FunctionImpl : public alt::IMValueFunction::Impl
    {
    public:
        FunctionImpl(V8ResourceImpl* _resource, v8::Local<v8::Function> fn) : resource(_resource), function(_resource->GetIsolate(), fn) {}

        alt::MValue Call(alt::MValueArgs args) const override;

    private:
        V8ResourceImpl* resource;
        v8::Global<v8::Function> function;
    };

    V8ResourceImpl(v8::Isolate* _isolate, alt::IResource* _resource) : isolate(_isolate), resource(_resource) {}

    bool Start() override;
    bool Stop() override;

    void OnTick() override;

    inline alt::IResource* GetResource()
    {
        return resource;
    }
    inline v8::Isolate* GetIsolate()
    {
        return isolate;
    }
    inline v8::Local<v8::Context> GetContext()
    {
        return context.Get(isolate);
    }

    void SubscribeLocal(const std::string& ev, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location, bool once = false)
    {
        alt::CEvent::Type type = V8Helpers::EventHandler::GetTypeForEventName(ev);
        if(type != alt::CEvent::Type::NONE) IRuntimeEventHandler::Instance().EventHandlerAdded(type);
        localHandlers.insert({ ev, V8Helpers::EventCallback{ isolate, cb, std::move(location), once } });
    }

    void SubscribeRemote(const std::string& ev, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location, bool once = false)
    {
        remoteHandlers.insert({ ev, V8Helpers::EventCallback{ isolate, cb, std::move(location), once } });
    }

    void SubscribeGenericLocal(v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location, bool once = false)
    {
        localGenericHandlers.push_back(V8Helpers::EventCallback{ isolate, cb, std::move(location), once });
    }

    void SubscribeGenericRemote(v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location, bool once = false)
    {
        remoteGenericHandlers.push_back(V8Helpers::EventCallback{ isolate, cb, std::move(location), once });
    }

    void UnsubscribeLocal(const std::string& ev, v8::Local<v8::Function> cb)
    {
        auto range = localHandlers.equal_range(ev);

        for(auto it = range.first; it != range.second; ++it)
        {
            if(it->second.fn.Get(isolate)->StrictEquals(cb)) it->second.removed = true;
        }
        alt::CEvent::Type type = V8Helpers::EventHandler::GetTypeForEventName(ev);
        if(type != alt::CEvent::Type::NONE) IRuntimeEventHandler::Instance().EventHandlerRemoved(type);
    }

    void UnsubscribeRemote(const std::string& ev, v8::Local<v8::Function> cb)
    {
        auto range = remoteHandlers.equal_range(ev);

        for(auto it = range.first; it != range.second; ++it)
        {
            if(it->second.fn.Get(isolate)->StrictEquals(cb)) it->second.removed = true;
        }
    }

    void UnsubscribeGenericLocal(v8::Local<v8::Function> cb)
    {
        for(auto& it : localGenericHandlers)
        {
            if(it.fn.Get(isolate)->StrictEquals(cb)) it.removed = true;
        }
    }

    void UnsubscribeGenericRemote(v8::Local<v8::Function> cb)
    {
        for(auto& it : remoteGenericHandlers)
        {
            if(it.fn.Get(isolate)->StrictEquals(cb)) it.removed = true;
        }
    }

    void DispatchStartEvent(bool error)
    {
        std::vector<v8::Local<v8::Value>> args;
        args.push_back(V8Helpers::JSValue(error));

        InvokeEventHandlers(nullptr, GetLocalHandlers("resourceStart"), args, true);
    }

    void DispatchStopEvent()
    {
        std::vector<v8::Local<v8::Value>> args;
        InvokeEventHandlers(nullptr, GetLocalHandlers("resourceStop"), args, true);
    }

    void DispatchErrorEvent(const std::string& errorMsg, const std::string& file, int32_t line, const std::string& stackTrace)
    {
        std::vector<v8::Local<v8::Value>> args = { v8::Exception::Error(V8Helpers::JSValue(errorMsg)), V8Helpers::JSValue(file), V8Helpers::JSValue(line), V8Helpers::JSValue(stackTrace) };
        InvokeEventHandlers(nullptr, GetLocalHandlers("resourceError"), args);
    }

    V8Entity* GetEntity(alt::IBaseObject* handle)
    {
        auto it = entities.find(handle);

        if(it == entities.end()) return nullptr;

        return it->second;
    }

    V8Entity* CreateEntity(alt::IBaseObject* handle)
    {
        V8Class* _class = V8Entity::GetClass(handle);
        if(!_class)
        {
            Log::Error << "Failed to create entity: Type " << (int)handle->GetType() << " has no class" << Log::Endl;
            return nullptr;
        }

        V8Entity* ent = new V8Entity(GetContext(), _class, _class->CreateInstance(GetContext()), handle);
        entities.insert({ handle, ent });
        return ent;
    }

    void BindEntity(v8::Local<v8::Object> val, alt::IBaseObject* handle);

    V8Entity* GetOrCreateEntity(alt::IBaseObject* handle, const char* className = "")
    {
        if(!handle) Log::Error << __FUNCTION__ << " received invalid handle please contact developers if you see this" << Log::Endl;

        V8Entity* ent = GetEntity(handle);

        if(!ent) ent = CreateEntity(handle);

        return ent;
    }

    v8::Local<v8::Value> GetBaseObjectOrNull(alt::IBaseObject* handle);

    template<class T>
    v8::Local<v8::Value> GetBaseObjectOrNull(const T*& handle)
    {
        return GetBaseObjectOrNull(handle.Get());
    }

    v8::Local<v8::Value> CreateVector3(alt::Vector3f vec);
    v8::Local<v8::Value> CreateVector2(alt::Vector2f vec);
    v8::Local<v8::Value> CreateRGBA(alt::RGBA rgba);

    bool IsVector3(v8::Local<v8::Value> val);
    bool IsVector2(v8::Local<v8::Value> val);
    bool IsRGBA(v8::Local<v8::Value> val);
    bool IsBaseObject(v8::Local<v8::Value> val);

    void OnCreateBaseObject(alt::IBaseObject* handle) override;
    void OnRemoveBaseObject(alt::IBaseObject* handle) override;

    alt::MValue GetFunction(v8::Local<v8::Value> val)
    {
        FunctionImpl* impl = new FunctionImpl{ this, val.As<v8::Function>() };
        return alt::ICore::Instance().CreateMValueFunction(impl);
    }

    uint32_t CreateTimer(v8::Local<v8::Context> context, v8::Local<v8::Function> callback, uint32_t interval, bool once, V8Helpers::SourceLocation&& location)
    {
        uint32_t id = ++nextTimerId;
        timers[id] = new V8Timer{ isolate, context, GetTime(), callback, interval, once, std::move(location) };

        return id;
    }

    void RemoveTimer(uint32_t id)
    {
        oldTimers.push_back(id);
    }

    bool DoesTimerExist(uint32_t id)
    {
        return timers.count(id) != 0;
    }

    void TimerBenchmark()
    {
        size_t totalCount = 0, everyTickCount = 0, intervalCount = 0, timeoutCount = 0;
        totalCount = timers.size();
        for(auto [id, timer] : timers)
        {
            if(timer->GetInterval() == 0 && !timer->IsOnce()) everyTickCount += 1;
            else if(timer->IsOnce())
                timeoutCount += 1;
            else
                intervalCount += 1;
        }

        Log::Info << GetResource()->GetName() << ": " << totalCount << " running timers (" << everyTickCount << " EveryTick, " << intervalCount << " Interval, " << timeoutCount << " Timeout"
                  << ")" << Log::Endl;
    }

    void NotifyPoolUpdate(alt::IBaseObject* ent);

    v8::Local<v8::Array> GetAllPlayers();
    v8::Local<v8::Array> GetAllVehicles();
    v8::Local<v8::Array> GetAllBlips();
#ifdef ALT_CLIENT_API
    v8::Local<v8::Array> GetAllObjects();
#endif

    std::vector<V8Helpers::EventCallback*> GetLocalHandlers(const std::string& name);
    std::vector<V8Helpers::EventCallback*> GetRemoteHandlers(const std::string& name);
    std::vector<V8Helpers::EventCallback*> GetGenericHandlers(bool local);

    using NextTickCallback = std::function<void()>;
    void RunOnNextTick(NextTickCallback&& callback)
    {
        nextTickCallbacks.push_back(callback);
    }

    v8::Local<v8::Object> GetOrCreateResourceObject(alt::IResource* resource);
    void DeleteResourceObject(alt::IResource* resource);

    bool HasBenchmarkTimer(const std::string& name)
    {
        return benchmarkTimers.count(name) != 0;
    }
    void CreateBenchmarkTimer(const std::string& name)
    {
        benchmarkTimers.insert({ name, std::chrono::high_resolution_clock::now() });
    }
    void RemoveBenchmarkTimer(const std::string& name)
    {
        benchmarkTimers.erase(name);
    }
    std::chrono::high_resolution_clock::time_point GetBenchmarkTimerStart(const std::string& name)
    {
        return benchmarkTimers.at(name);
    }

    v8::Local<v8::Function> GetLogFunction()
    {
        return logFunction.Get(isolate);
    }

    void SetLogFunction(v8::Local<v8::Function> function)
    {
        logFunction.Reset(isolate, function);
    }

    void SetupScriptGlobals();

    static V8ResourceImpl* Get(v8::Local<v8::Context> ctx)
    {
        alt::IResource* resource = GetResource(ctx);
        return resource ? static_cast<V8ResourceImpl*>(resource->GetImpl()) : nullptr;
    }

    static alt::IResource* GetResource(v8::Local<v8::Context> ctx)
    {
        return static_cast<alt::IResource*>(ctx->GetAlignedPointerFromEmbedderData(1));
    }

protected:
    v8::Isolate* isolate;
    alt::IResource* resource;

    V8Helpers::CPersistent<v8::Context> context;

    std::unordered_map<alt::IBaseObject*, V8Entity*> entities;
    std::unordered_map<uint32_t, V8Timer*> timers;
    // Key = Name, Value = Start time
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> benchmarkTimers;

    std::unordered_multimap<std::string, V8Helpers::EventCallback> localHandlers;
    std::unordered_multimap<std::string, V8Helpers::EventCallback> remoteHandlers;
    std::vector<V8Helpers::EventCallback> localGenericHandlers;
    std::vector<V8Helpers::EventCallback> remoteGenericHandlers;

    uint32_t nextTimerId = 0;
    std::vector<uint32_t> oldTimers;

    bool playerPoolDirty = true;
    V8Helpers::CPersistent<v8::Array> players;

    bool vehiclePoolDirty = true;
    V8Helpers::CPersistent<v8::Array> vehicles;

    bool objectPoolDirty = true;
    V8Helpers::CPersistent<v8::Array> objects;

    V8Helpers::CPersistent<v8::Function> vector3Class;
    V8Helpers::CPersistent<v8::Function> vector2Class;
    V8Helpers::CPersistent<v8::Function> rgbaClass;
    V8Helpers::CPersistent<v8::Function> baseObjectClass;

    V8Helpers::CPersistent<v8::Function> logFunction;

    std::unordered_map<alt::IResource*, V8Helpers::CPersistent<v8::Object>> resourceObjects;

    std::vector<NextTickCallback> nextTickCallbacks;

    // TEMP
    static int64_t GetTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    void InvokeEventHandlers(const alt::CEvent* ev, const std::vector<V8Helpers::EventCallback*>& handlers, std::vector<v8::Local<v8::Value>>& args, bool waitForPromiseResolve = false);
};
