#pragma once

#include <queue>
#include <string>
#include <mutex>

#include "V8Helpers.h"

class CEventHandler
{
public:
    using QueueItem = std::pair<std::string, std::vector<V8Helpers::Serialization::Value>>;
    using Queue = std::queue<QueueItem>;
    using HandlerMap = std::unordered_multimap<std::string, V8Helpers::EventCallback>;

private:
    Queue queue;
    std::mutex queueLock;
    HandlerMap handlers;
    std::mutex handlersLock;

    void CleanupHandlers();

public:
    void Emit(const std::string& eventName, const std::vector<V8Helpers::Serialization::Value>& args);
    void Subscribe(const std::string& eventName, v8::Local<v8::Function> callback, bool once = false);
    void Unsubscribe(const std::string& eventName, v8::Local<v8::Function> callback);

    void Process();

    void Reset();
};
