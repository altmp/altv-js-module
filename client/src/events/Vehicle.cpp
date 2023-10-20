#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CPlayerEnterVehicleEvent.h"
#include "cpp-sdk/events/CPlayerLeaveVehicleEvent.h"
#include "cpp-sdk/events/CPlayerChangeVehicleSeatEvent.h"
#include "cpp-sdk/events/CPlayerStartEnterVehicleEvent.h"
#include "cpp-sdk/events/CPlayerStartLeaveVehicleEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_LOCAL_EVENT_HANDLER enteredVehicle(EventType::PLAYER_ENTER_VEHICLE,
                                      "enteredVehicle",
                                      [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                      {
                                          auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(e);
                                          v8::Isolate* isolate = resource->GetIsolate();

                                          args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                                          args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                      });

V8_LOCAL_EVENT_HANDLER leftVehicle(EventType::PLAYER_LEAVE_VEHICLE,
                                   "leftVehicle",
                                   [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                   {
                                       auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(e);
                                       v8::Isolate* isolate = resource->GetIsolate();

                                       args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                                       args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                   });

V8_LOCAL_EVENT_HANDLER enterVehicle(EventType::PLAYER_START_ENTER_VEHICLE,
                                      "startEnteringVehicle",
                                      [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                      {
                                          auto ev = static_cast<const alt::CPlayerStartEnterVehicleEvent*>(e);
                                          v8::Isolate* isolate = resource->GetIsolate();

                                          args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                                          args.push_back(V8Helpers::JSValue(ev->GetSeat()));

                                          // TODO (xLuxy): Maybe move up? Would cause breaking change
                                          args.push_back(resource->GetOrCreateEntity(ev->GetPlayer())->GetJSVal(isolate));
                                      });

V8_LOCAL_EVENT_HANDLER leaveVehicle(EventType::PLAYER_START_LEAVE_VEHICLE,
                                   "startLeavingVehicle",
                                   [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                   {
                                       auto ev = static_cast<const alt::CPlayerStartLeaveVehicleEvent*>(e);
                                       v8::Isolate* isolate = resource->GetIsolate();

                                       args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                                       args.push_back(V8Helpers::JSValue(ev->GetSeat()));

                                       // TODO (xLuxy): Maybe move up? Would cause breaking change
                                       args.push_back(resource->GetOrCreateEntity(ev->GetPlayer())->GetJSVal(isolate));
                                   });

V8_LOCAL_EVENT_HANDLER
changedVehicleSeat(EventType::PLAYER_CHANGE_VEHICLE_SEAT,
                   "changedVehicleSeat",
                   [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                   {
                       auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(e);
                       v8::Isolate* isolate = resource->GetIsolate();

                       args.push_back(resource->GetOrCreateEntity(ev->GetTarget())->GetJSVal(isolate));
                       args.push_back(V8Helpers::JSValue(ev->GetOldSeat()));
                       args.push_back(V8Helpers::JSValue(ev->GetNewSeat()));
                   });
