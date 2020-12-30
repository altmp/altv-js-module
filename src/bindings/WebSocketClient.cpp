#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IWebView.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, url);

	alt::IResource* altres = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(altres, "invalid resource");

	alt::Ref<IWebSocketClient> webSocket = nullptr;

	webSocket = alt::ICore::Instance().CreateWebSocketClient(url, altres);

	V8_BIND_BASE_OBJECT(webSocket);
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, fun);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	static_cast<CV8ResourceImpl*>(resource)->SubscribeWebSocketClient(webSocket, evName.ToString(), fun, V8::SourceLocation::GetCurrent(isolate));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, fun);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	static_cast<CV8ResourceImpl*>(resource)->UnsubscribeWebSocketClient(webSocket, evName.ToString(), fun);
}

static void Start(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	webSocket->Start();
}

static void Send(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, msg);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_RETURN_BOOLEAN(webSocket->Send(msg.CStr()));
}

static void Stop(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	webSocket->Stop();
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

	v8::Local<v8::Array> protocolsArray = v8::Array::New(isolate, protocols.size());

	int idx = 0;
	for(auto& protocol : protocols)
		protocolsArray->Set(ctx, idx++, v8::String::NewFromUtf8(isolate, protocol.CStr()).ToLocalChecked());

	V8_RETURN(protocolsArray);
}

static void URLGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_RETURN_STRING(webSocket->GetUrl().CStr());
}

static void URLSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_TO_STRING(value, url);

	webSocket->SetUrl(url);
}

static void StateGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_RETURN_UINT32(webSocket->GetState());
}

static void AutoReconnectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_TO_BOOLEAN(value, toggle);

	webSocket->SetAutoReconnectEnabled(toggle);
}

static void AutoReconnectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_RETURN_BOOLEAN(webSocket->IsAutoReconnectEnabled());
}

static void PerMessageDeflateSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_TO_BOOLEAN(value, toggle);

	webSocket->SetPerMessageDeflateEnabled(toggle);
}

static void PerMessageDeflateGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_RETURN_BOOLEAN(webSocket->IsPerMessageDeflateEnabled());
}

static void PingIntervalSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_TO_INTEGER(value, interval);

	webSocket->SetPingInterval(interval);
}

static void PingIntervalGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(webSocket, alt::IWebSocketClient);

	V8_RETURN_UINT32(webSocket->GetPingInterval());
}

extern V8Class v8BaseObject;
extern V8Class v8WebSocketClient("WebSocketClient", v8BaseObject, &Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetMethod(isolate, tpl, "on", &On);
	V8::SetMethod(isolate, tpl, "off", &Off);

	V8::SetMethod(isolate, tpl, "start", &Start);
	V8::SetMethod(isolate, tpl, "send", &Send);
	V8::SetMethod(isolate, tpl, "stop", &Stop);

	V8::SetMethod(isolate, tpl, "addSubProtocol", &AddSubProtocol);
	V8::SetMethod(isolate, tpl, "getSubProtocols", &GetSubProtocols);

	V8::SetAccessor(isolate, tpl, "autoReconnect", &AutoReconnectGetter, &AutoReconnectSetter);
	V8::SetAccessor(isolate, tpl, "perMessageDeflate", &PerMessageDeflateGetter, &PerMessageDeflateSetter);
	V8::SetAccessor(isolate, tpl, "pingInterval", &PingIntervalGetter, &PingIntervalSetter);
	V8::SetAccessor(isolate, tpl, "url", &URLGetter, &URLSetter);
	V8::SetAccessor(isolate, tpl, "state", &StateGetter, nullptr);
});
