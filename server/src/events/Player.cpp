#include "stdafx.h"

#include "V8ResourceImpl.h"
#include "V8Helpers.h"

#include "cpp-sdk/events/CPlayerConnectEvent.h"
#include "cpp-sdk/events/CPlayerBeforeConnectEvent.h"
#include "cpp-sdk/events/CPlayerDisconnectEvent.h"
#include "cpp-sdk/events/CPlayerDamageEvent.h"
#include "cpp-sdk/events/CPlayerDeathEvent.h"
#include "cpp-sdk/events/CPlayerEnterVehicleEvent.h"
#include "cpp-sdk/events/CPlayerEnteringVehicleEvent.h"
#include "cpp-sdk/events/CPlayerLeaveVehicleEvent.h"
#include "cpp-sdk/events/CPlayerChangeVehicleSeatEvent.h"
#include "cpp-sdk/events/CPlayerWeaponChangeEvent.h"
#include "cpp-sdk/events/CLocalMetaDataChangeEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8Helpers::LocalEventHandler playerConnect(EventType::PLAYER_CONNECT, "playerConnect", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CPlayerConnectEvent*>(e);
    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
});

V8Helpers::LocalEventHandler
  beforePlayerConnect(EventType::PLAYER_BEFORE_CONNECT, "beforePlayerConnect", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
      auto ev = static_cast<const alt::CPlayerBeforeConnectEvent*>(e);
      const alt::ConnectionInfo& info = ev->GetConnectionInfo();
      v8::Isolate* isolate = resource->GetIsolate();
      v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
      V8_NEW_OBJECT(infoObj);
      V8_OBJECT_SET_STD_STRING(infoObj, "name", info.name);
      V8_OBJECT_SET_STD_STRING(infoObj, "socialID", std::to_string(info.socialId));
      V8_OBJECT_SET_STD_STRING(infoObj, "hwidHash", std::to_string(info.hwidHash));
      V8_OBJECT_SET_STD_STRING(infoObj, "hwidExHash", std::to_string(info.hwidExHash));
      V8_OBJECT_SET_STD_STRING(infoObj, "authToken", info.authToken);
      V8_OBJECT_SET_BOOLEAN(infoObj, "isDebug", info.isDebug);
      V8_OBJECT_SET_STD_STRING(infoObj, "branch", info.branch);
      V8_OBJECT_SET_UINT(infoObj, "build", info.build);
      V8_OBJECT_SET_STD_STRING(infoObj, "cdnUrl", info.cdnUrl);
      V8_OBJECT_SET_BIGUINT(infoObj, "passwordHash", info.passwordHash);
      args.push_back(infoObj);
      args.push_back(V8Helpers::JSValue(ev->GetReason()));
  });

V8Helpers::LocalEventHandler playerDisconnect(EventType::PLAYER_DISCONNECT, "playerDisconnect", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CPlayerDisconnectEvent*>(e);

    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
    args.push_back(V8Helpers::JSValue(ev->GetReason()));
});

V8Helpers::LocalEventHandler playerDamage(EventType::PLAYER_DAMAGE, "playerDamage", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CPlayerDamageEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
    args.push_back(resource->GetBaseObjectOrNull(ev->GetAttacker()));
    args.push_back(V8Helpers::JSValue(ev->GetHealthDamage()));
    args.push_back(V8Helpers::JSValue(ev->GetArmourDamage()));
    args.push_back(V8Helpers::JSValue(ev->GetWeapon()));
});

V8Helpers::LocalEventHandler playerDeath(EventType::PLAYER_DEATH, "playerDeath", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CPlayerDeathEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
    args.push_back(resource->GetBaseObjectOrNull(ev->GetKiller()));
    args.push_back(V8Helpers::JSValue(ev->GetWeapon()));
});

V8Helpers::LocalEventHandler playerEnterVehicle(EventType::PLAYER_ENTER_VEHICLE,
                                                "playerEnteredVehicle",  // TODO: change name for consistency
                                                [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
                                                    auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(e);

                                                    args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
                                                    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                    args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                                });

V8Helpers::LocalEventHandler playerEnteringVehicle(EventType::PLAYER_ENTERING_VEHICLE,
                                                   "playerEnteringVehicle",  // TODO: don't change names, it's okay
                                                   [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
                                                       auto ev = static_cast<const alt::CPlayerEnteringVehicleEvent*>(e);

                                                       args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
                                                       args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                       args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                                   });

V8Helpers::LocalEventHandler playerLeaveVehicle(EventType::PLAYER_LEAVE_VEHICLE,
                                                "playerLeftVehicle",  // TODO: change name for consistency
                                                [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
                                                    auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(e);

                                                    args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
                                                    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                    args.push_back(V8Helpers::JSValue(ev->GetSeat()));
                                                });

V8Helpers::LocalEventHandler playerChangeVehicleSeat(EventType::PLAYER_CHANGE_VEHICLE_SEAT,
                                                     "playerChangedVehicleSeat",  // TODO: change name for consistency
                                                     [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
                                                         auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(e);

                                                         args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
                                                         args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
                                                         args.push_back(V8Helpers::JSValue(ev->GetOldSeat()));
                                                         args.push_back(V8Helpers::JSValue(ev->GetNewSeat()));
                                                     });

V8Helpers::LocalEventHandler
  playerWeaponChange(EventType::PLAYER_WEAPON_CHANGE, "playerWeaponChange", [](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
      auto ev = static_cast<const alt::CPlayerWeaponChangeEvent*>(e);

      args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
      args.push_back(V8Helpers::JSValue(ev->GetOldWeapon()));
      args.push_back(V8Helpers::JSValue(ev->GetNewWeapon()));
  });

V8_LOCAL_EVENT_HANDLER localMetaChange(EventType::LOCAL_SYNCED_META_CHANGE, "localMetaChange", [](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
    auto ev = static_cast<const alt::CLocalMetaDataChangeEvent*>(e);
    v8::Isolate* isolate = resource->GetIsolate();

    args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
    args.push_back(V8Helpers::JSValue(ev->GetKey().CStr()));
    args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
    args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
});
