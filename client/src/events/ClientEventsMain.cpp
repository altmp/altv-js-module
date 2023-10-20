#include "../CV8Resource.h"
#include "../CV8ScriptRuntime.h"

#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CConnectionComplete.h"
#include "cpp-sdk/events/CDisconnectEvent.h"
#include "cpp-sdk/events/CConsoleCommandEvent.h"
#include "cpp-sdk/events/CClientScriptEvent.h"
#include "cpp-sdk/events/CServerScriptEvent.h"
#include "cpp-sdk/events/CKeyboardEvent.h"
#include "cpp-sdk/events/CWebViewEvent.h"
#include "cpp-sdk/events/CWebSocketClientEvent.h"
#include "cpp-sdk/events/CAudioEvent.h"
#include "cpp-sdk/events/CRmlEvent.h"
#include "cpp-sdk/events/CWindowFocusChangeEvent.h"
#include "cpp-sdk/events/CWindowResolutionChangeEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_EVENT_HANDLER clientScriptEvent(
  EventType::CLIENT_SCRIPT_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CClientScriptEvent*>(e);
      return resource->GetLocalHandlers(ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CClientScriptEvent*>(e);

      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8_EVENT_HANDLER serverScriptEvent(
  EventType::SERVER_SCRIPT_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CServerScriptEvent*>(e);
      return resource->GetRemoteHandlers(ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CServerScriptEvent*>(e);

      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8_EVENT_HANDLER webviewEvent(
  EventType::WEB_VIEW_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CWebViewEvent*>(e);

      return static_cast<CV8ResourceImpl*>(resource)->GetWebViewHandlers(ev->GetTarget(), ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CWebViewEvent*>(e);

      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8_EVENT_HANDLER webSocketEvent(
  EventType::WEB_SOCKET_CLIENT_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CWebSocketClientEvent*>(e);

      return static_cast<CV8ResourceImpl*>(resource)->GetWebSocketClientHandlers(ev->GetTarget(), ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CWebSocketClientEvent*>(e);

      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8_EVENT_HANDLER audioEvent(
  EventType::AUDIO_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CAudioEvent*>(e);

      return static_cast<CV8ResourceImpl*>(resource)->GetAudioHandlers(ev->GetTarget(), ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CAudioEvent*>(e);

      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8_EVENT_HANDLER rmlEvent(
  EventType::RMLUI_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CRmlEvent*>(e);
      return static_cast<CV8ResourceImpl*>(resource)->GetRmlHandlers(ev->GetElement(), ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CRmlEvent*>(e);
      args.push_back(resource->GetBaseObjectOrNull(ev->GetElement()));
      args.push_back(V8Helpers::MValueToV8(ev->GetArgs()));
  });

V8_EVENT_HANDLER keyboardEvent(
  EventType::KEYBOARD_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CKeyboardEvent*>(e);
      if(ev->GetKeyState() == alt::CKeyboardEvent::KeyState::UP) return resource->GetLocalHandlers("keyup");
      else if(ev->GetKeyState() == alt::CKeyboardEvent::KeyState::DOWN)
          return resource->GetLocalHandlers("keydown");
      else
      {
          Log::Error << "Unhandled keystate in keyboard event handler: " << (int)ev->GetKeyState() << Log::Endl;
          return std::vector<V8Helpers::EventCallback*>();
      }
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CKeyboardEvent*>(e);
      v8::Isolate* isolate = resource->GetIsolate();

      args.push_back(V8Helpers::JSValue(ev->GetKeyCode()));
  });

V8_LOCAL_EVENT_HANDLER
connectionComplete(EventType::CONNECTION_COMPLETE, "connectionComplete", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {});

V8_LOCAL_EVENT_HANDLER disconnect(EventType::DISCONNECT_EVENT, "disconnect", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {});

V8_LOCAL_EVENT_HANDLER spawned(EventType::SPAWNED, "spawned", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {});

V8_LOCAL_EVENT_HANDLER netOwnerChange(EventType::NETOWNER_CHANGE,
                                      "netOwnerChange",
                                      [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                      {
                                          auto ev = static_cast<const alt::CNetOwnerChangeEvent*>(e);

                                          args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                          args.push_back(resource->GetBaseObjectOrNull(ev->GetNewOwner()));
                                          args.push_back(resource->GetBaseObjectOrNull(ev->GetOldOwner()));
                                      });

V8_LOCAL_EVENT_HANDLER windowFocusChange(EventType::WINDOW_FOCUS_CHANGE,
                                         "windowFocusChange",
                                         [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                         {
                                             auto ev = static_cast<const alt::CWindowFocusChangeEvent*>(e);
                                             v8::Isolate* isolate = resource->GetIsolate();

                                             args.push_back(V8Helpers::JSValue(ev->GetState()));
                                         });

V8_LOCAL_EVENT_HANDLER
windowResolutionChange(EventType::WINDOW_RESOLUTION_CHANGE,
                       "windowResolutionChange",
                       [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                       {
                           auto ev = static_cast<const alt::CWindowResolutionChangeEvent*>(e);
                           v8::Isolate* isolate = resource->GetIsolate();

                           args.push_back(resource->CreateVector2(ev->GetOldResolution()));
                           args.push_back(resource->CreateVector2(ev->GetNewResolution()));
                       });
