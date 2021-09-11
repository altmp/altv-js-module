#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/events/CRemoveEntityEvent.h"
#include "cpp-sdk/events/CGameEntityCreateEvent.h"
#include "cpp-sdk/events/CGameEntityDestroyEvent.h"
#include "cpp-sdk/events/CTaskChangeEvent.h"

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

V8_EVENT_HANDLER gameEntityCreate(
		EventType::GAME_ENTITY_CREATE,
		[](V8ResourceImpl* resource, const alt::CEvent* e) {
			CV8ScriptRuntime::Instance().OnEntityStreamIn(static_cast<const alt::CGameEntityCreateEvent*>(e)->GetTarget());

			return resource->GetLocalHandlers("gameEntityCreate");
		},
		[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
			auto ev = static_cast<const alt::CGameEntityCreateEvent *>(e);
			v8::Isolate *isolate = resource->GetIsolate();

			args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
		});

V8_EVENT_HANDLER gameEntityDestroy(
		EventType::GAME_ENTITY_DESTROY,
		[](V8ResourceImpl* resource, const alt::CEvent* e) {
			CV8ScriptRuntime::Instance().OnEntityStreamOut(static_cast<const alt::CGameEntityDestroyEvent*>(e)->GetTarget());
			
			return resource->GetLocalHandlers("gameEntityDestroy");
		},
		[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
			auto ev = static_cast<const alt::CGameEntityDestroyEvent *>(e);
			v8::Isolate *isolate = resource->GetIsolate();

			args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
		});

V8_LOCAL_EVENT_HANDLER taskChange(
	EventType::TASK_CHANGE,
	"taskChange",
	[](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CTaskChangeEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetOldTask()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetNewTask()));
	});