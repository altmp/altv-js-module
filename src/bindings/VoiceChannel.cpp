#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void AddPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IVoiceChannel> ch = _this->GetHandle().As<alt::IVoiceChannel>();
	
	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IPlayer> ent = targetEntity->GetHandle().As<alt::IPlayer>();
	ch->AddPlayer(ent);
}

static void RemovePlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IVoiceChannel> ch = _this->GetHandle().As<alt::IVoiceChannel>();

	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IPlayer> ent = targetEntity->GetHandle().As<alt::IPlayer>();
	ch->RemovePlayer(ent);
}

static void IsPlayerInChannel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IVoiceChannel> ch = _this->GetHandle().As<alt::IVoiceChannel>();

	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IPlayer> ent = targetEntity->GetHandle().As<alt::IPlayer>();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, ch->HasPlayer(ent)));
}

static void MutePlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IVoiceChannel> ch = _this->GetHandle().As<alt::IVoiceChannel>();

	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IPlayer> ent = targetEntity->GetHandle().As<alt::IPlayer>();
	ch->MutePlayer(ent);
}

static void UnmutePlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IVoiceChannel> ch = _this->GetHandle().As<alt::IVoiceChannel>();

	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IPlayer> ent = targetEntity->GetHandle().As<alt::IPlayer>();
	ch->UnmutePlayer(ent);
}

static void IsPlayerMuted(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsObject(), "entity expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IVoiceChannel> ch = _this->GetHandle().As<alt::IVoiceChannel>();

	V8Entity* targetEntity = V8Entity::Get(info[0]);
	V8_CHECK(targetEntity, "entity is invalid");

	Ref<IPlayer> ent = targetEntity->GetHandle().As<alt::IPlayer>();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, ch->IsPlayerMuted(ent)));
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_BOOLEAN(1, isSpatial);
	V8_ARG_TO_NUMBER(2, maxDistance);
	V8_BIND_BASE_OBJECT(ICore::Instance().CreateVoiceChannel(isSpatial, maxDistance),
		"Failed to create VoiceChannel, make sure voice chat is enabled");
}

extern V8Class v8BaseObject;
extern V8Class v8VoiceChannel("VoiceChannel", v8BaseObject, Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetMethod(isolate, tpl, "addPlayer", &AddPlayer);
	V8::SetMethod(isolate, tpl, "removePlayer", &RemovePlayer);
	V8::SetMethod(isolate, tpl, "isPlayerInChannel", &IsPlayerInChannel);
	V8::SetMethod(isolate, tpl, "mutePlayer", &MutePlayer);
	V8::SetMethod(isolate, tpl, "unmutePlayer", &UnmutePlayer);
	V8::SetMethod(isolate, tpl, "isPlayerMuted", &IsPlayerMuted);
});
