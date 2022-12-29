#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/events/CGameEntityCreateEvent.h"
#include "cpp-sdk/events/CGameEntityDestroyEvent.h"
#include "cpp-sdk/events/CTaskChangeEvent.h"
#include "cpp-sdk/events/CPlayerWeaponShootEvent.h"
#include "cpp-sdk/events/CPlayerWeaponChangeEvent.h"
#include "cpp-sdk/events/CWeaponDamageEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_EVENT_HANDLER gameEntityCreate(
  EventType::GAME_ENTITY_CREATE,
  [](V8ResourceImpl* resource, const alt::CEvent* e)
  {
      CV8ScriptRuntime::Instance().OnEntityStreamIn(static_cast<const alt::CGameEntityCreateEvent*>(e)->GetTarget());

      return resource->GetLocalHandlers("gameEntityCreate");
  },
  [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CGameEntityCreateEvent*>(e);
      v8::Isolate* isolate = resource->GetIsolate();

      args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
  });

V8_EVENT_HANDLER gameEntityDestroy(
  EventType::GAME_ENTITY_DESTROY,
  [](V8ResourceImpl* resource, const alt::CEvent* e)
  {
      CV8ScriptRuntime::Instance().OnEntityStreamOut(static_cast<const alt::CGameEntityDestroyEvent*>(e)->GetTarget());

      return resource->GetLocalHandlers("gameEntityDestroy");
  },
  [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
  {
      auto ev = static_cast<const alt::CGameEntityDestroyEvent*>(e);
      v8::Isolate* isolate = resource->GetIsolate();

      args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
  });

V8_LOCAL_EVENT_HANDLER taskChange(EventType::TASK_CHANGE,
                                  "taskChange",
                                  [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                  {
                                      auto ev = static_cast<const alt::CTaskChangeEvent*>(e);
                                      v8::Isolate* isolate = resource->GetIsolate();

                                      args.push_back(V8Helpers::JSValue(ev->GetOldTask()));
                                      args.push_back(V8Helpers::JSValue(ev->GetNewTask()));
                                  });

V8_LOCAL_EVENT_HANDLER playerWeaponShoot(EventType::PLAYER_WEAPON_SHOOT_EVENT,
                                         "playerWeaponShoot",
                                         [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                         {
                                             auto ev = static_cast<const alt::CPlayerWeaponShootEvent*>(e);
                                             v8::Isolate* isolate = resource->GetIsolate();

                                             args.push_back(V8Helpers::JSValue(ev->GetWeapon()));
                                             args.push_back(V8Helpers::JSValue(ev->GetTotalAmmo()));
                                             args.push_back(V8Helpers::JSValue(ev->GetAmmoInClip()));
                                         });

V8_LOCAL_EVENT_HANDLER playerWeaponChange(EventType::PLAYER_WEAPON_CHANGE,
                                          "playerWeaponChange",
                                          [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                          {
                                              auto ev = static_cast<const alt::CPlayerWeaponChangeEvent*>(e);
                                              v8::Isolate* isolate = resource->GetIsolate();

                                              args.push_back(V8Helpers::JSValue(ev->GetOldWeapon()));
                                              args.push_back(V8Helpers::JSValue(ev->GetNewWeapon()));
                                          });

V8_LOCAL_EVENT_HANDLER weaponDamage(EventType::WEAPON_DAMAGE_EVENT,
                                    "weaponDamage",
                                    [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                    {
                                        auto ev = static_cast<const alt::CWeaponDamageEvent*>(e);
                                        v8::Isolate* isolate = resource->GetIsolate();

                                        args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                        args.push_back(V8Helpers::JSValue(ev->GetWeaponHash()));
                                        args.push_back(V8Helpers::JSValue(ev->GetDamageValue()));
                                        args.push_back(resource->CreateVector3(ev->GetShotOffset()));
                                        args.push_back(V8Helpers::JSValue(static_cast<int8_t>(ev->GetBodyPart())));
                                    });
