
#include "../CV8Resource.h"
#include "V8Class.h"
#include "V8Helpers.h"

static void CurrentUserGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    auto discord = alt::ICore::Instance().GetDiscordManager();
    if(discord->IsUserDataReady())
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

static void RequestOAuth2Token(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_CHECK(info[0]->IsString(), "zoomDataId must be a number or string");

    V8_ARG_TO_STRING(1, appid);

    V8_RETURN(alt::ICore::Instance().DiscordRequestOAuth2Token(appid));
}

static void IsOAuth2Finished(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_RETURN(alt::ICore::Instance().DiscordIsOAuth2Finished());
}

static void GetOAuth2Token(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_RETURN_STRING(alt::ICore::Instance().DiscordGetOAuth2Token());
}

extern V8Class v8Discord("Discord", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "currentUser", &CurrentUserGetter);

    V8Helpers::SetStaticMethod(isolate, tpl, "requestOAuth2Token", &RequestOAuth2Token);
    V8Helpers::SetStaticAccessor(isolate, tpl, "hasOAuth2Finished", &IsOAuth2Finished);
    V8Helpers::SetStaticAccessor(isolate, tpl, "getOAuth2Token", &GetOAuth2Token);
});
