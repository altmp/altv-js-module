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

    bool OnEvent(const alt::CEvent* ev) override;

    void OnTick() override;

    void OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage& data);
    void OnPromiseHandlerAdded(v8::PromiseRejectMessage& data);

    void SubscribeWebView(alt::Ref<alt::IWebView> view, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location, bool once = false)
    {
        webViewHandlers[view].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location), once } });
    }

    void UnsubscribeWebView(alt::Ref<alt::IWebView> view, const std::string& evName, v8::Local<v8::Function> cb)
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

    std::vector<V8Helpers::EventCallback*> GetWebViewHandlers(alt::Ref<alt::IWebView> view, const std::string& name);

    void SubscribeWebSocketClient(alt::Ref<alt::IWebSocketClient> webSocket, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location)
    {
        webSocketClientHandlers[webSocket].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location) } });
    }

    void UnsubscribeWebSocketClient(alt::Ref<alt::IWebSocketClient> webSocket, const std::string& evName, v8::Local<v8::Function> cb)
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

    std::vector<V8Helpers::EventCallback*> GetWebSocketClientHandlers(alt::Ref<alt::IWebSocketClient> webSocket, const std::string& name);

    void SubscribeAudio(alt::Ref<alt::IAudio> audio, const std::string& evName, v8::Local<v8::Function> cb, V8Helpers::SourceLocation&& location)
    {
        audioHandlers[audio].insert({ evName, V8Helpers::EventCallback{ isolate, cb, std::move(location) } });
    }

    void UnsubscribeAudio(alt::Ref<alt::IAudio> audio, const std::string& evName, v8::Local<v8::Function> cb)
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

    std::vector<V8Helpers::EventCallback*> GetAudioHandlers(alt::Ref<alt::IAudio> audio, const std::string& name);

    void AddOwned(alt::Ref<alt::IBaseObject> handle)
    {
        ownedObjects.insert(handle);
    }

    void OnRemoveBaseObject(alt::Ref<alt::IBaseObject> handle)
    {
        ownedObjects.erase(handle);

        if(handle->GetType() == alt::IBaseObject::Type::WEBVIEW) webViewHandlers.erase(handle.As<alt::IWebView>());

        if(handle->GetType() == alt::IBaseObject::Type::WEBSOCKET_CLIENT) webSocketClientHandlers.erase(handle.As<alt::IWebSocketClient>());

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

    void AddWebViewEventToQueue(const alt::Ref<alt::IWebView> view, const alt::String& evName, const alt::MValueArgs& mvArgs)
    {
        webViewsEventsQueue[view].push_back(std::pair(evName, mvArgs));
    }

private:
    using WebViewEvents = std::unordered_multimap<std::string, V8Helpers::EventCallback>;
    using WebViewsEventsQueue = std::unordered_map<alt::Ref<alt::IWebView>, std::vector<std::pair<alt::String, alt::MValueArgs>>>;

    std::unordered_map<alt::Ref<alt::IWebView>, WebViewEvents> webViewHandlers;
    std::unordered_map<alt::Ref<alt::IWebSocketClient>, WebViewEvents> webSocketClientHandlers;
    std::unordered_map<alt::Ref<alt::IAudio>, WebViewEvents> audioHandlers;

    WebViewsEventsQueue webViewsEventsQueue;
    WebViewsEventsQueue& GetWebviewsEventQueue()
    {
        return webViewsEventsQueue;
    }
    void HandleWebViewEventQueue(const alt::Ref<alt::IWebView> view);

    std::unordered_set<alt::Ref<alt::IBaseObject>> ownedObjects;

    std::unordered_set<CWorker*> workers;

    v8::Persistent<v8::Object> localStorage;

    std::unique_ptr<v8::MicrotaskQueue> microtaskQueue;

    std::list<std::function<void()>> dynamicImports;
    friend class CV8ScriptRuntime;

    V8Helpers::PromiseRejections promiseRejections;
};
