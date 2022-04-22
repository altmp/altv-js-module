
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
    static std::list<V8Helpers::CPersistent<v8::Promise::Resolver>> promises;
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, appid);

    auto& persistent = promises.emplace_back(V8Helpers::CPersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

    bool result = alt::ICore::Instance().DiscordRequestOAuth2Token(appid, [&persistent, resource](bool result, const std::string& token) {
        resource->RunOnNextTick([&persistent, resource, result, oauthToken = token] {
            if(!resource->GetResource()->IsStarted())
            {
                promises.remove(persistent);
                return;
            }
            v8::Local<v8::Promise::Resolver> promise = persistent.Get(resource->GetIsolate());
            if(!result) promise->Reject(resource->GetContext(), v8::Exception::Error(V8Helpers::JSValue("Failed to get OAuth2 token")));
            else
                promise->Resolve(resource->GetContext(), V8Helpers::JSValue(oauthToken));
            promises.remove(persistent);
        });
    });

    if(!result) persistent.Get(isolate)->Reject(ctx, v8::Exception::Error(V8Helpers::JSValue("Failed to request OAuth2 token")));

    V8_RETURN(persistent.Get(isolate));
}

extern V8Class v8Discord("Discord", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "currentUser", &CurrentUserGetter);

    V8Helpers::SetStaticMethod(isolate, tpl, "requestOAuth2Token", &RequestOAuth2Token);
});
