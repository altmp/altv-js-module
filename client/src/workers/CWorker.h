#pragma once
#include "v8.h"
#include "V8Helpers.h"
#include "WorkerPromiseRejections.h"
#include "CEventHandler.h"
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
    using TimerId = uint32_t;
    using BufferId = uint32_t;

    friend class WorkerPromiseRejections;
    friend class WorkerTimer;

private:
    std::string filePath;
    bool eval;
    std::thread thread;
    CV8ResourceImpl* resource;
    bool shouldTerminate = false;
    bool isReady = false;
    bool isPaused = false;

    CEventHandler mainEvents;
    CEventHandler workerEvents;

    WorkerPromiseRejections promiseRejections;

    v8::Isolate* isolate = nullptr;
    V8Helpers::CPersistent<v8::Context> context;
    std::unique_ptr<v8::MicrotaskQueue> microtaskQueue;

    TimerId nextTimerId = 0;
    std::vector<TimerId> oldTimers;
    std::unordered_map<TimerId, WorkerTimer*> timers;

    static BufferId nextBufferId;
    static std::unordered_map<BufferId, std::shared_ptr<v8::BackingStore>> sharedArrayBuffers;

    void Thread();

    bool EventLoop();

    bool Setup();
    void SetupIsolate();
    void SetupContext();
    void SetupGlobals();
    bool SetupScript();

    void DestroyIsolate();

    void EmitError(const std::string& error);

    static inline int64_t GetTime()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

public:
    CWorker(std::string& filePath, bool& eval, CV8ResourceImpl* resource);
    ~CWorker() = default;

    void Start();
    void Destroy();

    void Pause()
    {
        isPaused = true;
    }
    void Resume()
    {
        isPaused = false;
    }

    TimerId CreateTimer(v8::Local<v8::Function> callback, uint32_t interval, bool once, V8Helpers::SourceLocation&& location);
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

    CEventHandler& GetMainEventHandler()
    {
        return mainEvents;
    }
    CEventHandler& GetWorkerEventHandler()
    {
        return workerEvents;
    }

    void OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage& data)
    {
        promiseRejections.RejectedWithNoHandler(this, data);
    }
    void OnPromiseHandlerAdded(v8::PromiseRejectMessage& data)
    {
        promiseRejections.HandlerAdded(this, data);
    }

    static v8::MaybeLocal<v8::Module> Import(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray>, v8::Local<v8::Module> referrer);

    // Returns error or empty string
    static std::string TryCatch(const std::function<void()>& func);

    // Shared array buffers
    static BufferId AddSharedArrayBuffer(v8::Local<v8::SharedArrayBuffer> buffer);
    static bool RemoveSharedArrayBuffer(BufferId index);
    static v8::Local<v8::SharedArrayBuffer> GetSharedArrayBuffer(v8::Isolate* isolate, BufferId index);
};
