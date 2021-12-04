#pragma once

// Inspired by chromium and nodejs

#include "v8.h"
#include "V8Helpers.h"

class V8ResourceImpl;

namespace V8
{
    struct PromiseRejection
    {
        V8::CPersistent<v8::Promise> promise;
        V8::CPersistent<v8::Value> value;
        V8::SourceLocation location;
        V8::StackTrace stackTrace;

        PromiseRejection(v8::Isolate* isolate, v8::Local<v8::Promise> promise, v8::Local<v8::Value> value, V8::SourceLocation&& location, V8::StackTrace&& stackTrace);
    };

    class PromiseRejections
    {
    public:
        void RejectedWithNoHandler(V8ResourceImpl* resource, v8::PromiseRejectMessage& data);
        void HandlerAdded(V8ResourceImpl* resource, v8::PromiseRejectMessage& data);
        void ProcessQueue(V8ResourceImpl* resource);

    private:
        std::vector<std::unique_ptr<PromiseRejection>> queue;
    };
}  // namespace V8
