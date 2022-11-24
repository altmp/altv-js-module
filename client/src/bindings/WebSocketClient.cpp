#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IWebView.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, url);

    alt::IResource* altres = V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext());
    V8_CHECK(altres, "invalid resource");

    alt::IWebSocketClient* webSocket = nullptr;

    webSocket = alt::ICore::Instance().CreateWebSocketClient(url, altres);

    V8_BIND_BASE_OBJECT(webSocket, "Failed to create WebSocketClient");
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    static_cast<CV8ResourceImpl*>(resource)->SubscribeWebSocketClient(webSocket, evName, fun, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    static_cast<CV8ResourceImpl*>(resource)->UnsubscribeWebSocketClient(webSocket, evName, fun);
}

static void Send(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    bool ret = false;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, msg);
        ret = webSocket->Send(msg);
    }
    else if(info[0]->IsArrayBufferView())
    {
        V8_ARG_TO_ARRAY_BUFFER_VIEW(1, v8ArrayBufferView);
        auto v8Buffer = v8ArrayBufferView->Buffer()->GetBackingStore();
        ret = webSocket->SendBinary(std::string((char*)v8Buffer->Data(), v8Buffer->ByteLength()));
    }
    else if(info[0]->IsArrayBuffer())
    {
        V8_ARG_TO_ARRAY_BUFFER(1, v8ArrayBuffer);
        auto v8Buffer = v8ArrayBuffer->GetBackingStore();
        ret = webSocket->SendBinary(std::string((char*)v8Buffer->Data(), v8Buffer->ByteLength()));
    }
    else
    {
        V8_CHECK(false, "Unknown parameter type specified for IWebSocketClient::Send");
    }
    V8_RETURN_BOOLEAN(ret);
}

static void AddSubProtocol(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, msg);

    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    webSocket->AddSubProtocol(msg);
}

static void GetSubProtocols(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    auto protocols = webSocket->GetSubProtocols();

    v8::Local<v8::Array> protocolsArray = V8Helpers::JSValue(protocols);

    V8_RETURN(protocolsArray);
}

static void SetExtraHeader(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, name);
    V8_ARG_TO_STRING(2, value);

    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    webSocket->SetExtraHeader(name, value);
}

static void GetExtraHeaders(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    auto extraHeaders = webSocket->GetExtraHeaders();
    V8_NEW_OBJECT(headersObject);

    for(auto it = extraHeaders->Begin(); it; it = extraHeaders->Next())
    {
        std::string key = it->GetKey();
        V8_OBJECT_SET_STRING(headersObject, key.c_str(), extraHeaders->Get(key).As<alt::IMValueString>()->Value());
    }

    V8_RETURN(headersObject);
}

static void GetEventListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

    V8_ARG_TO_STRING(1, eventName);

    std::vector<V8Helpers::EventCallback*> handlers = static_cast<CV8ResourceImpl*>(resource)->GetWebSocketClientHandlers(webSocket, eventName);

    auto array = v8::Array::New(isolate, handlers.size());
    for(int i = 0; i < handlers.size(); i++)
    {
        array->Set(ctx, i, handlers[i]->fn.Get(isolate));
    }

    V8_RETURN(array);
}

extern V8Class v8BaseObject;
extern V8Class v8WebSocketClient("WebSocketClient",
                                 v8BaseObject,
                                 &Constructor,
                                 [](v8::Local<v8::FunctionTemplate> tpl)
                                 {
                                     v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                     V8Helpers::SetMethod(isolate, tpl, "on", &On);
                                     V8Helpers::SetMethod(isolate, tpl, "off", &Off);
                                     V8Helpers::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);

                                     V8Helpers::SetMethod<IWebSocketClient, &IWebSocketClient::Start>(isolate, tpl, "start");
                                     V8Helpers::SetMethod<IWebSocketClient, &IWebSocketClient::Stop>(isolate, tpl, "stop");
                                     V8Helpers::SetMethod(isolate, tpl, "send", &Send);

                                     V8Helpers::SetMethod(isolate, tpl, "addSubProtocol", &AddSubProtocol);
                                     V8Helpers::SetMethod(isolate, tpl, "getSubProtocols", &GetSubProtocols);

                                     V8Helpers::SetMethod(isolate, tpl, "setExtraHeader", &SetExtraHeader);
                                     V8Helpers::SetMethod(isolate, tpl, "getExtraHeaders", &GetExtraHeaders);

                                     V8Helpers::SetAccessor<IWebSocketClient, bool, &IWebSocketClient::IsAutoReconnectEnabled, &IWebSocketClient::SetAutoReconnectEnabled>(
                                       isolate, tpl, "autoReconnect");
                                     V8Helpers::SetAccessor<IWebSocketClient, bool, &IWebSocketClient::IsPerMessageDeflateEnabled, &IWebSocketClient::SetPerMessageDeflateEnabled>(
                                       isolate, tpl, "perMessageDeflate");
                                     V8Helpers::SetAccessor<IWebSocketClient, uint16_t, &IWebSocketClient::GetPingInterval, &IWebSocketClient::SetPingInterval>(isolate, tpl, "pingInterval");
                                     V8Helpers::SetAccessor<IWebSocketClient, const std::string&, &IWebSocketClient::GetUrl, &IWebSocketClient::SetUrl>(isolate, tpl, "url");
                                     V8Helpers::SetAccessor<IWebSocketClient, uint8_t, &IWebSocketClient::GetReadyState>(isolate, tpl, "readyState");
                                 });
