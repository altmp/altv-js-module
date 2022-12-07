#include "stdafx.h"

#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "helpers/Bindings.h"
#include "V8ResourceImpl.h"

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(con->GetName());
}

static void SocialIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(std::to_string(con->GetSocialId()));
}

static void HwidHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(std::to_string(con->GetHwIdHash()));
}

static void HwidExHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(std::to_string(con->GetHwIdExHash()));
}

static void AuthTokenGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(con->GetAuthToken());
}

static void IsDebugGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_BOOLEAN(con->GetIsDebug());
}

static void BranchGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(con->GetBranch());
}

static void BuildGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_UINT(con->GetBuild());
}

static void CdnUrlGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(con->GetCdnUrl());
}

static void PasswordHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_UINT64(con->GetPasswordHash());
}

static void IpGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(con->GetIp());
}

static void DiscordUserIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STRING(std::to_string(con->GetDiscordUserID()));
}

static void Accept(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(0, 1);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");

    V8_ARG_TO_BOOLEAN_OPT(1, sendNames, true);

    con->Accept(sendNames);
}

static void Decline(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, reason);

    con->Decline(reason);
}

static void IsAcceptedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, con, alt::IConnectionInfo);
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN(con->IsAccepted());
}

extern V8Class v8ConnectionInfo("ConnectionInfo",
                                [](v8::Local<v8::FunctionTemplate> tpl)
                                {
                                    v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                    tpl->InstanceTemplate()->SetInternalFieldCount(1);

                                    V8Helpers::SetMethod(isolate, tpl, "accept", &Accept);
                                    V8Helpers::SetMethod(isolate, tpl, "decline", &Decline);
                                    V8Helpers::SetAccessor(isolate, tpl, "isAccepted", &IsAcceptedGetter);

                                    V8Helpers::SetAccessor(isolate, tpl, "name", &NameGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "socialID", &SocialIDGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "hwidHash", &HwidHashGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "hwidExHash", &HwidExHashGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "authToken", &AuthTokenGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "isDebug", &IsDebugGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "branch", &BranchGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "build", &BuildGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "cdnUrl", &CdnUrlGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "passwordHash", &PasswordHashGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "ip", &IpGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "discordUserID", &DiscordUserIDGetter);
                                });
