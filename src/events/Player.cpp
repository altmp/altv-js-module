#include "stdafx.h"

#include "helpers/V8ResourceImpl.h"
#include "helpers/V8Helpers.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8::LocalEventHandler playerConnect(
	EventType::PLAYER_CONNECT,
	"playerConnect",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerConnectEvent*>(e);
		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Player")->GetJSVal());
	}
);

V8::LocalEventHandler playerDisconnect(
	EventType::PLAYER_DISCONNECT,
	"playerDisconnect",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerDisconnectEvent*>(e);

		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Player")->GetJSVal());
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetReason().CStr()));
	}
);

V8::LocalEventHandler playerDamage(
	EventType::PLAYER_DAMAGE,
	"playerDamage",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerDamageEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Player")->GetJSVal());

		if (ev->GetAttacker())
			args.push_back(resource->GetOrCreateEntity(ev->GetAttacker().Get(), "Entity")->GetJSVal());
		else
			args.push_back(v8::Null(isolate));

		args.push_back(v8::Integer::New(isolate, ev->GetDamage()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetWeapon()));
	}
);

V8::LocalEventHandler playerDeath(
	EventType::PLAYER_DEATH,
	"playerDeath",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerDeathEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Player")->GetJSVal());

		if (ev->GetKiller())
			args.push_back(resource->GetOrCreateEntity(ev->GetKiller().Get(), "Entity")->GetJSVal());
		else
			args.push_back(v8::Null(isolate));

		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetWeapon()));
	}
);

V8::LocalEventHandler playerEnterVehicle(
	EventType::PLAYER_ENTER_VEHICLE,
	"playerEnteredVehicle", // TODO: change name for consistency
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(e);

		args.push_back(resource->GetOrCreateEntity(ev->GetPlayer().Get(), "Player")->GetJSVal());
		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Vehicle")->GetJSVal());
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetSeat()));
	}
);

V8::LocalEventHandler playerLeaveVehicle(
	EventType::PLAYER_LEAVE_VEHICLE,
	"playerLeftVehicle", // TODO: change name for consistency
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(e);

		args.push_back(resource->GetOrCreateEntity(ev->GetPlayer().Get(), "Player")->GetJSVal());
		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Vehicle")->GetJSVal());
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetSeat()));
	}
);

V8::LocalEventHandler playerChangeVehicleSeat(
	EventType::PLAYER_CHANGE_VEHICLE_SEAT,
	"playerChangedVehicleSeat", // TODO: change name for consistency
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(e);

		args.push_back(resource->GetOrCreateEntity(ev->GetPlayer().Get(), "Player")->GetJSVal());
		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Vehicle")->GetJSVal());
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetOldSeat()));
		args.push_back(v8::Integer::New(resource->GetIsolate(), ev->GetNewSeat()));
	}
);
