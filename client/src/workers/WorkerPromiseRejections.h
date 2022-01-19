#pragma once
#include "v8.h"
#include "V8Helpers.h"

class CWorker;

struct WorkerPromiseRejection
{
    V8Helpers::CPersistent<v8::Promise> promise;
    V8Helpers::CPersistent<v8::Value> value;
    V8Helpers::SourceLocation location;

    WorkerPromiseRejection(v8::Isolate* isolate, v8::Local<v8::Promise> promise, v8::Local<v8::Value> value, V8Helpers::SourceLocation&& location);
};

class WorkerPromiseRejections
{
public:
    void RejectedWithNoHandler(CWorker* worker, v8::PromiseRejectMessage& data);
    void HandlerAdded(CWorker* worker, v8::PromiseRejectMessage& data);
    void ProcessQueue(CWorker* worker);

private:
    std::vector<std::unique_ptr<WorkerPromiseRejection>> queue;
};
