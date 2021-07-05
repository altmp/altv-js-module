#include "../CV8Resource.h"
#include "../CV8ScriptRuntime.h"

#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Helpers.h"

#include "cpp-sdk/events/CConnectionComplete.h"
#include "cpp-sdk/events/CDisconnectEvent.h"
#include "cpp-sdk/events/CRenderEvent.h"
#include "cpp-sdk/events/CConsoleCommandEvent.h"
#include "cpp-sdk/events/CClientScriptEvent.h"
#include "cpp-sdk/events/CServerScriptEvent.h"
#include "cpp-sdk/events/CKeyboardEvent.h"
#include "cpp-sdk/events/CWebViewEvent.h"
#include "cpp-sdk/events/CWebSocketClientEvent.h"
#include "cpp-sdk/events/CAudioEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_EVENT_HANDLER clientScriptEvent(
	EventType::CLIENT_SCRIPT_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CClientScriptEvent*>(e);
		return resource->GetLocalHandlers(ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CClientScriptEvent*>(e);

		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8_EVENT_HANDLER serverScriptEvent(
	EventType::SERVER_SCRIPT_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CServerScriptEvent*>(e);
		return resource->GetRemoteHandlers(ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CServerScriptEvent*>(e);

		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8_EVENT_HANDLER webviewEvent(
	EventType::WEB_VIEW_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CWebViewEvent*>(e);

		return static_cast<CV8ResourceImpl*>(resource)->GetWebViewHandlers(ev->GetTarget(), ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CWebViewEvent*>(e);

		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8_EVENT_HANDLER webSocketEvent(
	EventType::WEB_SOCKET_CLIENT_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CWebSocketClientEvent*>(e);

		return static_cast<CV8ResourceImpl*>(resource)->GetWebSocketClientHandlers(ev->GetTarget(), ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CWebSocketClientEvent*>(e);

		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8_EVENT_HANDLER audioEvent(
	EventType::AUDIO_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CAudioEvent*>(e);

		return static_cast<CV8ResourceImpl*>(resource)->GetAudioHandlers(ev->GetTarget(), ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CAudioEvent*>(e);

		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8_EVENT_HANDLER keyboardEvent(
	EventType::KEYBOARD_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CKeyboardEvent*>(e);
		if (ev->GetKeyState() == alt::CKeyboardEvent::KeyState::UP)
			return resource->GetLocalHandlers("keyup");
		else if (ev->GetKeyState() == alt::CKeyboardEvent::KeyState::DOWN)
			return resource->GetLocalHandlers("keydown");
		else
		{
			Log::Error << "Unhandled keystate in keyboard event handler: " << (int)ev->GetKeyState() << Log::Endl;
			return std::vector<V8::EventCallback*>();
		}
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CKeyboardEvent*>(e);
		v8::Isolate *isolate = resource->GetIsolate();

		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetKeyCode()));
	}
);

V8_LOCAL_EVENT_HANDLER render(
	EventType::RENDER,
	"render",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
	});

V8_LOCAL_EVENT_HANDLER connectionComplete(
	EventType::CONNECTION_COMPLETE,
	"connectionComplete",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
        CV8ScriptRuntime& runtime = CV8ScriptRuntime::Instance();
		if(!runtime.resourcesLoaded) 
        {
            runtime.resourcesLoaded = true;
            static_cast<CV8ResourceImpl*>(resource)->ProcessDynamicImports();
        }
	});

V8_LOCAL_EVENT_HANDLER disconnect(
	EventType::DISCONNECT_EVENT,
	"disconnect",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
        CV8ScriptRuntime::Instance().resourcesLoaded = false;
	});
