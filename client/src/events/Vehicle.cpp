#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CPlayerEnterVehicleEvent.h"
#include "cpp-sdk/events/CPlayerLeaveVehicleEvent.h"
#include "cpp-sdk/events/CPlayerChangeVehicleSeatEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8_LOCAL_EVENT_HANDLER enteredVehicle(EventType::PLAYER_ENTER_VEHICLE,
                                      "enteredVehicle",
                                      [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                      {
                                          auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(e);
                                          v8::Isolate* isolate = resource->GetIsolate();

                                          args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                          args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                      });

V8_LOCAL_EVENT_HANDLER leftVehicle(EventType::PLAYER_LEAVE_VEHICLE,
                                   "leftVehicle",
                                   [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                                   {
                                       auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(e);
                                       v8::Isolate* isolate = resource->GetIsolate();

                                       args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                       args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                   });

V8_LOCAL_EVENT_HANDLER
changedVehicleSeat(EventType::PLAYER_CHANGE_VEHICLE_SEAT,
                   "changedVehicleSeat",
                   [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args)
                   {
                       auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(e);
                       v8::Isolate* isolate = resource->GetIsolate();

                       args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                       args.push_back(V8Helpers::JSValue(ev->GetOldSeat()));
                       args.push_back(V8Helpers::JSValue(ev->GetNewSeat()));
                   });
