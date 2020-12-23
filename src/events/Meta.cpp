#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Helpers.h"

#include "cpp-sdk/events/CSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CStreamSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CGlobalSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CGlobalMetaDataChangeEvent.h"

#include "cpp-sdk/SDK.h"

using alt::CEvent;
using EventType = CEvent::Type;

V8::LocalEventHandler syncedMetaChange(
	EventType::SYNCED_META_CHANGE,
	"syncedMetaChange",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
        auto ev = static_cast<const alt::CSyncedMetaDataChangeEvent *>(e);
        v8::Isolate *isolate = resource->GetIsolate();

        args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
        args.push_back(V8_NEW_STRING(ev->GetKey().CStr()));
        args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	});

V8::LocalEventHandler streamSyncedMetaChange(
	EventType::STREAM_SYNCED_META_CHANGE,
	"streamSyncedMetaChange",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
        auto ev = static_cast<const alt::CStreamSyncedMetaDataChangeEvent *>(e);
        v8::Isolate *isolate = resource->GetIsolate();

        args.push_back(resource->GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
        args.push_back(V8_NEW_STRING(ev->GetKey().CStr()));
        args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	});

V8::LocalEventHandler globalSyncedMetaChange(
	EventType::GLOBAL_SYNCED_META_CHANGE,
	"globalSyncedMetaChange",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
        auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent *>(e);
        v8::Isolate *isolate = resource->GetIsolate();

        args.push_back(V8_NEW_STRING(ev->GetKey().CStr()));
        args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	});

V8::LocalEventHandler globalMetaChange(
	EventType::GLOBAL_META_CHANGE,
	"globalMetaChange",
	[](V8ResourceImpl *resource, const alt::CEvent *e, std::vector<v8::Local<v8::Value>> &args) {
        auto ev = static_cast<const alt::CGlobalMetaDataChangeEvent *>(e);
        v8::Isolate *isolate = resource->GetIsolate();

        args.push_back(V8_NEW_STRING(ev->GetKey().CStr()));
        args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
		args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));
	});