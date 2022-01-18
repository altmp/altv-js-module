#include "stdafx.h"

#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "helpers/Bindings.h"
#include "V8ResourceImpl.h"

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(con->name);
}

static void SocialIDGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(std::to_string(con->socialId));
}

static void HwidHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(std::to_string(con->hwidHash));
}

static void HwidExHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(std::to_string(con->hwidExHash));
}

static void AuthTokenGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(con->authToken);
}

static void IsDebugGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_BOOLEAN(con->isDebug);
}

static void BranchGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(con->branch);
}

static void BuildGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_UINT(con->build);
}

static void CdnUrlGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_STD_STRING(con->cdnUrl);
}

static void PasswordHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_RETURN_UINT64(con->passwordHash);
}

static void Accept(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");

    alt::ICore::Instance().AcceptConnectionFromQueue(con);
}

static void Decline(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    alt::ConnectionInfo* con = static_cast<alt::ConnectionInfo*>(info.This()->GetInternalField(0).As<v8::External>()->Value());
    V8_CHECK(con, "Invalid connection info");
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STD_STRING(1, reason);

    alt::ICore::Instance().DeclineConnectionFromQueue(con, reason);
}

extern V8Class v8ConnectionInfo("ConnectionInfo", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    V8Helpers::SetMethod(isolate, tpl, "accept", &Accept);
    V8Helpers::SetMethod(isolate, tpl, "decline", &Decline);

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
});
