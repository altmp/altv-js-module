#pragma once

// Inspired by chromium and nodejs

#include "v8.h"
#include "V8Helpers.h"

class CV8ResourceImpl;

namespace V8Helpers
{
    struct PromiseRejection
    {
        V8Helpers::CPersistent<v8::Promise> promise;
        V8Helpers::CPersistent<v8::Value> value;
        V8Helpers::SourceLocation location;
        V8Helpers::StackTrace stackTrace;

        PromiseRejection(v8::Isolate* isolate, v8::Local<v8::Promise> promise, v8::Local<v8::Value> value, V8Helpers::SourceLocation&& location, V8Helpers::StackTrace&& stackTrace);
    };

    class PromiseRejections
    {
    public:
        void RejectedWithNoHandler(CV8ResourceImpl* resource, v8::PromiseRejectMessage& data);
        void HandlerAdded(CV8ResourceImpl* resource, v8::PromiseRejectMessage& data);
        void ProcessQueue(CV8ResourceImpl* resource);

    private:
        std::vector<std::unique_ptr<PromiseRejection>> queue;
    };
}  // namespace V8Helpers
