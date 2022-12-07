#include "CEventHandler.h"
#include "V8ResourceImpl.h"

void CEventHandler::Emit(const std::string& eventName, const std::vector<V8Helpers::Serialization::Value>& args)
{
    std::scoped_lock lock(queueLock);
    queue.push(std::make_pair(eventName, std::move(args)));
}

void CEventHandler::Subscribe(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    std::scoped_lock lock(handlersLock);
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    // todo: This is a subclass of resource, this shouldn't be like this
    auto resource = V8ResourceImpl::Get(isolate->GetEnteredOrMicrotaskContext());
    handlers.insert(std::make_pair(eventName, V8Helpers::EventCallback(isolate, callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource), once)));
}

void CEventHandler::Unsubscribe(const std::string& eventName, v8::Local<v8::Function> callback)
{
    std::scoped_lock lock(handlersLock);
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto range = handlers.equal_range(eventName);
    for(auto it = range.first; it != range.second; ++it)
    {
        if(it->second.fn.Get(isolate)->StrictEquals(callback)) it->second.removed = true;
    }
}

void CEventHandler::CleanupHandlers()
{
    std::scoped_lock lock(handlersLock);
    // Clear removed event handlers
    for(auto it = handlers.begin(); it != handlers.end();)
    {
        if(it->second.removed) it = handlers.erase(it);
        else
            ++it;
    }
}

void CEventHandler::Process()
{
    if(queue.empty()) return;
    CleanupHandlers();

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();

    while(!queue.empty())
    {
        // Get the event at the front of the queue
        queueLock.lock();
        QueueItem& event = queue.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args;
        args.reserve(event.second.size());
        for(auto& arg : event.second)
        {
            auto value = V8Helpers::Serialization::Deserialize(context, arg);
            if(value.IsEmpty())
            {
                Log::Error << "Failed to deserialize worker event argument for event '" << event.first << "'" << Log::Endl;
                continue;
            }
            args.push_back(value.ToLocalChecked());
        }
        handlersLock.lock();
        auto evHandlers = handlers.equal_range(event.first);
        queueLock.unlock();

        // Call all handlers with the arguments
        for(auto it = evHandlers.first; it != evHandlers.second; it++)
        {
            V8Helpers::CallFunctionWithTimeout(it->second.fn.Get(isolate), context, args);
            if(it->second.once) it->second.removed = true;
        }
        handlersLock.unlock();

        // Pop the event from the queue
        queueLock.lock();
        queue.pop();
        queueLock.unlock();
    }
}

void CEventHandler::Reset()
{
    std::scoped_lock lock(queueLock);
    std::scoped_lock lock2(handlersLock);
    while(!queue.empty()) queue.pop();
    handlers.clear();
}
