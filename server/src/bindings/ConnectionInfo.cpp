#include "stdafx.h"

#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "helpers/Bindings.h"
#include "V8ResourceImpl.h"

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(con->GetName());
}

static void SocialIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(std::to_string(con->GetSocialId()));
}

static void HwidHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(std::to_string(con->GetHwIdHash()));
}

static void HwidExHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(std::to_string(con->GetHwIdExHash()));
}

static void AuthTokenGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(con->GetAuthToken());
}

static void IsDebugGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_BOOLEAN(con->GetIsDebug());
}

static void BranchGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(con->GetBranch());
}

static void BuildGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_DEPRECATE("ConnectionInfo build", "versionMajor & versionMinor");

    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_UINT(con->GetVersionMajor());
}

static void VersionMajorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_UINT(con->GetVersionMajor());
}

static void VersionMinorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_UINT(con->GetVersionMinor());
}

static void CdnUrlGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(con->GetCdnUrl());
}

static void PasswordHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_UINT64(con->GetPasswordHash());
}

static void IpGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(con->GetIp());
}

static void DiscordUserIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(std::to_string(con->GetDiscordUserID()));
}

static void SocialClubNameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_STRING(con->GetSocialName());
}

static void ConnectionIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN_INT(con->GetID());
}

static void Accept(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(0, 1);
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);

    V8_ARG_TO_BOOLEAN_OPT(1, sendNames, true);

    con->Accept(sendNames);
}

static void Decline(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);

    V8_ARG_TO_STRING(1, reason);

    con->Decline(reason);
}

static void IsAcceptedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_RETURN(con->IsAccepted());
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllConnectionInfos());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::CONNECTION_INFO, id);

    if(entity)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void GetText(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);

    V8_RETURN_STRING(con->GetText());
}

static void SetText(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);
    V8_TO_STRING(val, text);

    con->SetText(text);
}

static void GetCloudIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);

    V8_RETURN_STRING(con->GetCloudID());
}

static void CloudAuthResultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(con, alt::IConnectionInfo);

    V8_RETURN_UINT(con->GetCloudAuthResult());
}

extern V8Class v8BaseObject;
extern V8Class v8ConnectionInfo("ConnectionInfo",
                                v8BaseObject,
                                nullptr,
                                [](v8::Local<v8::FunctionTemplate> tpl)
                                {
                                    v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                    V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                                    V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

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
                                    V8Helpers::SetAccessor(isolate, tpl, "versionMajor", &VersionMajorGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "versionMinor", &VersionMinorGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "cdnUrl", &CdnUrlGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "passwordHash", &PasswordHashGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "ip", &IpGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "discordUserID", &DiscordUserIDGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "socialClubName", &SocialClubNameGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "id", &ConnectionIDGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "cloudID", &GetCloudIDGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "cloudAuthResult", &CloudAuthResultGetter);
                                    V8Helpers::SetAccessor(isolate, tpl, "text", &GetText, &SetText);
                                });
