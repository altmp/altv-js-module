#include "stdafx.h"

#include "helpers/V8Module.h"
#include "CNodeResourceImpl.h"

using namespace alt;

static void OnClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, eventName);
	V8_ARG_TO_FUNCTION(2, callback);
	
	resource->SubscribeRemote(eventName.ToString(), callback, V8::SourceLocation::GetCurrent(isolate));
}

static void OnceClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, eventName);
	V8_ARG_TO_FUNCTION(2, callback);
	
	resource->SubscribeRemote(eventName.ToString(), callback, V8::SourceLocation::GetCurrent(isolate), true);
}

static void OffClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, eventName);
	V8_ARG_TO_FUNCTION(2, callback);
	
	resource->UnsubscribeRemote(eventName.ToString(), callback);
}

static void EmitClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN_MIN(2);

	V8_ARG_TO_STRING(2, eventName);

	Ref<IPlayer> player;
	if (!info[0]->IsNull())
	{
		V8Entity* v8Player = V8Entity::Get(info[0]);

		V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or null expected");

		player = v8Player->GetHandle().As<IPlayer>();
	}

	MValueArgs mvArgs;

	for (int i = 2; i < info.Length(); ++i)
		mvArgs.Push(V8Helpers::V8ToMValue(info[i]));

	ICore::Instance().TriggerClientEvent(player, eventName.ToString(), mvArgs);
}

static void SetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, value);

	alt::ICore::Instance().SetSyncedMetaData(key, value);
}

static void DeleteSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_STRING(1, key);
	
	alt::ICore::Instance().DeleteSyncedMetaData(key);
}

static void GetPlayersByName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	Log::Warning << "alt.getPlayersByName is deprecated and will be removed in future versions, "
		<< "consider using alt.Player.all.filter(p => p.name == name)" << Log::Endl;

	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_STRING(1, name);

	alt::Array<Ref<alt::IPlayer>> players = alt::ICore::Instance().GetPlayersByName(name);

	v8::Local<v8::Array> arr = v8::Array::New(isolate, players.GetSize());
	
	for (uint32_t i = 0; i < players.GetSize(); ++i)
		arr->Set(i, resource->GetBaseObjectOrNull(players[i]));

	V8_RETURN(arr);
}

static void GetNetTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	
	uint32_t netTime = alt::ICore::Instance().GetNetTime();

	V8_RETURN_UINT32(netTime);
}

static void StartResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, name);
	
	alt::ICore::Instance().StartResource(name);
}

static void StopResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, name);
	
	alt::ICore::Instance().StopResource(name);
}

static void RestartResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, name);
	
	alt::ICore::Instance().RestartResource(name);
}

static void GetResourceMain(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_STRING(1, name);

	alt::IResource* resource = alt::ICore::Instance().GetResource(name);

	V8_CHECK(resource, "Resource does not exist");

	V8_RETURN_STRING(resource->GetMain().CStr());
}

static void GetResourcePath(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_STRING(1, name);

	alt::IResource* resource = alt::ICore::Instance().GetResource(name);

	V8_CHECK(resource, "Resource does not exist");
	
	V8_RETURN_STRING(resource->GetPath().CStr());
}

static void HasResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, name);
	
	alt::IResource* resource = alt::ICore::Instance().GetResource(name);

	V8_RETURN_BOOLEAN(resource && resource->IsStarted());
}

static void GetResourceExports(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, name);

	alt::IResource* _resource = alt::ICore::Instance().GetResource(name);
	if (_resource)
	{
		v8::Local<v8::Value> exports = V8Helpers::MValueToV8(_resource->GetExports());
		V8_RETURN(exports);
	}
}

// For internal usage only
static void ResourceLoaded(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK(ICore::Instance().IsDebug(), "alt.resourceLoaded is only available in debug mode");
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, name);
	
	alt::IResource* resource = alt::ICore::Instance().GetResource(name);
	if (resource && resource->GetType() == "js")
	{
		CNodeResourceImpl* _resource = static_cast<CNodeResourceImpl*>(resource->GetImpl());
		_resource->Started(info[1]);
	}
}

extern V8Class v8Vector3,
    v8RGBA,
    v8File,
    v8BaseObject,
    v8WorldObject,
    v8Entity,
    v8Player,
    v8Vehicle,
    v8Blip,
    v8PointBlip,
    v8Checkpoint,
    v8VoiceChannel,
    v8Colshape,
    v8ColshapeCylinder,
    v8ColshapeSphere,
    v8ColshapeCircle,
    v8ColshapeCuboid,
    v8ColshapeRectangle;

extern V8Module v8Alt("alt",
{
	v8Vector3,
	v8RGBA,
	v8File,
	v8BaseObject,
	v8WorldObject,
	v8Entity,
	v8Player,
	v8Vehicle,
	v8Blip,
	v8PointBlip,
	v8Checkpoint,
	v8VoiceChannel,
	v8Colshape,
	v8ColshapeCylinder,
	v8ColshapeSphere,
	v8ColshapeCircle,
	v8ColshapeCuboid,
	v8ColshapeRectangle
},
[](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::RegisterSharedMain(ctx, exports);

	V8Helpers::RegisterFunc(exports, "getResourceMain", &GetResourceMain);
	V8Helpers::RegisterFunc(exports, "getResourcePath", &GetResourcePath);
	V8Helpers::RegisterFunc(exports, "resourceLoaded", &ResourceLoaded);
	V8Helpers::RegisterFunc(exports, "hasResource", &HasResource);
	V8Helpers::RegisterFunc(exports, "getResourceExports", &GetResourceExports);

	V8Helpers::RegisterFunc(exports, "startResource", &StartResource);
	V8Helpers::RegisterFunc(exports, "stopResource", &StopResource);
	V8Helpers::RegisterFunc(exports, "restartResource", &RestartResource);

	V8Helpers::RegisterFunc(exports, "onClient", &OnClient);
	V8Helpers::RegisterFunc(exports, "onceClient", &OnceClient);
	V8Helpers::RegisterFunc(exports, "offClient", &OffClient);
	V8Helpers::RegisterFunc(exports, "emitClient", &EmitClient);

	V8Helpers::RegisterFunc(exports, "setSyncedMeta", &SetSyncedMeta);
	V8Helpers::RegisterFunc(exports, "deleteSyncedMeta", &DeleteSyncedMeta);

	V8Helpers::RegisterFunc(exports, "getPlayersByName", &GetPlayersByName);

	V8Helpers::RegisterFunc(exports, "getNetTime", &GetNetTime);

	alt::StringView rootDir = alt::ICore::Instance().GetRootDirectory();
	exports->Set(isolate->GetEnteredContext(), v8::String::NewFromUtf8(isolate, "rootDir"), v8::String::NewFromUtf8(isolate, rootDir.CStr()));

	exports->Set(isolate->GetEnteredContext(), v8::String::NewFromUtf8(isolate, "defaultDimension"), v8::Integer::New(isolate, alt::DEFAULT_DIMENSION));
	exports->Set(isolate->GetEnteredContext(), v8::String::NewFromUtf8(isolate, "globalDimension"), v8::Integer::New(isolate, alt::GLOBAL_DIMENSION));
});
