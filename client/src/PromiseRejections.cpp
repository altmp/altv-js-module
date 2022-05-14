
#include "CV8Resource.h"
#include "V8Helpers.h"

#include "PromiseRejections.h"

void V8Helpers::PromiseRejections::RejectedWithNoHandler(CV8ResourceImpl* resource, v8::PromiseRejectMessage& data)
{
    v8::Isolate* isolate = resource->GetIsolate();

    queue.push_back(
      std::make_unique<PromiseRejection>(isolate, data.GetPromise(), data.GetValue(), V8Helpers::SourceLocation::GetCurrent(isolate), V8Helpers::StackTrace::GetCurrent(isolate)));
}

void V8Helpers::PromiseRejections::HandlerAdded(CV8ResourceImpl* resource, v8::PromiseRejectMessage& data)
{
    v8::Isolate* isolate = resource->GetIsolate();

    auto newEnd = std::remove_if(queue.begin(), queue.end(), [&](std::unique_ptr<PromiseRejection>& rejection) { return rejection->promise.Get(isolate) == data.GetPromise(); });

    queue.erase(newEnd, queue.end());
}

void V8Helpers::PromiseRejections::ProcessQueue(CV8ResourceImpl* resource)
{
    v8::Isolate* isolate = resource->GetIsolate();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    for(auto& rejection : queue)
    {
        std::string rejectionMsg = *v8::String::Utf8Value(isolate, rejection->value.Get(isolate)->ToString(ctx).ToLocalChecked());
        auto fileName = rejection->location.GetFileName();
        if(rejection->location.GetLineNumber() != 0 && !resource->IsBytecodeResource())
        {
            Log::Error << "[V8] Unhandled promise rejection at " << resource->GetResource()->GetName() << ":" << fileName << ":" << rejection->location.GetLineNumber() << " ("
                       << rejectionMsg << ")" << Log::Endl;
        }
        else
        {
            Log::Error << "[V8] Unhandled promise rejection at " << resource->GetResource()->GetName() << ":" << fileName << " (" << rejectionMsg << ")" << Log::Endl;
        }
        rejection->stackTrace.Print(1);

        resource->DispatchErrorEvent(rejectionMsg, fileName, rejection->location.GetLineNumber(), rejection->stackTrace.ToString());
    }

    queue.clear();
}

V8Helpers::PromiseRejection::PromiseRejection(
  v8::Isolate* isolate, v8::Local<v8::Promise> _promise, v8::Local<v8::Value> _value, V8Helpers::SourceLocation&& _location, V8Helpers::StackTrace&& _stackTrace)
    : promise(isolate, _promise), value(isolate, _value), location(std::move(_location)), stackTrace(std::move(_stackTrace))
{
}
