
#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "PromiseRejections.h"

void V8::PromiseRejections::RejectedWithNoHandler(V8ResourceImpl* resource, v8::PromiseRejectMessage& data)
{
    v8::Isolate* isolate = resource->GetIsolate();

    queue.push_back(std::make_unique<PromiseRejection>(isolate, data.GetPromise(), data.GetValue(), V8::SourceLocation::GetCurrent(isolate)));
}

void V8::PromiseRejections::HandlerAdded(V8ResourceImpl* resource, v8::PromiseRejectMessage& data)
{
    v8::Isolate* isolate = resource->GetIsolate();

    auto newEnd = std::remove_if(queue.begin(), queue.end(), [&](std::unique_ptr<PromiseRejection>& rejection) { return rejection->promise.Get(isolate) == data.GetPromise(); });

    queue.erase(newEnd, queue.end());
}

void V8::PromiseRejections::ProcessQueue(V8ResourceImpl* resource)
{
    v8::Isolate* isolate = resource->GetIsolate();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    for(auto& rejection : queue)
    {
        auto rejectionMsg = *v8::String::Utf8Value(isolate, rejection->value.Get(isolate)->ToString(ctx).ToLocalChecked());
        auto fileName = rejection->location.GetFileName();
        if(rejection->location.GetLineNumber() != 0)
        {
            Log::Error << "[V8] Unhandled promise rejection at " << resource->GetResource()->GetName() << ":" << fileName << ":" << rejection->location.GetLineNumber() << " ("
                       << rejectionMsg << ")" << Log::Endl;
        }
        else
        {
            Log::Error << "[V8] Unhandled promise rejection at " << resource->GetResource()->GetName() << ":" << fileName << " (" << rejectionMsg << ")" << Log::Endl;
        }

        resource->DispatchErrorEvent(rejectionMsg, fileName, rejection->location.GetLineNumber());
    }

    queue.clear();
}

V8::PromiseRejection::PromiseRejection(v8::Isolate* isolate, v8::Local<v8::Promise> _promise, v8::Local<v8::Value> _value, V8::SourceLocation&& _location)
    : promise(isolate, _promise), value(isolate, _value), location(std::move(_location))
{
}
