#include "CWorker.h"
#include <functional>

CWorker::CWorker(const std::string& filePath) : filePath(filePath)
{
    Start();
}

void CWorker::Destroy()
{
    shouldTerminate = true;
}

void CWorker::EmitToWorker(const std::string& eventName, std::vector<alt::MValue>& args)
{
    std::unique_lock<std::mutex> lock(worker_queueLock);
    worker_queuedEvents.push(std::make_pair(eventName, args));
}

void CWorker::EmitToMain(const std::string& eventName, std::vector<alt::MValue>& args)
{
    std::unique_lock<std::mutex> lock(main_queueLock);
    main_queuedEvents.push(std::make_pair(eventName, args));
}

void CWorker::SubscribeToWorker(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    auto isolate = v8::Isolate::GetCurrent();
    worker_eventHandlers.insert({ eventName, V8::EventCallback(isolate, callback, V8::SourceLocation::GetCurrent(isolate), once) });
}

void CWorker::SubscribeToMain(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    auto isolate = v8::Isolate::GetCurrent();
    main_eventHandlers.insert({ eventName, V8::EventCallback(isolate, callback, V8::SourceLocation::GetCurrent(isolate), once) });
}

void CWorker::Start()
{
    thread = std::thread(std::bind(&CWorker::Thread, this));
    thread.detach();
}

void CWorker::Thread()
{
    SetupIsolate();
    isReady = true;

    while(true)
    {
        // Sleep for a short while to not overload the thread
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        if(!EventLoop()) break;
    }

    DestroyIsolate();
    delete this;  // ! IMPORTANT TO DO THIS LAST !
}

bool CWorker::EventLoop()
{
    if(shouldTerminate) return false;
    // todo: event loop

    HandleWorkerEventQueue();

    return true;
}

void CWorker::SetupIsolate()
{
    // todo: create isolate and stuff
}

void CWorker::DestroyIsolate()
{
    // todo: clean up isolate and stuff
}

void CWorker::SetupGlobals(v8::Local<v8::Object> global)
{
    // todo: set up global functions for the isolate
}

void CWorker::HandleMainEventQueue()
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetEnteredOrMicrotaskContext();
    std::unique_lock<std::mutex> lock(main_queueLock);

    while(!main_queuedEvents.empty())
    {
        // Get the event at the front of the queue
        auto& event = main_queuedEvents.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args(event.second.size());
        for(auto& arg : event.second)
        {
            args.push_back(V8Helpers::MValueToV8(arg));
        }

        // Call all handlers with the arguments
        auto handlers = main_eventHandlers.equal_range(event.first);
        for(auto it = handlers.first; it != handlers.second; it++)
        {
            it->second.fn.Get(isolate)->Call(context, v8::Undefined(isolate), args.size(), args.data());
            if(it->second.once) main_eventHandlers.erase(it);
        }

        // Pop the event from the queue
        main_queuedEvents.pop();
    }
}

void CWorker::HandleWorkerEventQueue()
{
    // todo: handle event queue
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetEnteredOrMicrotaskContext();
    std::unique_lock<std::mutex> lock(worker_queueLock);

    while(!worker_queuedEvents.empty())
    {
        // Get the event at the front of the queue
        auto& event = worker_queuedEvents.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args(event.second.size());
        for(auto& arg : event.second)
        {
            args.push_back(V8Helpers::MValueToV8(arg));
        }

        // Call all handlers with the arguments
        auto handlers = worker_eventHandlers.equal_range(event.first);
        for(auto it = handlers.first; it != handlers.second; it++)
        {
            it->second.fn.Get(isolate)->Call(context, v8::Undefined(isolate), args.size(), args.data());
            if(it->second.once) worker_eventHandlers.erase(it);
        }

        // Pop the event from the queue
        worker_queuedEvents.pop();
    }
}
