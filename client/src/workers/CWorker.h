#pragma once
#include "v8.h"
#include "V8Helpers.h"
#include "WorkerPromiseRejections.h"
#include "../IImportHandler.h"

#include <string>
#include <thread>
#include <map>
#include <queue>
#include <chrono>

class CV8ResourceImpl;
class WorkerTimer;

class CWorker : public IImportHandler
{
public:
    struct EventDataItem
    {
        uint8_t* data;
        size_t size;

        bool Valid() const
        {
            return data != nullptr && size > 0;
        }

        EventDataItem() : data(nullptr), size(0) {}
        EventDataItem(uint8_t* data, size_t size) : data(data), size(size) {}
        ~EventDataItem()
        {
            free(data);
        }
    };

    using EventHandlerMap = std::unordered_multimap<std::string, V8::EventCallback>;
    using QueuedEvent = std::pair<std::string, std::vector<EventDataItem>>;
    using EventQueue = std::queue<QueuedEvent>;
    using TimerId = uint32_t;
    using BufferId = uint32_t;

private:
    alt::String filePath;
    alt::String origin;
    std::thread thread;
    CV8ResourceImpl* resource;
    bool shouldTerminate = false;
    bool isReady = false;
    bool isPaused = false;

    WorkerPromiseRejections promiseRejections;

    EventHandlerMap main_eventHandlers;
    EventHandlerMap worker_eventHandlers;

    EventQueue main_queuedEvents;
    EventQueue worker_queuedEvents;
    std::mutex main_queueLock;
    std::mutex worker_queueLock;

    v8::Isolate* isolate = nullptr;
    V8::CPersistent<v8::Context> context;
    std::unique_ptr<v8::MicrotaskQueue> microtaskQueue;

    TimerId nextTimerId = 0;
    std::vector<TimerId> oldTimers;
    std::unordered_map<TimerId, WorkerTimer*> timers;

    static BufferId nextBufferId;
    static std::unordered_map<BufferId, std::shared_ptr<v8::BackingStore>> sharedArrayBuffers;

    void Thread();

    bool EventLoop();

    bool SetupIsolate();
    void DestroyIsolate();
    void SetupGlobals(v8::Local<v8::Object> global);

    static inline int64_t GetTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

public:
    CWorker(alt::String& filePath, alt::String& origin, CV8ResourceImpl* resource);
    ~CWorker() = default;

    void Start();
    void Destroy()
    {
        shouldTerminate = true;
    }

    void Pause()
    {
        isPaused = true;
    }
    void Resume()
    {
        isPaused = false;
    }

    void EmitToWorker(const std::string& eventName, std::vector<CWorker::EventDataItem>& args);
    void EmitToMain(const std::string& eventName, std::vector<CWorker::EventDataItem>& args);

    void SubscribeToWorker(const std::string& eventName, v8::Local<v8::Function> callback, bool once = false);
    void SubscribeToMain(const std::string& eventName, v8::Local<v8::Function> callback, bool once = false);

    void HandleMainEventQueue();
    void HandleWorkerEventQueue();

    void EmitError(const std::string& error);

    TimerId CreateTimer(v8::Local<v8::Function> callback, uint32_t interval, bool once, V8::SourceLocation&& location);
    void RemoveTimer(TimerId id)
    {
        oldTimers.push_back(id);
    }

    std::string GetFilePath()
    {
        return filePath.ToString();
    }
    bool IsReady()
    {
        return isReady;
    }
    bool IsPaused()
    {
        return isPaused;
    }
    v8::Isolate* GetIsolate()
    {
        return isolate;
    }
    CV8ResourceImpl* GetResource()
    {
        return resource;
    }

    void OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage& data)
    {
        promiseRejections.RejectedWithNoHandler(this, data);
    }
    void OnPromiseHandlerAdded(v8::PromiseRejectMessage& data)
    {
        promiseRejections.HandlerAdded(this, data);
    }

    static EventDataItem SerializeValue(v8::Local<v8::Context> context, v8::Local<v8::Value> value);
    static v8::MaybeLocal<v8::Value> DeserializeValue(v8::Local<v8::Context> context, const EventDataItem& item);

    static v8::MaybeLocal<v8::Module> Import(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray>, v8::Local<v8::Module> referrer);

    // Returns error or empty string
    static std::string TryCatch(const std::function<void()>& func);

    // Shared array buffers
    static BufferId AddSharedArrayBuffer(v8::Local<v8::SharedArrayBuffer> buffer);
    static bool RemoveSharedArrayBuffer(BufferId index);
    static v8::Local<v8::SharedArrayBuffer> GetSharedArrayBuffer(v8::Isolate* isolate, BufferId index);
};
