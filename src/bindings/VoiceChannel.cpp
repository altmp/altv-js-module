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
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Boolean> isSpatial = info[0]->ToBoolean(isolate);
	v8::Local<v8::Number> maxDistance = info[1]->ToNumber(isolate);

	Ref<IVoiceChannel> ch = ICore::Instance().CreateVoiceChannel(isSpatial->Value(), maxDistance->Value());

	if (ch)
		resource->BindEntity(info.This(), ch.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create VoiceChannel");
}

static V8Class v8VoiceChannel("VoiceChannel", "BaseObject", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

	//Common getter/setters
	//proto->SetAccessor(v8::String::NewFromUtf8(isolate, "population"), &PopulationGetter);

	proto->Set(v8::String::NewFromUtf8(isolate, "addPlayer"), v8::FunctionTemplate::New(isolate, &AddPlayer));
	proto->Set(v8::String::NewFromUtf8(isolate, "removePlayer"), v8::FunctionTemplate::New(isolate, &RemovePlayer));
	proto->Set(v8::String::NewFromUtf8(isolate, "isPlayerInChannel"), v8::FunctionTemplate::New(isolate, &IsPlayerInChannel));
	proto->Set(v8::String::NewFromUtf8(isolate, "mutePlayer"), v8::FunctionTemplate::New(isolate, &MutePlayer));
	proto->Set(v8::String::NewFromUtf8(isolate, "unmutePlayer"), v8::FunctionTemplate::New(isolate, &UnmutePlayer));
	proto->Set(v8::String::NewFromUtf8(isolate, "isPlayerMuted"), v8::FunctionTemplate::New(isolate, &IsPlayerMuted));
});
