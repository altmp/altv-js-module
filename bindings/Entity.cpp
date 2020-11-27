
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"
#include "cpp-sdk/objects/IPlayer.h"

using namespace alt;

static void IDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);
	
	V8_RETURN_INTEGER(ent->GetID());
}

static void OwnerGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_BASE_OBJECT(ent->GetNetworkOwner());
}

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN(resource->CreateVector3(ent->GetPosition()));
}

static void DimensionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_INTEGER(ent->GetDimension());
}

static void RotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN(resource->CreateVector3(ent->GetRotation()));
}

static void ModelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_UINT32(ent->GetModel());
}

static void HasSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_BOOLEAN(ent->HasSyncedMetaData(key));
}

static void GetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_MVALUE(ent->GetSyncedMetaData(key));
}

static void HasStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_BOOLEAN(ent->HasStreamSyncedMetaData(key));
}

static void GetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_RETURN_MVALUE(ent->GetStreamSyncedMetaData(key));
}

#ifdef ALT_SERVER_API

static void RotationSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	V8_TO_OBJECT(val, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	ent->SetRotation({ x, y, z });
}

static void ModelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

	if (val->IsNumber())
	{
		V8_TO_INTEGER(val, model);
		player->SetModel(model);
	}
	else
	{
		V8_TO_STRING(val, model);
		player->SetModel(alt::ICore::Instance().Hash(model));
	}
}

static void SetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, value);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	ent->SetSyncedMetaData(key, value);
}

static void DeleteSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	ent->DeleteSyncedMetaData(key);
}

static void SetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, value);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	ent->SetStreamSyncedMetaData(key, value);
}

static void DeleteStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

	ent->DeleteStreamSyncedMetaData(key);
}

static void SetNetOwner(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN2(1, 2);
	V8_GET_THIS_BASE_OBJECT(_this, IEntity);
	V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");
	V8_ARG_TO_BOOLEAN_OPT(2, disableMigration, false);

	_this->SetNetworkOwner(player, disableMigration);
}

static void ResetNetOwner(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN2(0, 1);
	V8_GET_THIS_BASE_OBJECT(_this, IEntity);
	V8_ARG_TO_BOOLEAN_OPT(1, disableMigration, false);

	_this->SetNetworkOwner(nullptr, disableMigration);
}

#endif // ALT_SERVER_API

#ifdef ALT_CLIENT_API

static void ScriptIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IEntity);
	V8_RETURN_INTEGER(_this->GetScriptGuid());
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, scriptGuid);
	V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid));
}

#endif // ALT_CLIENT_API

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, id);
	V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByID(id));
}

static void StaticAllGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	v8::Local<v8::Array> arr = v8::Array::New(isolate);

	uint16_t i = 0;
	for (auto entity : alt::ICore::Instance().GetEntities())
	{
		if (entity)
			arr->Set(ctx, i++, resource->GetOrCreateEntity(entity.Get(), "Entity")->GetJSVal(isolate));
	};

	V8_RETURN(arr);
}

extern V8Class v8WorldObject;
extern V8Class v8Entity("Entity", v8WorldObject, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
	V8::SetStaticAccessor(isolate, tpl, "all", StaticAllGetter);

	V8::SetAccessor(isolate, tpl, "id", IDGetter);

	V8::SetAccessor(isolate, tpl, "netOwner", OwnerGetter);

	V8::SetMethod(isolate, tpl, "hasSyncedMeta", HasSyncedMeta);
	V8::SetMethod(isolate, tpl, "getSyncedMeta", GetSyncedMeta);

	V8::SetMethod(isolate, tpl, "hasStreamSyncedMeta", HasStreamSyncedMeta);
	V8::SetMethod(isolate, tpl, "getStreamSyncedMeta", GetStreamSyncedMeta);

#ifdef ALT_SERVER_API
	V8::SetAccessor(isolate, tpl, "rot", RotationGetter, RotationSetter);
	V8::SetAccessor(isolate, tpl, "model", ModelGetter, ModelSetter);

	V8::SetMethod(isolate, tpl, "setSyncedMeta", SetSyncedMeta);
	V8::SetMethod(isolate, tpl, "deleteSyncedMeta", DeleteSyncedMeta);

	V8::SetMethod(isolate, tpl, "setStreamSyncedMeta", SetStreamSyncedMeta);
	V8::SetMethod(isolate, tpl, "deleteStreamSyncedMeta", DeleteStreamSyncedMeta);

	V8::SetMethod(isolate, tpl, "setNetOwner", SetNetOwner);
	V8::SetMethod(isolate, tpl, "resetNetOwner", ResetNetOwner);
#endif // ALT_SERVER_API

#ifdef ALT_CLIENT_API
	V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

	V8::SetAccessor(isolate, tpl, "pos", PositionGetter);
	//V8::SetAccessor(isolate, tpl, "dimension", DimensionGetter);
	V8::SetAccessor(isolate, tpl, "rot", RotationGetter);
	V8::SetAccessor(isolate, tpl, "model", ModelGetter);
	V8::SetAccessor(isolate, tpl, "scriptID", ScriptIDGetter);
#endif // ALT_CLIENT_API
});
