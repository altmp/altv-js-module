#pragma once
#include "v8.h"
#include "V8Helpers.h"

#include <string>
#include <thread>
#include <map>
#include <queue>
#include <chrono>

class CV8ResourceImpl;
class V8Timer;

class CWorker
{
    std::string filePath;
    std::thread thread;
    CV8ResourceImpl* resource;
    bool shouldTerminate = false;
    bool isReady = false;

    std::unordered_multimap<std::string, V8::EventCallback> main_eventHandlers;
    std::unordered_multimap<std::string, V8::EventCallback> worker_eventHandlers;

    using QueuedEvent = std::pair<std::string, std::vector<alt::MValue>>;
    std::queue<QueuedEvent> main_queuedEvents;
    std::queue<QueuedEvent> worker_queuedEvents;
    std::mutex main_queueLock;
    std::mutex worker_queueLock;

    v8::Isolate* isolate = nullptr;
    V8::CPersistent<v8::Context> context;

    using TimerId = uint32_t;
    TimerId nextTimerId = 0;
    std::vector<TimerId> oldTimers;
    std::unordered_map<TimerId, V8Timer*> timers;

    void Thread();

    bool EventLoop();

    bool SetupIsolate();
    void DestroyIsolate();
    void SetupGlobals(v8::Local<v8::Object> global);

    void EmitError(const std::string& error);

    static inline int64_t GetTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

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

    TimerId CreateTimer(v8::Local<v8::Function> callback, uint32_t interval, bool once, V8::SourceLocation&& location);
    void RemoveTimer(TimerId id)
    {
        oldTimers.push_back(id);
    }

    std::string GetFilePath()
    {
        return filePath;
    }
    bool IsReady()
    {
        return isReady;
    }
};
