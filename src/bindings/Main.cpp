#include "stdafx.h"

#include "helpers/V8Module.h"
#include "CNodeResourceImpl.h"

using namespace alt;

static void OnClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "string expected");
	V8_CHECK(info[1]->IsFunction(), "function expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::String::Utf8Value evName(isolate, info[0]);
	resource->SubscribeRemote(*evName, info[1].As<v8::Function>(), V8::SourceLocation::GetCurrent(isolate));
}

static void OnceClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "string expected");
	V8_CHECK(info[1]->IsFunction(), "function expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::String::Utf8Value evName(isolate, info[0]);
	resource->SubscribeRemote(*evName, info[1].As<v8::Function>(), V8::SourceLocation::GetCurrent(isolate), true);
}

static void OffClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "string expected");
	V8_CHECK(info[1]->IsFunction(), "function expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::String::Utf8Value evName(isolate, info[0]);
	resource->UnsubscribeRemote(*evName, info[1].As<v8::Function>());
}

static void EmitClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() >= 2, "at least 2 args expected");
	V8_CHECK(info[0]->IsObject() || info[0]->IsNull(), "player or null expected");
	V8_CHECK(info[1]->IsString(), "string expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::String::Utf8Value evName(isolate, info[1]);
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

	ICore::Instance().TriggerClientEvent(player, *evName, mvArgs);
}

static void EmitAllClients(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, eventName);

	Ref<IPlayer> player;
	MValueArgs args;

	for (int i = 1; i < info.Length(); ++i)
		args.Push(V8Helpers::V8ToMValue(info[i]));

	ICore::Instance().TriggerClientEvent(player, eventName, args);
}

static void SetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	v8::Local<v8::String> key = info[0]->ToString(isolate);
	alt::ICore::Instance().SetSyncedMetaData(*v8::String::Utf8Value(isolate, key), V8Helpers::V8ToMValue(info[1]));
}

static void DeleteSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::String> key = info[0]->ToString(isolate);
	alt::ICore::Instance().DeleteSyncedMetaData(*v8::String::Utf8Value(isolate, key));
}

static void GetPlayersByName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	Log::Warning << "alt.getPlayersByName is deprecated and will be removed in future versions, "
		<< "consider using alt.Player.all.filter(p => p.name == name)" << Log::Endl;

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::Array<Ref<alt::IPlayer>> players = alt::ICore::Instance().GetPlayersByName(*name);
	v8::Local<v8::Array> arr = v8::Array::New(isolate, players.GetSize());
	
	for (uint32_t i = 0; i < players.GetSize(); ++i)
		arr->Set(i, resource->GetBaseObjectOrNull(players[i]));

	info.GetReturnValue().Set(arr);
}

static void GetNetTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	uint32_t netTime = alt::ICore::Instance().GetNetTime();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, netTime));
}

static void StartResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::ICore::Instance().StartResource(*name);
}

static void StopResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::ICore::Instance().StopResource(*name);
}

static void RestartResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::ICore::Instance().RestartResource(*name);
}

static void GetResourceMain(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::IResource* resource = alt::ICore::Instance().GetResource(*name);

	if (resource)
		info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, resource->GetMain().CStr()));
}

static void GetResourcePath(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::IResource* resource = alt::ICore::Instance().GetResource(*name);

	if (resource)
		info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, resource->GetPath().CStr()));
}

static void HasResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::IResource* resource = alt::ICore::Instance().GetResource(*name);

	if (resource && resource->IsStarted())
		info.GetReturnValue().Set(v8::True(isolate));
	else
		info.GetReturnValue().Set(v8::False(isolate));
}

static void GetResourceExports(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::IResource* _resource = alt::ICore::Instance().GetResource(*name);
	if (_resource)
	{
		v8::Local<v8::Value> exports = V8Helpers::MValueToV8(_resource->GetExports());
		info.GetReturnValue().Set(exports);
	}
}

static void ResourceLoaded(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value name(isolate, info[0]);

	alt::IResource* resource = alt::ICore::Instance().GetResource(*name);
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
	V8Helpers::RegisterFunc(exports, "emitAllClients", &EmitAllClients);

	V8Helpers::RegisterFunc(exports, "setSyncedMeta", &SetSyncedMeta);
	V8Helpers::RegisterFunc(exports, "deleteSyncedMeta", &DeleteSyncedMeta);

	V8Helpers::RegisterFunc(exports, "getPlayersByName", &GetPlayersByName);

	V8Helpers::RegisterFunc(exports, "getNetTime", &GetNetTime);

	alt::StringView rootDir = alt::ICore::Instance().GetRootDirectory();
	exports->Set(isolate->GetEnteredContext(), v8::String::NewFromUtf8(isolate, "rootDir"), v8::String::NewFromUtf8(isolate, rootDir.CStr()));

	exports->Set(isolate->GetEnteredContext(), v8::String::NewFromUtf8(isolate, "defaultDimension"), v8::Integer::New(isolate, alt::DEFAULT_DIMENSION));
	exports->Set(isolate->GetEnteredContext(), v8::String::NewFromUtf8(isolate, "globalDimension"), v8::Integer::New(isolate, alt::GLOBAL_DIMENSION));
});
