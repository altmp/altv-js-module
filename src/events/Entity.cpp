#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Helpers.h"

#include "cpp-sdk/events/CRemoveEntityEvent.h"
#include "cpp-sdk/events/CGameEntityCreateEvent.h"
#include "cpp-sdk/events/CGameEntityDestroyEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_LOCAL_EVENT_HANDLER removeEntity(
		EventType::REMOVE_ENTITY_EVENT,
		"removeEntity",
		[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
			auto ev = static_cast<const alt::CRemoveEntityEvent *>(e);
			v8::Isolate *isolate = resource->GetIsolate();

			args.push_back(resource->GetOrCreateEntity(ev->GetEntity().Get())->GetJSVal(isolate));
		});

V8_LOCAL_EVENT_HANDLER gameEntityCreate(
		EventType::GAME_ENTITY_CREATE,
		"gameEntityCreate",
		[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
			auto ev = static_cast<const alt::CGameEntityCreateEvent *>(e);
			v8::Isolate *isolate = resource->GetIsolate();

			args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
		});

V8_LOCAL_EVENT_HANDLER gameEntityDestroy(
		EventType::GAME_ENTITY_DESTROY,
		"gameEntityDestroy",
		[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
			auto ev = static_cast<const alt::CGameEntityCreateEvent *>(e);
			v8::Isolate *isolate = resource->GetIsolate();

			args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
		});