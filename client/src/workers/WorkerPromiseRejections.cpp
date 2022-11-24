#include "WorkerPromiseRejections.h"
#include "CWorker.h"
#include "V8Helpers.h"

void WorkerPromiseRejections::RejectedWithNoHandler(CWorker* worker, v8::PromiseRejectMessage& data)
{
    v8::Isolate* isolate = worker->GetIsolate();

    queue.push_back(std::make_unique<WorkerPromiseRejection>(isolate, data.GetPromise(), data.GetValue(), V8Helpers::SourceLocation::GetCurrent(isolate, nullptr)));
}

void WorkerPromiseRejections::HandlerAdded(CWorker* worker, v8::PromiseRejectMessage& data)
{
    v8::Isolate* isolate = worker->GetIsolate();

    auto newEnd = std::remove_if(queue.begin(), queue.end(), [&](std::unique_ptr<WorkerPromiseRejection>& rejection) { return rejection->promise.Get(isolate) == data.GetPromise(); });

    queue.erase(newEnd, queue.end());
}

void WorkerPromiseRejections::ProcessQueue(CWorker* worker)
{
    v8::Isolate* isolate = worker->GetIsolate();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    for(auto& rejection : queue)
    {
        std::ostringstream errStream;
        std::string rejectionMsg = *v8::String::Utf8Value(isolate, rejection->value.Get(isolate)->ToString(ctx).ToLocalChecked());
        auto fileName = rejection->location.GetFileName();
        if(rejection->location.GetLineNumber() != 0) errStream << "Unhandled promise rejection at " << fileName << ":" << rejection->location.GetLineNumber() << " (" << rejectionMsg << ")";
        else
            errStream << "Unhandled promise rejection at " << fileName << " (" << rejectionMsg << ")";

        worker->EmitError(errStream.str());
    }

    queue.clear();
}

WorkerPromiseRejection::WorkerPromiseRejection(v8::Isolate* isolate, v8::Local<v8::Promise> _promise, v8::Local<v8::Value> _value, V8Helpers::SourceLocation&& _location)
    : promise(isolate, _promise), value(isolate, _value), location(std::move(_location))
{
}
