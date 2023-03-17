#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CResourceStartEvent.h"
#include "cpp-sdk/events/CResourceStopEvent.h"
#include "cpp-sdk/events/CResourceErrorEvent.h"
#include "cpp-sdk/SDK.h"

using EventType = alt::CEvent::Type;

V8_LOCAL_EVENT_HANDLER anyResourceStart(EventType::RESOURCE_START,
                                        "anyResourceStart",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CResourceStartEvent*>(e);

                                            args.push_back(V8Helpers::JSValue(ev->GetResource()->GetName()));
                                        });

V8_EVENT_HANDLER anyResourceStop(
  EventType::RESOURCE_STOP,
  [](V8ResourceImpl* resource, const alt::CEvent* e)
  {
      auto ev = static_cast<const alt::CResourceStopEvent*>(e);
      for(alt::IResource* res : alt::ICore::Instance().GetAllResources())
      {
          if(res->GetType() != "js" || !res->IsStarted()) continue;
          static_cast<V8ResourceImpl*>(res->GetImpl())->DeleteResourceObject(resource->GetResource());
      }
      return resource->GetLocalHandlers("anyResourceStop");
  },
  [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CResourceStopEvent*>(e);

      args.push_back(V8Helpers::JSValue(ev->GetResource()->GetName()));
  });

V8_LOCAL_EVENT_HANDLER anyResourceError(EventType::RESOURCE_ERROR,
                                        "anyResourceError",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CResourceErrorEvent*>(e);
                                            v8::Isolate* isolate = resource->GetIsolate();

                                            args.push_back(V8Helpers::JSValue(ev->GetResource()->GetName()));
                                        });
