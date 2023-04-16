
#include "../V8ResourceImpl.h"
#include "../V8Helpers.h"

#include "cpp-sdk/events/CConsoleCommandEvent.h"
#include "cpp-sdk/events/CCreateBaseObjectEvent.h"
#include "cpp-sdk/events/CRemoveBaseObjectEvent.h"

using EventType = alt::CEvent::Type;

V8_LOCAL_EVENT_HANDLER consoleCommand(EventType::CONSOLE_COMMAND_EVENT,
                                      "consoleCommand",
                                      [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                      {
                                          auto ev = static_cast<const alt::CConsoleCommandEvent*>(e);
                                          v8::Isolate* isolate = resource->GetIsolate();

                                          args.push_back(V8Helpers::JSValue(ev->GetName()));
                                          for(auto& arg : ev->GetArgs()) args.push_back(V8Helpers::JSValue(arg));
                                      });

V8_LOCAL_EVENT_HANDLER baseObjectCreate(EventType::CREATE_BASE_OBJECT_EVENT,
                                        "baseObjectCreate",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CCreateBaseObjectEvent*>(e);
                                            v8::Isolate* isolate = resource->GetIsolate();

                                            args.push_back(resource->GetBaseObjectOrNull(ev->GetObject()));
                                        });

V8_LOCAL_EVENT_HANDLER baseObjectRemove(EventType::REMOVE_BASE_OBJECT_EVENT,
                                        "baseObjectRemove",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CRemoveBaseObjectEvent*>(e);
                                            v8::Isolate* isolate = resource->GetIsolate();

                                            args.push_back(resource->GetBaseObjectOrNull(ev->GetObject()));
                                        });

V8_EVENT_HANDLER colshapeEvent(
  EventType::COLSHAPE_EVENT,
  [](V8ResourceImpl* resource, const alt::CEvent* e)
  {
      auto ev = static_cast<const alt::CColShapeEvent*>(e);

      if(ev->GetState()) return resource->GetLocalHandlers("entityEnterColshape");
      else
          return resource->GetLocalHandlers("entityLeaveColshape");
  },
  [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CColShapeEvent*>(e);

      args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
      args.push_back(resource->GetBaseObjectOrNull(ev->GetEntity()));
  });
