#include "stdafx.h"

#include "helpers/V8ResourceImpl.h"
#include "helpers/V8Helpers.h"

#include "cpp-sdk/events/CVehicleAttachEvent.h"
#include "cpp-sdk/events/CVehicleDetachEvent.h"
#include "cpp-sdk/events/CNetOwnerChangeEvent.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8::LocalEventHandler vehicleDestroy(
	EventType::VEHICLE_DESTROY,
	"vehicleDestroy",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CVehicleDestroyEvent*>(e);
		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
	}
);

V8::LocalEventHandler vehicleAttach(
	EventType::VEHICLE_ATTACH,
	"vehicleAttach",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CVehicleAttachEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetAttached()));
	}
);

V8::LocalEventHandler vehicleDetach(
	EventType::VEHICLE_DETACH,
	"vehicleDetach",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CVehicleDetachEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetDetached()));
	}
);

V8::LocalEventHandler netOwnerChange(
	EventType::NETOWNER_CHANGE,
	"netOwnerChange",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CNetOwnerChangeEvent*>(e);

		args.push_back(resource->GetBaseObjectOrNull(ev->GetTarget()));
		args.push_back(resource->GetBaseObjectOrNull(ev->GetPlayer()));
	}
);