#include "stdafx.h"

#include "helpers/V8ResourceImpl.h"
#include "helpers/V8Helpers.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8::LocalEventHandler vehicleDestroy(
	EventType::VEHICLE_DESTROY,
	"vehicleDestroy",
	[](V8ResourceImpl* resource, const CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CVehicleDestroyEvent*>(e);
		args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get(), "Player")->GetJSVal());
	}
);