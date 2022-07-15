#include "../V8ResourceImpl.h"
#include "../V8Helpers.h"

#include "cpp-sdk/events/CPlayerChangeAnimationEvent.h"

using EventType = alt::CEvent::Type;

V8_LOCAL_EVENT_HANDLER
animationChange(EventType::PLAYER_CHANGE_ANIMATION_EVENT, "playerAnimationChange", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CPlayerChangeAnimationEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
    args.push_back(V8Helpers::JSValue(ev->GetOldAnimationDict()));
    args.push_back(V8Helpers::JSValue(ev->GetNewAnimationDict()));
    args.push_back(V8Helpers::JSValue(ev->GetOldAnimationName()));
    args.push_back(V8Helpers::JSValue(ev->GetNewAnimationName()));
});
