#include "stdafx.h"

#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CClientScriptEvent.h"
#include "cpp-sdk/events/CServerScriptEvent.h"
#include "cpp-sdk/events/CColShapeEvent.h"
#include "cpp-sdk/events/CWeaponDamageEvent.h"
#include "cpp-sdk/events/CExplosionEvent.h"
#include "cpp-sdk/events/CFireEvent.h"
#include "cpp-sdk/events/CStartProjectileEvent.h"
#include "cpp-sdk/events/CMetaDataChangeEvent.h"
#include "cpp-sdk/events/CClientDeleteObjectEvent.h"
#include "cpp-sdk/events/CClientRequestObjectEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8Helpers::EventHandler clientScriptEvent(
  EventType::CLIENT_SCRIPT_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CClientScriptEvent*>(e);
      return resource->GetRemoteHandlers(ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CClientScriptEvent*>(e);

      args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8Helpers::EventHandler serverScriptEvent(
  EventType::SERVER_SCRIPT_EVENT,
  [](V8ResourceImpl* resource, const CEvent* e)
  {
      auto ev = static_cast<const alt::CServerScriptEvent*>(e);
      return resource->GetLocalHandlers(ev->GetName());
  },
  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CServerScriptEvent*>(e);
      V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
  });

V8Helpers::LocalEventHandler weaponDamage(EventType::WEAPON_DAMAGE_EVENT,
                                          "weaponDamage",
                                          [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                          {
                                              auto ev = static_cast<const alt::CWeaponDamageEvent*>(e);
                                              v8::Isolate* isolate = resource->GetIsolate();

                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                              args.push_back(V8Helpers::JSValue(ev->GetWeaponHash()));
                                              args.push_back(V8Helpers::JSValue(ev->GetDamageValue()));
                                              args.push_back(resource->CreateVector3(ev->GetShotOffset()));
                                              args.push_back(V8Helpers::JSValue(static_cast<int8_t>(ev->GetBodyPart())));
                                          });

V8Helpers::LocalEventHandler explosionEvent(EventType::EXPLOSION_EVENT,
                                            "explosion",
                                            [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                            {
                                                auto ev = static_cast<const alt::CExplosionEvent*>(e);
                                                v8::Isolate* isolate = resource->GetIsolate();

                                                args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                                args.push_back(V8Helpers::JSValue(static_cast<int8_t>(ev->GetExplosionType())));
                                                args.push_back(resource->CreateVector3(ev->GetPosition()));
                                                args.push_back(V8Helpers::JSValue(ev->GetExplosionFX()));
                                                args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                            });

V8Helpers::LocalEventHandler fireEvent(EventType::FIRE_EVENT,
                                       "startFire",
                                       [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                       {
                                           auto ev = static_cast<const alt::CFireEvent*>(e);
                                           v8::Isolate* isolate = resource->GetIsolate();

                                           auto& fires = ev->GetFires();
                                           v8::Local<v8::Array> v8fires = v8::Array::New(isolate, fires.size());

                                           for(int i = 0; i < fires.size(); ++i)
                                           {
                                               v8::Local<v8::Object> v8fire = v8::Object::New(isolate);
                                               v8fire->Set(resource->GetContext(), V8Helpers::Fire_PosKey(isolate), resource->CreateVector3(fires[i].position));
                                               v8fire->Set(resource->GetContext(), V8Helpers::Fire_WeaponKey(isolate), V8Helpers::JSValue(fires[i].weaponHash));

                                               v8fires->Set(resource->GetContext(), i, v8fire);
                                           }

                                           args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                           args.push_back(v8fires);
                                       });

V8Helpers::LocalEventHandler startProjectileEvent(EventType::START_PROJECTILE_EVENT,
                                                  "startProjectile",
                                                  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                                  {
                                                      auto ev = static_cast<const alt::CStartProjectileEvent*>(e);
                                                      v8::Isolate* isolate = resource->GetIsolate();

                                                      args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                                      args.push_back(resource->CreateVector3(ev->GetStartPosition()));
                                                      args.push_back(resource->CreateVector3(ev->GetDirection()));
                                                      args.push_back(V8Helpers::JSValue(ev->GetAmmoHash()));
                                                      args.push_back(V8Helpers::JSValue(ev->GetWeaponHash()));
                                                  });

V8Helpers::LocalEventHandler syncedMetaChange(EventType::SYNCED_META_CHANGE,
                                              "syncedMetaChange",
                                              [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                              {
                                                  auto ev = static_cast<const alt::CSyncedMetaDataChangeEvent*>(e);

                                                  args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                  args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                                  args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                                  args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                              });

V8Helpers::LocalEventHandler streamSyncedMetaChange(EventType::STREAM_SYNCED_META_CHANGE,
                                                    "streamSyncedMetaChange",
                                                    [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                                    {
                                                        auto ev = static_cast<const alt::CStreamSyncedMetaDataChangeEvent*>(e);

                                                        args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                        args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                                        args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                                        args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                                    });

V8Helpers::LocalEventHandler globalMetaChange(EventType::GLOBAL_META_CHANGE,
                                              "globalMetaChange",
                                              [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                              {
                                                  auto ev = static_cast<const alt::CGlobalMetaDataChangeEvent*>(e);

                                                  args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                                  args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                                  args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                              });

V8Helpers::LocalEventHandler globalSyncedMetaChange(EventType::GLOBAL_SYNCED_META_CHANGE,
                                                    "globalSyncedMetaChange",
                                                    [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                                    {
                                                        auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent*>(e);

                                                        args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                                        args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                                        args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                                    });

V8Helpers::LocalEventHandler metaChange(EventType::META_CHANGE,
                                        "metaChange",
                                        [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CMetaChangeEvent*>(e);

                                            args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                            args.push_back(V8Helpers::JSValue(ev->GetKey()));
                                            args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
                                            args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
                                        });

V8Helpers::LocalEventHandler serverStarted(EventType::SERVER_STARTED, "serverStarted", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {});

V8_LOCAL_EVENT_HANDLER requestSyncedScene(EventType::REQUEST_SYNCED_SCENE,
                                          "requestSyncedScene",
                                          [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                          {
                                              auto ev = static_cast<const alt::CRequestSyncedSceneEvent*>(e);
                                              v8::Isolate* isolate = resource->GetIsolate();

                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                              args.push_back(V8Helpers::JSValue(ev->GetSceneID()));
                                          });

V8_LOCAL_EVENT_HANDLER startSyncedScene(EventType::START_SYNCED_SCENE,
                                        "startSyncedScene",
                                        [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                        {
                                            auto ev = static_cast<const alt::CStartSyncedSceneEvent*>(e);
                                            v8::Isolate* isolate = resource->GetIsolate();
                                            const auto ctx = resource->GetContext();

                                            args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                            args.push_back(V8Helpers::JSValue(ev->GetSceneID()));
                                            args.push_back(resource->CreateVector3(ev->GetStartPosition()));
                                            args.push_back(resource->CreateVector3(ev->GetStartRotation()));
                                            args.push_back(V8Helpers::JSValue(ev->GetAnimDictHash()));

                                            const auto entityAndAnimHashPairs = ev->GetEntityAndAnimHashPairs();
                                            const auto animHashArray = v8::Array::New(isolate, entityAndAnimHashPairs.size());
                                            auto idx = 0;
                                            for(auto [entity, animHash] : entityAndAnimHashPairs)
                                            {
                                                V8_NEW_OBJECT(entityAnimPair)
                                                V8_OBJECT_SET_BASE_OBJECT(entityAnimPair, "entity", entity.get())
                                                V8_OBJECT_SET_UINT(entityAnimPair, "animHash", animHash)
                                                animHashArray->Set(ctx, idx++, entityAnimPair);
                                            }
                                            args.push_back(animHashArray);
                                        });

V8_LOCAL_EVENT_HANDLER stopSyncedScene(EventType::STOP_SYNCED_SCENE,
                                       "stopSyncedScene",
                                       [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                       {
                                           auto ev = static_cast<const alt::CStopSyncedSceneEvent*>(e);
                                           v8::Isolate* isolate = resource->GetIsolate();

                                           args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                           args.push_back(V8Helpers::JSValue(ev->GetSceneID()));
                                       });

V8_LOCAL_EVENT_HANDLER updateSyncedScene(EventType::UPDATE_SYNCED_SCENE,
                                         "updateSyncedScene",
                                         [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                         {
                                             auto ev = static_cast<const alt::CUpdateSyncedSceneEvent*>(e);
                                             v8::Isolate* isolate = resource->GetIsolate();

                                             args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
                                             args.push_back(V8Helpers::JSValue(ev->GetStartRate()));
                                             args.push_back(V8Helpers::JSValue(ev->GetSceneID()));
                                         });

V8_LOCAL_EVENT_HANDLER clientDeleteObject(EventType::CLIENT_DELETE_OBJECT_EVENT,
                                          "clientDeleteObject",
                                          [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                          {
                                              auto ev = static_cast<const alt::CClientDeleteObjectEvent*>(e);
                                              v8::Isolate* isolate = resource->GetIsolate();

                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                          });

V8_LOCAL_EVENT_HANDLER clientRequestObject(EventType::CLIENT_REQUEST_OBJECT_EVENT,
                                           "clientRequestObject",
                                           [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                           {
                                               auto ev = static_cast<const alt::CClientRequestObjectEvent*>(e);
                                               v8::Isolate* isolate = resource->GetIsolate();

                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                               args.push_back(V8Helpers::JSValue(ev->GetModel()));
                                               args.push_back(resource->CreateVector3(ev->GetPosition()));
                                           });
