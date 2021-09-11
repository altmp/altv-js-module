#include "stdafx.h"

#include "helpers/V8ResourceImpl.h"
#include "helpers/V8Helpers.h"

#include "cpp-sdk/events/CPlayerConnectEvent.h"
#include "cpp-sdk/events/CPlayerDisconnectEvent.h"
#include "cpp-sdk/events/CPlayerDamageEvent.h"
#include "cpp-sdk/events/CPlayerDeathEvent.h"
#include "cpp-sdk/events/CPlayerEnterVehicleEvent.h"
#include "cpp-sdk/events/CPlayerEnteringVehicleEvent.h"
#include "cpp-sdk/events/CPlayerLeaveVehicleEvent.h"
#include "cpp-sdk/events/CPlayerChangeVehicleSeatEvent.h"
#include "cpp-sdk/events/CPlayerWeaponChangeEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8::LocalEventHandler playerConnect(
	EventType::PLAYER_CONNECT,
	"playerConnect",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerConnectEvent*>(e);
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
	}
);

V8::LocalEventHandler playerDisconnect(
	EventType::PLAYER_DISCONNECT,
	"playerDisconnect",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerDisconnectEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetReason().CStr()).ToLocalChecked());
	}
);

V8::LocalEventHandler playerDamage(
	EventType::PLAYER_DAMAGE,
	"playerDamage",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerDamageEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetAttacker()));
		args.push_back(v8::Integer::New(isolate, ev->GetHealthDamage()));
		args.push_back(v8::Integer::New(isolate, ev->GetArmourDamage()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetWeapon()));
	}
);

V8::LocalEventHandler playerDeath(
	EventType::PLAYER_DEATH,
	"playerDeath",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerDeathEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetKiller()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetWeapon()));
	}
);

V8::LocalEventHandler playerEnterVehicle(
	EventType::PLAYER_ENTER_VEHICLE,
	"playerEnteredVehicle", // TODO: change name for consistency
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetSeat()));
	}
);

V8::LocalEventHandler playerEnteringVehicle(
	EventType::PLAYER_ENTERING_VEHICLE,
	"playerEnteringVehicle", // TODO: don't change names, it's okay
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerEnteringVehicleEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetSeat()));
	}
);

V8::LocalEventHandler playerLeaveVehicle(
	EventType::PLAYER_LEAVE_VEHICLE,
	"playerLeftVehicle", // TODO: change name for consistency
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetSeat()));
	}
);

V8::LocalEventHandler playerChangeVehicleSeat(
	EventType::PLAYER_CHANGE_VEHICLE_SEAT,
	"playerChangedVehicleSeat", // TODO: change name for consistency
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetOldSeat()));
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetNewSeat()));
	}
);

V8::LocalEventHandler playerWeaponChange(
	EventType::PLAYER_WEAPON_CHANGE,
	"playerWeaponChange",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerWeaponChangeEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::Integer::NewFromUnsigned(resource->GetIsolate(), ev->GetOldWeapon()));
		args.push_back(v8::Integer::NewFromUnsigned(resource->GetIsolate(), ev->GetNewWeapon()));
	}
);
