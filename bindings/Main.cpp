
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void HashCb(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	v8::String::Utf8Value str(isolate, info[0]);
	uint32_t hash = alt::ICore::Instance().Hash(*str);

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(info.GetIsolate(), hash));
}

static void On(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 2, "on expects 2 args");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");
	V8_CHECK(info[1]->IsFunction(), "callback must be a function");

	V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
	V8_CHECK(resource, "Invalid resource");

	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	v8::Local<v8::Function> callback = info[1].As<v8::Function>();

	resource->SubscribeLocal(evName, callback, V8::SourceLocation::GetCurrent(isolate));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "on expects 2 args");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");
	V8_CHECK(info[1]->IsFunction(), "callback must be a function");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	v8::Local<v8::Function> callback = info[1].As<v8::Function>();

	resource->UnsubscribeLocal(evName, callback);
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() >= 1, "emitClient expects at least 1 arg");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");

	std::string name = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	alt::MValueArgs args;

	for (int i = 1; i < info.Length(); ++i)
		args.Push(V8Helpers::V8ToMValue(info[i]));

	alt::ICore::Instance().TriggerLocalEvent(name, args);
}

static void HasMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, alt::ICore::Instance().HasMetaData(*v8::String::Utf8Value(isolate, key))));
}

static void GetMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();

	alt::MValueConst val = alt::ICore::Instance().GetMetaData(*v8::String::Utf8Value(isolate, key));
	info.GetReturnValue().Set(V8Helpers::MValueToV8(val));
}

static void SetMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 2, "2 args expected");

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	v8::Local<v8::Value> value = info[1];

	alt::ICore::Instance().SetMetaData(*v8::String::Utf8Value(isolate, key), V8Helpers::V8ToMValue(value));
}

static void DeleteMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	alt::ICore::Instance().DeleteMetaData(*v8::String::Utf8Value(isolate, key));
}

static void HasSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, alt::ICore::Instance().HasSyncedMetaData(*v8::String::Utf8Value(isolate, key))));
}

static void GetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::String> key = info[0]->ToString(ctx).ToLocalChecked();

	alt::MValueConst val = alt::ICore::Instance().GetSyncedMetaData(*v8::String::Utf8Value(isolate, key));
	info.GetReturnValue().Set(V8Helpers::MValueToV8(val));
}

static void Log(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	std::stringstream ss;

	for (int i = 0; i < info.Length(); ++i)
	{
		v8::Local<v8::Value> val = info[i];

		if (i > 0)
			ss << " ";

		ss << *v8::String::Utf8Value(isolate, val->ToString(ctx).ToLocalChecked());
	}

	alt::ICore::Instance().LogColored(ss.str());
}

static void LogWarning(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	std::stringstream ss;

	for (int i = 0; i < info.Length(); ++i)
	{
		v8::Local<v8::Value> val = info[i];

		if (i > 0)
			ss << " ";

		ss << *v8::String::Utf8Value(isolate, val->ToString(ctx).ToLocalChecked());
	}

	alt::ICore::Instance().LogWarning(ss.str());
}

static void LogError(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	std::stringstream ss;

	for (int i = 0; i < info.Length(); ++i)
	{
		v8::Local<v8::Value> val = info[i];

		if (i > 0)
			ss << " ";

		ss << *v8::String::Utf8Value(isolate, val->ToString(ctx).ToLocalChecked());
	}

	alt::ICore::Instance().LogError(ss.str());
}

static void SetTimeout(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsFunction(), "function expected");
	V8_CHECK(info[1]->IsNumber(), "number expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	v8::Local<v8::Function> callback = info[0].As<v8::Function>();
	v8::Local<v8::Uint32> time = info[1]->ToUint32(ctx).ToLocalChecked();

	uint32_t id = resource->CreateTimer(ctx, callback, time->Value(), true, V8::SourceLocation::GetCurrent(isolate));

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, id));
}

static void SetInterval(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsFunction(), "function expected");
	V8_CHECK(info[1]->IsNumber(), "number expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	v8::Local<v8::Function> callback = info[0].As<v8::Function>();
	v8::Local<v8::Uint32> time = info[1]->ToUint32(ctx).ToLocalChecked();

	uint32_t id = resource->CreateTimer(ctx, callback, time->Value(), false, V8::SourceLocation::GetCurrent(isolate));

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, id));
}

static void NextTick(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsFunction(), "function expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	v8::Local<v8::Function> callback = info[0].As<v8::Function>();

	uint32_t id = resource->CreateTimer(ctx, callback, 0, true, V8::SourceLocation::GetCurrent(isolate));

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, id));
}

static void EveryTick(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsFunction(), "function expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	v8::Local<v8::Function> callback = info[0].As<v8::Function>();

	uint32_t id = resource->CreateTimer(ctx, callback, 0, false, V8::SourceLocation::GetCurrent(isolate));

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, id));
}

static void ClearTimer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsNumber(), "number expected");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	v8::Local<v8::Uint32> time = info[0]->ToUint32(ctx).ToLocalChecked();

	resource->RemoveTimer(time->Value());
}

void V8::RegisterSharedMain(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)
{
	v8::Isolate *isolate = ctx->GetIsolate();

	V8Helpers::RegisterFunc(exports, "hash", &HashCb);

	V8Helpers::RegisterFunc(exports, "log", &Log);
	V8Helpers::RegisterFunc(exports, "logWarning", &LogWarning);
	V8Helpers::RegisterFunc(exports, "logError", &LogError);

	V8Helpers::RegisterFunc(exports, "on", &On);
	V8Helpers::RegisterFunc(exports, "off", &Off);
	V8Helpers::RegisterFunc(exports, "emit", &Emit);

	V8Helpers::RegisterFunc(exports, "hasMeta", &HasMeta);
	V8Helpers::RegisterFunc(exports, "getMeta", &GetMeta);
	V8Helpers::RegisterFunc(exports, "setMeta", &SetMeta);
	V8Helpers::RegisterFunc(exports, "deleteMeta", &DeleteMeta);

	V8Helpers::RegisterFunc(exports, "hasSyncedMeta", &HasSyncedMeta);
	V8Helpers::RegisterFunc(exports, "getSyncedMeta", &GetSyncedMeta);

	V8Helpers::RegisterFunc(exports, "nextTick", &NextTick);
	V8Helpers::RegisterFunc(exports, "everyTick", &EveryTick);
	V8Helpers::RegisterFunc(exports, "setTimeout", &SetTimeout);
	V8Helpers::RegisterFunc(exports, "setInterval", &SetInterval);
	V8Helpers::RegisterFunc(exports, "clearTimer", &ClearTimer);
	V8Helpers::RegisterFunc(exports, "clearNextTick", &ClearTimer);
	V8Helpers::RegisterFunc(exports, "clearEveryTick", &ClearTimer);
	V8Helpers::RegisterFunc(exports, "clearTimeout", &ClearTimer);
	V8Helpers::RegisterFunc(exports, "clearInterval", &ClearTimer);

	V8::DefineOwnProperty(isolate, ctx, exports, "DefaultDimension", v8::Integer::New(isolate, alt::DEFAULT_DIMENSION));
	V8::DefineOwnProperty(isolate, ctx, exports, "GlobalDimension", v8::Integer::New(isolate, alt::GLOBAL_DIMENSION));

#ifdef ALT_CLIENT
	V8::DefineOwnProperty(isolate, ctx, exports, "Version", v8::String::NewFromUtf8(isolate, alt::ICore::Instance().GetVersion().CStr()).ToLocalChecked());
	V8::DefineOwnProperty(isolate, ctx, exports, "Branch", v8::String::NewFromUtf8(isolate, alt::ICore::Instance().GetBranch().CStr()).ToLocalChecked());
#endif
}
