
#include "../CV8Resource.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Helpers.h"

struct DiscordRequestOAuth2TokenCallbackData
{
	CV8ResourceImpl *resource;
	v8::Persistent<v8::Promise::Resolver> resolver;
};

static void CurrentUserGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	auto discord = alt::ICore::Instance().GetDiscordManager();
	if (discord->IsUserDataReady())
	{
		v8::Local<v8::Object> discordInfo = v8::Object::New(isolate);

		discordInfo->Set(
			ctx,
			v8::String::NewFromUtf8(isolate, "id").ToLocalChecked(),
			v8::String::NewFromUtf8(isolate, discord->GetUserID().CStr()).ToLocalChecked());

		discordInfo->Set(
			ctx,
			v8::String::NewFromUtf8(isolate, "name").ToLocalChecked(),
			v8::String::NewFromUtf8(isolate, discord->GetUsername().CStr()).ToLocalChecked());

		discordInfo->Set(
			ctx,
			v8::String::NewFromUtf8(isolate, "discriminator").ToLocalChecked(),
			v8::String::NewFromUtf8(isolate, discord->GetDiscriminator().CStr()).ToLocalChecked());

		discordInfo->Set(
			ctx,
			v8::String::NewFromUtf8(isolate, "avatar").ToLocalChecked(),
			v8::String::NewFromUtf8(isolate, discord->GetAvatar().CStr()).ToLocalChecked());

		info.GetReturnValue().Set(discordInfo);
	}
	else
	{
		info.GetReturnValue().Set(v8::Null(isolate));
	}
}

static V8Class v8Discord("Discord", "", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticAccessor(isolate, tpl, "currentUser", &CurrentUserGetter);
});
