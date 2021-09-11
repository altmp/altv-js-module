
#include "../CV8Resource.h"
#include "V8Class.h"
#include "V8Helpers.h"

struct DiscordRequestOAuth2TokenCallbackData
{
	CV8ResourceImpl *resource;
	v8::Persistent<v8::Promise::Resolver> resolver;
};

static void CurrentUserGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	auto discord = alt::ICore::Instance().GetDiscordManager();
	if (discord->IsUserDataReady())
	{
		V8_NEW_OBJECT(discordInfo);

		V8_OBJECT_SET_STRING(discordInfo, "id", discord->GetUserID());
		V8_OBJECT_SET_STRING(discordInfo, "name", discord->GetUsername());
		V8_OBJECT_SET_STRING(discordInfo, "discriminator", discord->GetDiscriminator());
		V8_OBJECT_SET_STRING(discordInfo, "avatar", discord->GetAvatar());

		V8_RETURN(discordInfo);
	}
	else
	{
		V8_RETURN_NULL();
	}
}

extern V8Class v8Discord("Discord", [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticAccessor(isolate, tpl, "currentUser", &CurrentUserGetter);
});
