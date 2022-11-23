#pragma once

#include "cpp-sdk/IResource.h"
#include "cpp-sdk/objects/IEntity.h"

#include "V8ResourceImpl.h"
#include "IImportHandler.h"
#include "PromiseRejections.h"

#include <queue>

#include "Log.h"

class CV8ScriptRuntime;
class CWorker;

class CV8ResourceImpl : public V8ResourceImpl, public IImportHandler
{
public:
    CV8ResourceImpl(alt::IResource* resource, v8::Isolate* isolate) : V8ResourceImpl(isolate, resource) {}

    ~CV8ResourceImpl()
    {
        Log::Debug << __FUNCTION__ << Log::Endl;
    }

    void ProcessDynamicImports();

    bool Start() override;

    bool Stop() override;

    void OnEvent(const alt::CEvent* ev) override;

    void OnTick() override;

    void OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage& data);
    void OnPromiseHandlerAdded(v8::PromiseRejectMessage& data);

    void SubscribeWebView(alt::IWebView* view, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location, bool once = false)
    {
        webViewHandlers[view].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location), once } });
    }

    void UnsubscribeWebView(alt::IWebView* view, const std::string& evName, v8::Local<v8::Function> cb)
    {
        auto it = webViewHandlers.find(view);
        if(it != webViewHandlers.end())
        {
            auto& viewEvents = it->second;
            auto range = viewEvents.equal_range(evName);

            for(auto it = range.first; it != range.second; ++it)
            {
                if(it->second.fn.Get(isolate)->StrictEquals(cb)) it->second.removed = true;
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> GetWebViewHandlers(alt::IWebView* view, const std::string& name);

    void SubscribeWebSocketClient(alt::IWebSocketClient* webSocket, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location)
    {
        webSocketClientHandlers[webSocket].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location) } });
    }

    void UnsubscribeWebSocketClient(alt::IWebSocketClient* webSocket, const std::string& evName, v8::Local<v8::Function> cb)
    {
        auto it = webSocketClientHandlers.find(webSocket);
        if(it != webSocketClientHandlers.end())
        {
            auto& webSocketEvents = it->second;
            auto range = webSocketEvents.equal_range(evName);

            for(auto it = range.first; it != range.second; ++it)
            {
                if(it->second.fn.Get(isolate)->StrictEquals(cb)) it->second.removed = true;
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> GetWebSocketClientHandlers(alt::IWebSocketClient* webSocket, const std::string& name);

    void SubscribeAudio(alt::IAudio* audio, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location)
    {
        audioHandlers[audio].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location) } });
    }

    void UnsubscribeAudio(alt::IAudio* audio, const std::string& evName, v8::Local<v8::Function> cb)
    {
        auto it = audioHandlers.find(audio);
        if(it != audioHandlers.end())
        {
            auto& audioEvents = it->second;
            auto range = audioEvents.equal_range(evName);

            for(auto it = range.first; it != range.second; ++it)
            {
                if(it->second.fn.Get(isolate)->StrictEquals(cb)) it->second.removed = true;
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> GetAudioHandlers(alt::IAudio* audio, const std::string& name);

    void SubscribeRml(alt::IRmlElement* element, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location)
    {
        rmlHandlers[element].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location) } });
    }

    void UnsubscribeRml(alt::IRmlElement* element, const std::string& evName, v8::Local<v8::Function> cb)
    {
        auto it = rmlHandlers.find(element);
        if(it != rmlHandlers.end())
        {
            auto& rmlEvents = it->second;
            auto range = rmlEvents.equal_range(evName);

            for(auto it = range.first; it != range.second; ++it)
            {
                if(it->second.fn.Get(isolate)->StrictEquals(cb)) it->second.removed = true;
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> GetRmlHandlers(alt::IRmlElement* element, const std::string& name);

    void AddOwned(alt::IBaseObject* handle)
    {
        ownedObjects.insert(handle);
    }

    void OnRemoveBaseObject(alt::IBaseObject* handle)
    {
        ownedObjects.erase(handle);

        if(handle->GetType() == alt::IBaseObject::Type::WEBVIEW) webViewHandlers.erase(dynamic_cast<alt::IWebView*>(handle));

        if(handle->GetType() == alt::IBaseObject::Type::WEBSOCKET_CLIENT) webSocketClientHandlers.erase(dynamic_cast<alt::IWebSocketClient*>(handle));

        V8ResourceImpl::OnRemoveBaseObject(handle);
    }

    v8::Local<v8::Object> GetLocalStorage()
    {
        if(localStorage.IsEmpty())
        {
            std::vector<v8::Local<v8::Value>> args;
            extern V8Class v8LocalStorage;
            localStorage.Reset(isolate, v8LocalStorage.New(GetContext(), args).As<v8::Object>());
        }

        return localStorage.Get(isolate);
    }

    void AddWorker(CWorker* worker);
    void RemoveWorker(CWorker* worker);

    v8::Local<v8::Module> CreateSyntheticModule(const std::string& name, v8::Local<v8::Value> exportValue);
    v8::MaybeLocal<v8::Value> GetSyntheticModuleExport(v8::Local<v8::Module> syntheticModule);

    bool InstantiateModule(v8::Local<v8::Module> mod, bool preload = false);

    bool IsPreloading()
    {
        return isPreloading;
    }

private:
    friend class CV8ScriptRuntime;

    using EventHandlerMap = std::unordered_multimap<std::string, V8Helpers::EventCallback>;

    std::unordered_map<alt::IWebView*, EventHandlerMap> webViewHandlers;
    std::unordered_map<alt::IWebSocketClient*, EventHandlerMap> webSocketClientHandlers;
    std::unordered_map<alt::IAudio*, EventHandlerMap> audioHandlers;
    std::unordered_map<alt::IRmlElement*, EventHandlerMap> rmlHandlers;

    std::unordered_set<alt::IBaseObject*> ownedObjects;

    std::unordered_set<CWorker*> workers;

    V8Helpers::CPersistent<v8::Object> localStorage;

    std::unique_ptr<v8::MicrotaskQueue> microtaskQueue;

    std::list<std::function<void()>> dynamicImports;

    V8Helpers::PromiseRejections promiseRejections;

    // Key = Module identity hash, Value = Export value
    std::unordered_map<int, V8Helpers::CPersistent<v8::Value>> syntheticModuleExports;

    bool isPreloading = true;
};
