#include "stdafx.h"

#include "helpers/V8ResourceImpl.h"
#include "helpers/V8Helpers.h"

#include "cpp-sdk/events/CClientScriptEvent.h"
#include "cpp-sdk/events/CServerScriptEvent.h"
#include "cpp-sdk/events/CColShapeEvent.h"
#include "cpp-sdk/events/CRemoveEntityEvent.h"
#include "cpp-sdk/events/CWeaponDamageEvent.h"
#include "cpp-sdk/events/CExplosionEvent.h"
#include "cpp-sdk/events/CFireEvent.h"
#include "cpp-sdk/events/CStartProjectileEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8::EventHandler clientScriptEvent(
	EventType::CLIENT_SCRIPT_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CClientScriptEvent*>(e);
		return resource->GetRemoteHandlers(ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CClientScriptEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8::EventHandler serverScriptEvent(
	EventType::SERVER_SCRIPT_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CServerScriptEvent*>(e);
		return resource->GetLocalHandlers(ev->GetName().ToString());
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CServerScriptEvent*>(e);
		V8Helpers::MValueArgsToV8(ev->GetArgs(), args);
	}
);

V8::EventHandler colshapeEvent(
	EventType::COLSHAPE_EVENT,
	[](V8ResourceImpl* resource, const CEvent* e) {
		auto ev = static_cast<const alt::CColShapeEvent*>(e);

		if (ev->GetState())
			return resource->GetLocalHandlers("entityEnterColshape");
		else
			return resource->GetLocalHandlers("entityLeaveColshape");
	},
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CColShapeEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetEntity()));
	}
);

V8::LocalEventHandler removeEntity(
	EventType::REMOVE_ENTITY_EVENT,
	"removeEntity",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CRemoveEntityEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetEntity()));
	}
);

V8::LocalEventHandler weaponDamage(
	EventType::WEAPON_DAMAGE_EVENT,
	"weaponDamage",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CWeaponDamageEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetWeaponHash()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetDamageValue()));
		args.push_back(resource->CreateVector3(ev->GetShotOffset()));
		args.push_back(v8::Integer::New(isolate, static_cast<int8_t>(ev->GetBodyPart())));
	}
);

V8::LocalEventHandler explosionEvent(
	EventType::EXPLOSION_EVENT,
	"explosion",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CExplosionEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
		args.push_back(v8::Integer::New(isolate, static_cast<int8_t>(ev->GetExplosionType())));
		args.push_back(resource->CreateVector3(ev->GetPosition()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetExplosionFX()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
	}
);

V8::LocalEventHandler fireEvent(
	EventType::FIRE_EVENT,
	"startFire",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CFireEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		auto& fires = ev->GetFires();
		v8::Local<v8::Array> v8fires = v8::Array::New(isolate, fires.GetSize());

		for (int i = 0; i < fires.GetSize(); ++i)
		{
			v8::Local<v8::Object> v8fire = v8::Object::New(isolate);
			v8fire->Set(resource->GetContext(), V8::Fire_PosKey(isolate), resource->CreateVector3(fires[i].position));
			v8fire->Set(resource->GetContext(), V8::Fire_WeaponKey(isolate), v8::Integer::NewFromUnsigned(isolate, fires[i].weaponHash));

			v8fires->Set(resource->GetContext(), i, v8fire);
		}

		args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
		args.push_back(v8fires);
	}
);

V8::LocalEventHandler startProjectileEvent(
	EventType::START_PROJECTILE_EVENT,
	"startProjectile",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CStartProjectileEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(resource->GetBaseObjectOrNull(ev->GetSource()));
		args.push_back(resource->CreateVector3(ev->GetStartPosition()));
		args.push_back(resource->CreateVector3(ev->GetDirection()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetAmmoHash()));
		args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetWeaponHash()));
	}
);

V8::LocalEventHandler resourceStart(
	EventType::RESOURCE_START,
	"anyResourceStart",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CResourceStartEvent*>(e);
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetResource()->GetName().CStr()).ToLocalChecked());
	}
);

V8::LocalEventHandler resourceStop(
	EventType::RESOURCE_STOP,
	"anyResourceStop",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CResourceStopEvent*>(e);
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetResource()->GetName().CStr()).ToLocalChecked());
	}
);

V8::LocalEventHandler resourceError(
	EventType::RESOURCE_ERROR,
	"anyResourceError",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CResourceErrorEvent*>(e);
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetResource()->GetName().CStr()).ToLocalChecked());
	}
);

V8::LocalEventHandler syncedMetaChange(
	EventType::SYNCED_META_CHANGE,
	"syncedMetaChange",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CSyncedMetaDataChangeEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetKey().CStr()).ToLocalChecked());
		args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	}
);

V8::LocalEventHandler streamSyncedMetaChange(
	EventType::STREAM_SYNCED_META_CHANGE,
	"streamSyncedMetaChange",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CStreamSyncedMetaDataChangeEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetKey().CStr()).ToLocalChecked());
		args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	}
);

V8::LocalEventHandler globalMetaChange(
	EventType::GLOBAL_META_CHANGE,
	"globalMetaChange",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CGlobalMetaDataChangeEvent*>(e);

		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetKey().CStr()).ToLocalChecked());
		args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	}
);

V8::LocalEventHandler globalSyncedMetaChange(
	EventType::GLOBAL_SYNCED_META_CHANGE,
	"globalSyncedMetaChange",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent*>(e);

		args.push_back(v8::String::NewFromUtf8(resource->GetIsolate(), ev->GetKey().CStr()).ToLocalChecked());
		args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	}
);
