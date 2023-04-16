#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "helpers/BindHelpers.h"

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
        headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
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

static std::list<v8::Global<v8::Promise::Resolver>> requestPromises;

static void Get(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, url);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Get(callback, url, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Head(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, url);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Head(callback, url, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Post(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Post(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Put(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Put(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Delete(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Delete(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Connect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Connect(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Options(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Options(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Trace(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Trace(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void Patch(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(client, alt::IHttpClient);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, url);
    V8_ARG_TO_STRING(2, body);

    auto& persistent = requestPromises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));
    auto callback = [](alt::IHttpClient::HttpResponse response, const void* userData)
    {
        // TODO: NOT PERFORMANCE EFFICIENT TO LOCK HERE, RESOLVE IN NEXT TICK INSTEAD

        v8::Isolate* isolate = CV8ScriptRuntime::Instance().GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);

        auto persistent = (v8::Global<v8::Promise::Resolver>*)userData;
        auto resolver = persistent->Get(isolate);
        auto ctx = resolver->GetCreationContext().ToLocalChecked();
        {
            v8::Context::Scope ctxscope(ctx);
            V8_NEW_OBJECT(responseObj);
            V8_OBJECT_SET_INT(responseObj, "statusCode", response.statusCode);
            V8_OBJECT_SET_STRING(responseObj, "body", response.body);
            V8_NEW_OBJECT(headers);
            for(auto it = response.headers->Begin(); it; it = response.headers->Next())
            {
                headers->Set(ctx, V8Helpers::JSValue(it->GetKey().c_str()), V8Helpers::JSValue(it->GetValue().As<alt::IMValueString>()->Value().c_str()));
            }
            responseObj->Set(ctx, V8Helpers::JSValue("headers"), headers);
            resolver->Resolve(resolver->GetCreationContext().ToLocalChecked(), responseObj);
        }

        requestPromises.remove(*persistent);
    };

    client->Patch(callback, url, body, &persistent);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::HTTP_CLIENT, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::HTTP_CLIENT)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

extern V8Class v8BaseObject;
extern V8Class v8HttpClient("HttpClient",
                            v8BaseObject,
                            &Constructor,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                V8Helpers::SetAccessor<alt::IHttpClient, uint32_t, &alt::IHttpClient::GetID>(isolate, tpl, "id");
                                V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

                                V8Helpers::SetMethod(isolate, tpl, "setExtraHeader", &SetExtraHeader);
                                V8Helpers::SetMethod(isolate, tpl, "getExtraHeaders", &GetExtraHeaders);

                                V8Helpers::SetMethod(isolate, tpl, "get", Get);
                                V8Helpers::SetMethod(isolate, tpl, "head", Head);
                                V8Helpers::SetMethod(isolate, tpl, "post", Post);
                                V8Helpers::SetMethod(isolate, tpl, "put", Put);
                                V8Helpers::SetMethod(isolate, tpl, "delete", Delete);
                                V8Helpers::SetMethod(isolate, tpl, "connect", Connect);
                                V8Helpers::SetMethod(isolate, tpl, "options", Options);
                                V8Helpers::SetMethod(isolate, tpl, "trace", Trace);
                                V8Helpers::SetMethod(isolate, tpl, "patch", Patch);
                            });
