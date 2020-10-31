
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"
#include "cpp-sdk/entities/IPlayer.h"

using namespace alt;

static void IDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	info.GetReturnValue().Set(v8::Integer::New(isolate, ent->GetID()));
}

static void OwnerGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IEntity);
	V8_RETURN_BASE_OBJECT(_this->GetNetworkOwner());
}

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IEntity> ent = _this->GetHandle().As<IEntity>();

	alt::Position _pos = ent->GetPosition();
	info.GetReturnValue().Set(resource->CreateVector3(_pos));
}

static void DimensionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IEntity> ent = _this->GetHandle().As<IEntity>();

	info.GetReturnValue().Set(v8::Integer::New(isolate, ent->GetDimension()));
}

static void RotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	alt::Rotation _rot = ent->GetRotation();
	info.GetReturnValue().Set(resource->CreateVector3(_rot));
}

static void ModelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();
	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, ent->GetModel()));
}

static void HasSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, ent->HasSyncedMetaData(*v8::String::Utf8Value(isolate, key))));
}

static void GetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();

	alt::MValueConst val = ent->GetSyncedMetaData(*v8::String::Utf8Value(isolate, key));
	info.GetReturnValue().Set(V8Helpers::MValueToV8(val));
}

static void HasStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, ent->HasStreamSyncedMetaData(*v8::String::Utf8Value(isolate, key))));
}

static void GetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();

	alt::MValueConst val = ent->GetStreamSyncedMetaData(*v8::String::Utf8Value(isolate, key));
	info.GetReturnValue().Set(V8Helpers::MValueToV8(val));
}

#ifdef ALT_SERVER_API

static void RotationSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(val->IsObject(), "object expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> obj = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::Object> pos = val.As<v8::Object>();

	v8::Local<v8::Number> x = pos->Get(v8::String::NewFromUtf8(isolate, "x"))->ToNumber(isolate);
	v8::Local<v8::Number> y = pos->Get(v8::String::NewFromUtf8(isolate, "y"))->ToNumber(isolate);
	v8::Local<v8::Number> z = pos->Get(v8::String::NewFromUtf8(isolate, "z"))->ToNumber(isolate);

	obj->SetRotation({float(x->Value()), float(y->Value()), float(z->Value())});
}

static void ModelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8_CHECK(val->IsNumber() || val->IsString(), "model can be number or string");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();
	V8_CHECK(player, "model can be set only for player");

	if (val->IsNumber())
	{
		player->SetModel(val->ToInteger(isolate)->Value());
	}
	else
	{
		v8::String::Utf8Value strVal{isolate, val};
		player->SetModel(alt::ICore::Instance().Hash(*strVal));
	}
}

static void SetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(isolate);

	ent->SetSyncedMetaData(*v8::String::Utf8Value(isolate, key), V8Helpers::V8ToMValue(info[1]));
}

static void DeleteSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(isolate);
	ent->DeleteSyncedMetaData(*v8::String::Utf8Value(isolate, key));
}

static void SetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(isolate);

	ent->SetStreamSyncedMetaData(*v8::String::Utf8Value(isolate, key), V8Helpers::V8ToMValue(info[1]));
}

static void DeleteStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<alt::IEntity> ent = _this->GetHandle().As<alt::IEntity>();

	v8::Local<v8::String> key = info[0]->ToString(isolate);
	ent->DeleteStreamSyncedMetaData(*v8::String::Utf8Value(isolate, key));
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

static V8Class v8entity("Entity", "WorldObject", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

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
