#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CStreamSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CGlobalSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CGlobalMetaDataChangeEvent.h"
#include "cpp-sdk/events/CLocalMetaDataChangeEvent.h"
#include "cpp-sdk/events/CMetaDataChangeEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_LOCAL_EVENT_HANDLER syncedMetaChange(EventType::SYNCED_META_CHANGE,
                                        "syncedMetaChange",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CSyncedMetaDataChangeEvent*>(e);
                                            v8::Isolate* isolate = resource->GetIsolate();

                                            args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                                            args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                            args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                            args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                        });

V8_LOCAL_EVENT_HANDLER
streamSyncedMetaChange(EventType::STREAM_SYNCED_META_CHANGE,
                       "streamSyncedMetaChange",
                       [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                       {
                           auto ev = static_cast<const alt::CStreamSyncedMetaDataChangeEvent*>(e);
                           v8::Isolate* isolate = resource->GetIsolate();

                           args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                           args.push_back(V8Helpers::JSValue(ev->GetKey()));
                           args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                           args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                       });

V8_LOCAL_EVENT_HANDLER
globalSyncedMetaChange(EventType::GLOBAL_SYNCED_META_CHANGE,
                       "globalSyncedMetaChange",
                       [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                       {
                           auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent*>(e);
                           v8::Isolate* isolate = resource->GetIsolate();

                           args.push_back(V8Helpers::JSValue(ev->GetKey()));
                           args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                           args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                       });

V8_LOCAL_EVENT_HANDLER globalMetaChange(EventType::GLOBAL_META_CHANGE,
                                        "globalMetaChange",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CGlobalMetaDataChangeEvent*>(e);
                                            v8::Isolate* isolate = resource->GetIsolate();

                                            args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                            args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                            args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                        });

V8_LOCAL_EVENT_HANDLER localMetaChange(EventType::LOCAL_SYNCED_META_CHANGE,
                                       "localMetaChange",
                                       [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                       {
                                           auto ev = static_cast<const alt::CLocalMetaDataChangeEvent*>(e);
                                           v8::Isolate* isolate = resource->GetIsolate();

                                           args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                           args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                           args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                       });

V8_LOCAL_EVENT_HANDLER metaChange(EventType::META_CHANGE,
                                  "metaChange",
                                  [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                  {
                                      auto ev = static_cast<const alt::CMetaChangeEvent*>(e);
                                      v8::Isolate* isolate = resource->GetIsolate();

                                      args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                                      args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                      args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                      args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                  });
