#pragma once
#include "v8.h"
#include "V8Helpers.h"

#include <string>
#include <thread>
#include <map>
#include <queue>

class CV8ResourceImpl;

class CWorker
{
    std::string filePath;
    std::thread thread;
    CV8ResourceImpl* resource;
    bool shouldTerminate = false;
    bool isReady = false;

    void Thread();

    std::unordered_multimap<std::string, V8::EventCallback> main_eventHandlers;
    std::unordered_multimap<std::string, V8::EventCallback> worker_eventHandlers;

    using QueuedEvent = std::pair<std::string, std::vector<alt::MValue>>;
    std::queue<QueuedEvent> main_queuedEvents;
    std::queue<QueuedEvent> worker_queuedEvents;
    std::mutex main_queueLock;
    std::mutex worker_queueLock;

    v8::Isolate* isolate = nullptr;
    v8::Local<v8::Context> context;

    v8::Local<v8::Function> destroyHandler;

    bool EventLoop();

    bool SetupIsolate();
    void DestroyIsolate();
    void SetupGlobals(v8::Local<v8::Object> global);

public:
    CWorker(const std::string& filePath, CV8ResourceImpl* resource);
    ~CWorker() = default;

    void Start();
    void Destroy();

    void EmitToWorker(const std::string& eventName, std::vector<alt::MValue>& args);
    void EmitToMain(const std::string& eventName, std::vector<alt::MValue>& args);

    void SubscribeToWorker(const std::string& eventName, v8::Local<v8::Function> callback, bool once = false);
    void SubscribeToMain(const std::string& eventName, v8::Local<v8::Function> callback, bool once = false);

    void HandleMainEventQueue();
    void HandleWorkerEventQueue();

    void SetDestroyHandler(v8::Local<v8::Function> handler);

    std::string GetFilePath()
    {
        return filePath;
    }
    bool IsReady()
    {
        return isReady;
    }
};
