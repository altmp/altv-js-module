
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void HashCb(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, str);

	V8_RETURN_INTEGER(alt::ICore::Instance().Hash(str));
}

static void On(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, callback);

	resource->SubscribeLocal(evName.ToString(), callback, V8::SourceLocation::GetCurrent(isolate));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, callback);

	resource->UnsubscribeLocal(evName.ToString(), callback);
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, name);

	alt::MValueArgs args;

	for (int i = 1; i < info.Length(); ++i)
		args.Push(V8Helpers::V8ToMValue(info[i]));

	alt::ICore::Instance().TriggerLocalEvent(name, args);
}

static void HasMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN(alt::ICore::Instance().HasMetaData(key));
}

static void GetMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN_MVALUE(alt::ICore::Instance().GetMetaData(key));
}

static void SetMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(2);
	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_MVALUE(2, value);

	alt::ICore::Instance().SetMetaData(key, value);
}

static void DeleteMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, key);

	alt::ICore::Instance().DeleteMetaData(key);
}

static void HasSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN(alt::ICore::Instance().HasSyncedMetaData(key));
}

static void GetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN_MVALUE(alt::ICore::Instance().GetSyncedMetaData(key));
}

static void Log(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	
	V8_CHECK_ARGS_LEN_MIN(1);

	std::stringstream ss;

	for (int i = 0; i < info.Length(); ++i)
	{
		v8::Local<v8::Value> val = info[i];

		if (i > 0)
			ss << " ";

		v8::Local<v8::String> str;
		if (val->IsObject() || val->IsArray()) {
			v8::MaybeLocal<v8::String> maybe = v8::JSON::Stringify(ctx, val);
			v8::Local<v8::String> stringified;
			if (maybe.ToLocal(&stringified)) str = stringified;
		}
		else str = val->ToString(ctx).ToLocalChecked();

		ss << *v8::String::Utf8Value(isolate, str);
	}

	alt::ICore::Instance().LogColored(ss.str());
}

static void LogWarning(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);

	std::stringstream ss;

	for (int i = 0; i < info.Length(); ++i)
	{
		v8::Local<v8::Value> val = info[i];

		if (i > 0)
			ss << " ";

		v8::Local<v8::String> str;
		if (val->IsObject() || val->IsArray()) {
			v8::MaybeLocal<v8::String> maybe = v8::JSON::Stringify(ctx, val);
			v8::Local<v8::String> stringified;
			if (maybe.ToLocal(&stringified)) str = stringified;
		}
		else str = val->ToString(ctx).ToLocalChecked();

		ss << *v8::String::Utf8Value(isolate, str);
	}

	alt::ICore::Instance().LogWarning(ss.str());
}

static void LogError(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN_MIN(1);

	std::stringstream ss;

	for (int i = 0; i < info.Length(); ++i)
	{
		v8::Local<v8::Value> val = info[i];

		if (i > 0)
			ss << " ";

		v8::Local<v8::String> str;
		if (val->IsObject() || val->IsArray()) {
			v8::MaybeLocal<v8::String> maybe = v8::JSON::Stringify(ctx, val);
			v8::Local<v8::String> stringified;
			if (maybe.ToLocal(&stringified)) str = stringified;
		}
		else str = val->ToString(ctx).ToLocalChecked();

		ss << *v8::String::Utf8Value(isolate, str);
	}

	alt::ICore::Instance().LogError(ss.str());
}

static void SetTimeout(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_FUNCTION(1, callback);
	V8_ARG_TO_INTEGER(2, time);

	V8_RETURN_INTEGER(resource->CreateTimer(ctx, callback, time, true, V8::SourceLocation::GetCurrent(isolate)));
}

static void SetInterval(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_FUNCTION(1, callback);
	V8_ARG_TO_INTEGER(2, time);

	V8_RETURN_INTEGER(resource->CreateTimer(ctx, callback, time, false, V8::SourceLocation::GetCurrent(isolate)));
}

static void NextTick(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_FUNCTION(1, callback);

	V8_RETURN_INTEGER(resource->CreateTimer(ctx, callback, 0, true, V8::SourceLocation::GetCurrent(isolate)));
}

static void EveryTick(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_FUNCTION(1, callback);

	V8_RETURN_INTEGER(resource->CreateTimer(ctx, callback, 0, false, V8::SourceLocation::GetCurrent(isolate)));
}

static void ClearTimer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_INTEGER(1, timer);

	resource->RemoveTimer(timer);
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

#ifdef ALT_SERVER_API
	V8::DefineOwnProperty(isolate, ctx, exports, "version", v8::String::NewFromUtf8(isolate, alt::ICore::Instance().GetVersion().CStr()));
	V8::DefineOwnProperty(isolate, ctx, exports, "branch", v8::String::NewFromUtf8(isolate, alt::ICore::Instance().GetBranch().CStr()));

	alt::IResource* resource = V8ResourceImpl::GetResource(ctx);
	V8::DefineOwnProperty(isolate, ctx, exports, "resourceName", v8::String::NewFromUtf8(isolate, resource->GetName().CStr()));
#else 
	V8::DefineOwnProperty(isolate, ctx, exports, "version", v8::String::NewFromUtf8(isolate, alt::ICore::Instance().GetVersion().CStr()).ToLocalChecked());
	V8::DefineOwnProperty(isolate, ctx, exports, "branch", v8::String::NewFromUtf8(isolate, alt::ICore::Instance().GetBranch().CStr()).ToLocalChecked());

	alt::IResource* resource = V8ResourceImpl::GetResource(ctx);
	V8::DefineOwnProperty(isolate, ctx, exports, "resourceName", v8::String::NewFromUtf8(isolate, resource->GetName().CStr()).ToLocalChecked());
#endif

	V8::DefineOwnProperty(isolate, ctx, exports, "sdkVersion", v8::Integer::New(isolate, alt::ICore::Instance().SDK_VERSION));
}
