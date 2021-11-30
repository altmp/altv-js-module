#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CResourceStartEvent.h"
#include "cpp-sdk/events/CResourceStopEvent.h"
#include "cpp-sdk/events/CResourceErrorEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_LOCAL_EVENT_HANDLER anyResourceStart(EventType::RESOURCE_START, "anyResourceStart", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CResourceStartEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(V8::JSValue(ev->GetResource()->GetName().CStr()));
});

V8_LOCAL_EVENT_HANDLER anyResourceStop(EventType::RESOURCE_STOP, "anyResourceStop", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CResourceStopEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(V8::JSValue(ev->GetResource()->GetName().CStr()));
});

V8_LOCAL_EVENT_HANDLER anyResourceError(EventType::RESOURCE_ERROR, "anyResourceError", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CResourceErrorEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(V8::JSValue(ev->GetResource()->GetName().CStr()));
});
