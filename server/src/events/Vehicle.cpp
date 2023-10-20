#include "stdafx.h"

#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CVehicleAttachEvent.h"
#include "cpp-sdk/events/CVehicleDetachEvent.h"
#include "cpp-sdk/events/CNetOwnerChangeEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8Helpers::LocalEventHandler vehicleDestroy(EventType::VEHICLE_DESTROY,
                                            "vehicleDestroy",
                                            [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                            {
                                                auto ev = static_cast<const alt::CVehicleDestroyEvent*>(e);
                                                args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                            });

V8Helpers::LocalEventHandler vehicleAttach(EventType::VEHICLE_ATTACH,
                                           "vehicleAttach",
                                           [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                           {
                                               auto ev = static_cast<const alt::CVehicleAttachEvent*>(e);

                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetAttached()));
                                           });

V8Helpers::LocalEventHandler vehicleDetach(EventType::VEHICLE_DETACH,
                                           "vehicleDetach",
                                           [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                           {
                                               auto ev = static_cast<const alt::CVehicleDetachEvent*>(e);

                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetDetached()));
                                           });

V8Helpers::LocalEventHandler netOwnerChange(EventType::NETOWNER_CHANGE,
                                            "netOwnerChange",
                                            [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                            {
                                                auto ev = static_cast<const alt::CNetOwnerChangeEvent*>(e);

                                                args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                args.push_back(resource->GetBaseObjectOrNull(ev->GetNewOwner()));
                                                args.push_back(resource->GetBaseObjectOrNull(ev->GetOldOwner()));
                                            });

V8Helpers::LocalEventHandler vehicleDamage(EventType::VEHICLE_DAMAGE,
                                           "vehicleDamage",
                                           [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                           {
                                               auto ev = static_cast<const alt::CVehicleDamageEvent*>(e);
                                               auto isolate = resource->GetIsolate();

                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                               args.push_back(resource->GetBaseObjectOrNull(ev->GetDamager()));
                                               args.push_back(V8Helpers::JSValue(ev->GetBodyHealthDamage()));
                                               args.push_back(V8Helpers::JSValue(ev->GetBodyAdditionalHealthDamage()));
                                               args.push_back(V8Helpers::JSValue(ev->GetEngineHealthDamage()));
                                               args.push_back(V8Helpers::JSValue(ev->GetPetrolTankHealthDamage()));
                                               args.push_back(V8Helpers::JSValue(ev->GetDamagedWith()));
                                           });

V8Helpers::LocalEventHandler vehicleHorn(EventType::VEHICLE_HORN,
                                         "vehicleHorn",
                                         [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                         {
                                             auto ev = static_cast<const alt::CVehicleHornEvent*>(e);

                                             args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                             args.push_back(resource->GetBaseObjectOrNull(ev->GetReporter()));
                                             args.push_back(V8Helpers::JSValue(ev->GetToggle()));
                                         });

V8Helpers::LocalEventHandler vehicleSiren(EventType::VEHICLE_SIREN,
                                          "vehicleSiren",
                                          [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                          {
                                              auto ev = static_cast<const alt::CVehicleSirenEvent*>(e);

                                              args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                              args.push_back(V8Helpers::JSValue(ev->GetToggle()));
                                          });
