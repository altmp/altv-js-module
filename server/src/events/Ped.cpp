#include "stdafx.h"

#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "CNodeResourceImpl.h"

#include "cpp-sdk/events/CPedDamageEvent.h"
#include "cpp-sdk/events/CPedDeathEvent.h"
#include "cpp-sdk/events/CPedHealEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

extern V8Class v8ConnectionInfo;

V8Helpers::LocalEventHandler pedDamage(EventType::PED_DAMAGE,
                                          "pedDamage",
                                          [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                          {
                                              auto ev = static_cast<const alt::CPedDamageEvent*>(e);
                                              v8::Isolate* isolate = resource->GetIsolate();

                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetAttacker()));
                                              args.push_back(V8Helpers::JSValue(ev->GetHealthDamage()));
                                              args.push_back(V8Helpers::JSValue(ev->GetArmourDamage()));
                                              args.push_back(V8Helpers::JSValue(ev->GetWeapon()));
                                          });

V8Helpers::LocalEventHandler pedDeath(EventType::PED_DEATH,
                                         "pedDeath",
                                         [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                         {
                                             auto ev = static_cast<const alt::CPedDeathEvent*>(e);
                                             v8::Isolate* isolate = resource->GetIsolate();

                                             args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                             args.push_back(resource->GetBaseObjectOrNull(ev->GetKiller()));
                                             args.push_back(V8Helpers::JSValue(ev->GetWeapon()));
                                         });

V8Helpers::LocalEventHandler pedHeal(EventType::PED_HEAL,
                                  "pedHeal",
                                  [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                  {
                                      auto ev = static_cast<const alt::CPedHealEvent*>(e);
                                      v8::Isolate* isolate = resource->GetIsolate();

                                      args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                      args.push_back(V8Helpers::JSValue(ev->GetOldHealth()));
                                      args.push_back(V8Helpers::JSValue(ev->GetNewHealth()));
                                      args.push_back(V8Helpers::JSValue(ev->GetOldArmour()));
                                      args.push_back(V8Helpers::JSValue(ev->GetNewArmour()));
                                  });
