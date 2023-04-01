#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CWorldObjectPositonChangeEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_LOCAL_EVENT_HANDLER worldObjectPositionChange(EventType::WORLD_OBJECT_POSITION_CHANGE,
                                                 "worldObjectPositionChange",
                                                 [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                                 {
                                                     auto ev = static_cast<const alt::CWorldObjectPositionChangeEvent*>(e);
                                                     v8::Isolate* isolate = resource->GetIsolate();

                                                     args.push_back(resource->GetOrCreateEntity(ev->GetWorldObject())->GetJSVal(isolate));
                                                     args.push_back(resource->CreateVector3(ev->GetOldPosition()));
                                                 });

V8_LOCAL_EVENT_HANDLER worldObjectStreamIn(EventType::WORLD_OBJECT_STREAM_IN,
                                           "worldObjectStreamIn",
                                           [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                           {
                                               auto ev = static_cast<const alt::CWorldObjectStreamInEvent*>(e);
                                               v8::Isolate* isolate = resource->GetIsolate();

                                               args.push_back(resource->GetOrCreateEntity(ev->GetWorldObject())->GetJSVal(isolate));
                                           });

V8_LOCAL_EVENT_HANDLER worldObjectStreamOut(EventType::WORLD_OBJECT_STREAM_OUT,
                                            "worldObjectStreamOut",
                                            [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                            {
                                                auto ev = static_cast<const alt::CWorldObjectStreamOutEvent*>(e);
                                                v8::Isolate* isolate = resource->GetIsolate();

                                                args.push_back(resource->GetOrCreateEntity(ev->GetWorldObject())->GetJSVal(isolate));
                                            });
