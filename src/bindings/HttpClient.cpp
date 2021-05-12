#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Class.h"
#include "../CV8ScriptRuntime.h"

static void SetExtraHeader(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, name);
    V8_ARG_TO_STRING(2, value);

    client->SetExtraHeader(name, value);
}

static void GetExtraHeaders(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    
    auto dict = client->GetExtraHeaders();
    V8_NEW_OBJECT(headers);
    for(auto it = dict->Begin(); it; it = dict->Next())
    {
        headers->Set(ctx, V8_NEW_STRING(it->GetKey().CStr()), V8_NEW_STRING(it->GetValue().As<alt::IMValueString>()->Value().CStr()));
    }

    V8_RETURN(headers);
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();

    auto client = alt::ICore::Instance().CreateHttpClient(resource->GetResource());
    V8_BIND_BASE_OBJECT(client, "Failed to create HttpClient");
}

static std::list<v8::UniquePersistent<v8::Promise::Resolver>> requestPromises;

template<auto Func, bool HasBody>
static void Request(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, url);
    alt::String body;
    if constexpr(HasBody == true) V8_CHECK(V8::SafeToString(info[1], isolate, ctx, body), "Failed to convert argument 2 to string")

    auto& persistent = requestPromises.emplace_back(v8::UniquePersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData) {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::instance->GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::UniquePersistent<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->CreationContext();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INTEGER(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body.CStr());
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8_NEW_STRING(it->GetKey().CStr()), V8_NEW_STRING(it->GetValue().As<alt::IMValueString>()->Value().CStr()));
            }

            resolver->Resolve(ctx, responseObj);
        }

        requestPromises.remove(*persistent);
    };

    if constexpr(HasBody == true) std::invoke(Func, client.Get(), callback, url, body, &persistent);
    else std::invoke(Func, client.Get(), callback, url, &persistent);
}

extern V8Class v8BaseObject;
extern V8Class v8HttpClient("HttpClient", v8BaseObject, &Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

    V8::SetMethod(isolate, tpl, "setExtraHeader", &SetExtraHeader);
    V8::SetMethod(isolate, tpl, "getExtraHeaders", &GetExtraHeaders);

    V8::SetMethod(isolate, tpl, "get", Request<&alt::IHttpClient::Get, false>);
    V8::SetMethod(isolate, tpl, "head", Request<&alt::IHttpClient::Head, false>);
    V8::SetMethod(isolate, tpl, "post", Request<&alt::IHttpClient::Post, true>);
    V8::SetMethod(isolate, tpl, "put", Request<&alt::IHttpClient::Put, true>);
    V8::SetMethod(isolate, tpl, "delete", Request<&alt::IHttpClient::Delete, true>);
    V8::SetMethod(isolate, tpl, "connect", Request<&alt::IHttpClient::Connect, true>);
    V8::SetMethod(isolate, tpl, "options", Request<&alt::IHttpClient::Options, true>);
    V8::SetMethod(isolate, tpl, "trace", Request<&alt::IHttpClient::Trace, true>);
    V8::SetMethod(isolate, tpl, "patch", Request<&alt::IHttpClient::Patch, true>);
});
