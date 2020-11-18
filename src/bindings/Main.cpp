
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/objects/IPlayer.h"
#include "../helpers/V8Module.h"

#include "cpp-sdk/SDK.h"

#include "../helpers/Log.h"

using namespace alt;

static void OnServer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, eventName);
	V8_ARG_TO_FUNCTION(2, callback);

	resource->SubscribeRemote(eventName.ToString(), callback, V8::SourceLocation::GetCurrent(isolate));
}

static void OffServer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, eventName);
	V8_ARG_TO_FUNCTION(2, callback);

	resource->UnsubscribeRemote(eventName.ToString(), callback);
}

static void EmitServer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, eventName);
	
	alt::MValueArgs args;

	for (int i = 1; i < info.Length(); ++i)
		args.Push(V8Helpers::V8ToMValue(info[i]));

	alt::ICore::Instance().TriggerServerEvent(eventName.ToString(), args);
}

static void GameControlsEnabled(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);
	V8_RETURN_BOOLEAN(alt::ICore::Instance().AreControlsEnabled());
}

static void ToggleGameControls(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_BOOLEAN(1, state);

	CV8ResourceImpl *jsResource = static_cast<CV8ResourceImpl *>(resource);
	jsResource->ToggleGameControls(state);
}

static void ToggleVoiceControls(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_BOOLEAN(1, state);

	// TODO: make it resource-bound
	//jsResource->ToggleVoiceControls(state);
	alt::ICore::Instance().ToggleVoiceControls(state);
}

static void ShowCursor(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_BOOLEAN(1, state);

	CV8ResourceImpl *jsResource = static_cast<CV8ResourceImpl *>(resource);

	if (!jsResource->ToggleCursor(state))
	{
		if (alt::ICore::Instance().IsDebug())
		{
			V8Helpers::Throw(isolate, "Cursor state can't go < 0");
		}
		else
		{
			Log::Warning << "Cursor state can't go < 0";
		}
	}
}

static void GetCursorPos(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	alt::Vector2i cursorPos = alt::ICore::Instance().GetCursorPosition();

	V8_NEW_OBJECT(pos);
	V8_OBJECT_SET_INTEGER(pos, "x", cursorPos[0]);
	V8_OBJECT_SET_INTEGER(pos, "y", cursorPos[1]);
	
	V8_RETURN(pos);
}

static void SetCursorPos(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_OBJECT(1, pos);
	V8_OBJECT_GET_INTEGER(pos, "x", x);
	V8_OBJECT_GET_INTEGER(pos, "y", y);

	ICore::Instance().SetCursorPosition({ x, y });
}

static void LoadModel(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	Log::Warning << "loadModel is deprecated, this function has no effect" << Log::Endl;
}

static void LoadModelAsync(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	Log::Warning << "loadModelAsync is deprecated, this function has no effect" << Log::Endl;
}

static void IsTextureExistInArchetype(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_INTEGER(1, modelHash);
	V8_ARG_TO_STRING(2, modelName);

	V8_RETURN_BOOLEAN(nullptr != ICore::Instance().GetTextureFromDrawable(modelHash, modelName));
}

static void RequestIPL(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, iplName);

	ICore::Instance().RequestIPL(iplName);
}

static void RemoveIPL(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, iplName);

	ICore::Instance().RemoveIPL(iplName);
}

static void GetLicenseHash(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_RETURN_STRING(ICore::Instance().GetLicenseHash().CStr());
}

static void SetCamFrozen(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_BOOLEAN(1, state);

	ICore::Instance().SetCamFrozen(state);
}

static void IsInDebug(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_RETURN_BOOLEAN(alt::ICore::Instance().IsDebug());
}

static void IsVoiceActivityInputEnabled(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_RETURN_BOOLEAN(ICore::Instance().IsVoiceActivationEnabled());
}

static void AddGxtText(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, key);
	V8_ARG_TO_STRING(2, textValue);

	static_cast<CV8ResourceImpl *>(resource)->AddGxtText(ICore::Instance().Hash(key), textValue.ToString());
}

static void RemoveGxtText(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	static_cast<CV8ResourceImpl *>(resource)->RemoveGxtText(ICore::Instance().Hash(key));
}

static void GetGxtText(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, key);

	V8_RETURN_STRING(static_cast<CV8ResourceImpl*>(resource)->GetGxtText(ICore::Instance().Hash(key)).c_str());
}

static void GetMsPerGameMinute(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_RETURN_INTEGER(ICore::Instance().GetMsPerGameMinute());
}

static void SetMsPerGameMinute(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, ms);

	ICore::Instance().SetMsPerGameMinute(ms);
}

static void BeginScaleformMovieMethodMinimap(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, methodName);

	V8_RETURN_BOOLEAN(ICore::Instance().BeginScaleformMovieMethodMinimap(methodName.CStr()));
}

static void GetLocale(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_RETURN_STRING(ICore::Instance().GetLocale().CStr());
}

static void SetWeatherCycle(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsArray(), "weathers must be an array");
	V8_CHECK(info[1]->IsArray(), "timeMultipliers must be an array");

	v8::Local<v8::Array> weathers = info[0].As<v8::Array>();
	v8::Local<v8::Array> multipliers = info[1].As<v8::Array>();

	V8_CHECK(weathers->Length() <= 256, "weathers count must be <= 256");
	V8_CHECK(multipliers->Length() <= 256, "multipliers count must be <= 256");
	V8_CHECK(weathers->Length() == multipliers->Length(), "weathers count and multipliers count must be the same");

	Array<uint8_t> weathersVec;
	Array<uint8_t> multipliersVec;

	for (int i = 0; i < weathers->Length(); ++i)
	{
		v8::Local<v8::Value> weatherVal = weathers->Get(ctx, i).ToLocalChecked();
		uint32_t weatherNum = weatherVal->ToUint32(ctx).ToLocalChecked()->Value();
		V8_CHECK(weatherNum >= 0 && weatherNum <= 14, "weather ids must be >= 0 && <= 14");
		weathersVec.Push(weatherNum);

		v8::Local<v8::Value> multiplierVal = multipliers->Get(ctx, i).ToLocalChecked();
		uint32_t multiplierNum = multiplierVal->ToUint32(ctx).ToLocalChecked()->Value();
		V8_CHECK(multiplierNum > 0 && multiplierNum <= 720, "multipliers must be > 0 && <= 720");
		multipliersVec.Push(multiplierNum);
	}

	ICore::Instance().SetWeatherCycle(weathersVec, multipliersVec);
}

static void SetWeatherSyncActive(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_BOOLEAN(1, isActive);

	ICore::Instance().SetWeatherSyncActive(isActive);
}

static void SetCharStat(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, statName);

	IStatData *targetStat = alt::ICore::Instance().GetStatData(statName);
	V8_CHECK(targetStat != nullptr, "stat with this name not found");

	V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be edited");

	if (!strcmp(targetStat->GetStatType(), "INT"))
	{
		V8_ARG_TO_INTEGER(2, value);
		targetStat->SetInt32Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "INT64"))
	{
		V8_ARG_TO_INT64(2, value);
		targetStat->SetInt64Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "TEXTLABEL"))
	{
		V8_ARG_TO_INT32(2, value);
		targetStat->SetInt32Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "FLOAT"))
	{
		V8_ARG_TO_NUMBER(2, value);
		targetStat->SetFloatValue(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "BOOL"))
	{
		V8_ARG_TO_BOOLEAN(2, value);
		targetStat->SetBoolValue(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "STRING"))
	{
		V8_ARG_TO_STRING(2, value);
		targetStat->SetStringValue(value.CStr());
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT8"))
	{
		V8_ARG_TO_UINT32(2, value);
		targetStat->SetUInt8Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT16"))
	{
		V8_ARG_TO_UINT32(2, value);
		targetStat->SetUInt16Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT32"))
	{
		V8_ARG_TO_UINT32(2, value);
		targetStat->SetUInt32Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}
	else if (
		!strcmp(targetStat->GetStatType(), "UINT64") ||
		!strcmp(targetStat->GetStatType(), "POS") ||
		!strcmp(targetStat->GetStatType(), "DATE") ||
		!strcmp(targetStat->GetStatType(), "PACKED") ||
		!strcmp(targetStat->GetStatType(), "USERID"))
	{
		V8_ARG_TO_UINT64(2, value);
		targetStat->SetUInt64Value(value);
		V8_RETURN_BOOLEAN(true);
		return;
	}

	V8_RETURN_BOOLEAN(false);
}

static void GetCharStat(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, statName);

	IStatData *targetStat = alt::ICore::Instance().GetStatData(statName);
	V8_CHECK(targetStat != nullptr, "stat with this name not found");

	V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be readed");

	if (!strcmp(targetStat->GetStatType(), "INT"))
	{
		V8_RETURN_INT32(targetStat->GetInt32Value());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "INT64"))
	{
		V8_RETURN_INT64(targetStat->GetInt64Value());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "TEXTLABEL"))
	{
		V8_RETURN_INT32(targetStat->GetInt32Value());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "FLOAT"))
	{
		V8_RETURN_NUMBER(targetStat->GetFloatValue());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "BOOL"))
	{
		V8_RETURN_BOOLEAN(targetStat->GetBoolValue());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "STRING"))
	{
		V8_RETURN_STRING(targetStat->GetStringValue());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT8"))
	{
		V8_RETURN_UINT32(targetStat->GetUInt8Value());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT16"))
	{
		V8_RETURN_UINT32(targetStat->GetUInt16Value());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT32"))
	{
		V8_RETURN_UINT32(targetStat->GetUInt32Value());
		return;
	}
	else if (
		!strcmp(targetStat->GetStatType(), "UINT64") ||
		!strcmp(targetStat->GetStatType(), "POS") ||
		!strcmp(targetStat->GetStatType(), "DATE") ||
		!strcmp(targetStat->GetStatType(), "PACKED") ||
		!strcmp(targetStat->GetStatType(), "USERID"))
	{
		V8_RETURN_UINT64(targetStat->GetUInt64Value());
		return;
	}

	V8_RETURN_NULL();
}

static void ResetCharStat(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, statName);

	IStatData *targetStat = alt::ICore::Instance().GetStatData(statName);
	V8_CHECK(targetStat != nullptr, "stat with this name not found");

	V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be reseted");
	targetStat->Reset();
	V8_RETURN_BOOLEAN(true);
}

static void IsMenuOpen(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);
	V8_RETURN_BOOLEAN(ICore::Instance().IsMenuOpen());
}

static void IsConsoleOpen(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);
	V8_RETURN_BOOLEAN(ICore::Instance().IsConsoleOpen());
}

static void IsKeyDown(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, keycode);

	V8_RETURN_BOOLEAN(alt::ICore::Instance().GetKeyState(keycode).IsDown());
}

static void IsKeyToggled(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, keycode);

	V8_RETURN_BOOLEAN(alt::ICore::Instance().GetKeyState(keycode).IsToggled());
}

static void SetConfigFlag(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_STRING(1, flag);
	V8_ARG_TO_BOOLEAN(2, state);

	ICore::Instance().SetConfigFlag(flag, state);
}

static void GetConfigFlag(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, flag);

	V8_RETURN_BOOLEAN(ICore::Instance().GetConfigFlag(flag));
}

static void DoesConfigFlagExist(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_ARG_TO_STRING(1, flag);

	V8_RETURN_BOOLEAN(ICore::Instance().DoesConfigFlagExist(flag));
}

// extern V8Class v8MemoryBuffer;
// static void GetEntityMemoryByID(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	V8_GET_ISOLATE_CONTEXT();

// #ifdef NDEBUG
// 	auto &game = CGame::Instance();
// 	V8_CHECK(game.IsDebug() && IsDevOrInternalBranch(), "Must be in debug mode and dev branch");
// #endif

// 	V8_CHECK_ARGS_LEN(1);
// 	V8_ARG_TO_INTEGER(1, id);

// 	::CEntity *addr = funcs::GetEntityFromScriptID<::CEntity *>(id);

// 	auto buf = v8MemoryBuffer.CreateInstance(ctx);
// 	buf->SetAlignedPointerInInternalField(0, addr);
// 	buf->SetInternalField(1, v8::Integer::NewFromUnsigned(isolate, UINT32_MAX));

// 	V8_RETURN(buf);
// }

// static void SetAngularVelocity(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	V8_GET_ISOLATE_CONTEXT();
// 	V8_CHECK_ARGS_LEN(4);

// 	V8_ARG_TO_INTEGER(1, id);
// 	V8_ARG_TO_NUMBER(2, x);
// 	V8_ARG_TO_NUMBER(3, y);
// 	V8_ARG_TO_NUMBER(4, z);

// 	::CDynamicEntity *ent = funcs::GetEntityFromScriptID<::CDynamicEntity *>(id);

// 	ent->SetAngularVelocity({x, y, z, 0.0});
// }

static void GetPermissionState(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, permnum);

	V8_RETURN_INTEGER((uint8_t)alt::ICore::Instance().GetPermissionState((alt::Permission)permnum));
}

static void IsInStreamerMode(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_RETURN_BOOLEAN(alt::ICore::Instance().IsInStreamerMode());
}

static void TakeScreenshot(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	static std::list<v8::UniquePersistent<v8::Promise::Resolver>> promises;

	V8_GET_ISOLATE_CONTEXT();

	auto &api = alt::ICore::Instance();
	auto state = api.GetPermissionState(alt::Permission::ScreenCapture);
	V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
	V8_CHECK(state != alt::PermissionState::Unspecified, "Permissions not specified");

	auto &persistent = promises.emplace_back(v8::UniquePersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

	api.TakeScreenshot([](alt::StringView base64, const void *userData) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolateScope(isolate);
		v8::HandleScope handleScope(isolate);

		auto persistent = (v8::UniquePersistent<v8::Promise::Resolver> *)userData;
		auto resolver = persistent->Get(isolate);
		auto ctx = resolver->CreationContext();
		{
			v8::Context::Scope ctxscope(ctx);
			resolver->Resolve(resolver->CreationContext(), v8::String::NewFromUtf8(isolate, base64.CStr()).ToLocalChecked());
		}

		promises.remove(*persistent);
	}, &persistent);

}

static void TakeScreenshotGameOnly(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	static std::list<v8::UniquePersistent<v8::Promise::Resolver>> promises;

	V8_GET_ISOLATE_CONTEXT();

	auto &api = alt::ICore::Instance();
	auto state = api.GetPermissionState(alt::Permission::ScreenCapture);
	V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
	V8_CHECK(state != alt::PermissionState::Unspecified, "Permissions not specified");

	auto &persistent = promises.emplace_back(v8::UniquePersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

	api.TakeScreenshotGameOnly([](alt::StringView base64, const void *userData) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolateScope(isolate);
		v8::HandleScope handleScope(isolate);

		auto persistent = (v8::UniquePersistent<v8::Promise::Resolver> *)userData;
		auto resolver = persistent->Get(isolate);
		auto ctx = resolver->CreationContext();
		{
			v8::Context::Scope ctxscope(ctx);
			resolver->Resolve(resolver->CreationContext(), v8::String::NewFromUtf8(isolate, base64.CStr()).ToLocalChecked());
		}

		promises.remove(*persistent);
		ctx->Exit();
	}, &persistent);

	V8_RETURN(persistent.Get(isolate)->GetPromise());
}

extern V8Class v8Vector3,
	v8RGBA,
	v8BaseObject,
	v8WorldObject,
	v8Entity,
	v8Player,
	v8Player,
	v8Vehicle,
	v8WebView,
	v8Blip,
	v8AreaBlip,
	v8RadiusBlip,
	v8PointBlip,
	v8HandlingData,
	v8LocalStorage,
	v8MemoryBuffer,
	v8File,
	v8MapZoomData,
	v8Discord,
	v8Voice,
	v8PedBlip,
	v8VehicleBlip;
extern V8Module altModule(
	"alt",
	{v8Vector3,
	 v8RGBA,
	 v8BaseObject,
	 v8WorldObject,
	 v8Entity,
	 v8Player,
	 v8Vehicle,
	 v8WebView,
	 v8Blip,
	 v8AreaBlip,
	 v8RadiusBlip,
	 v8PointBlip,
	 v8HandlingData,
	 v8LocalStorage,
	 //  v8MemoryBuffer,
	 v8File,
	 //  v8MapZoomData,
	 v8Discord,
	 v8Voice},
	[](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports) {
		V8::RegisterSharedMain(ctx, exports);

		V8Helpers::RegisterFunc(exports, "onServer", &OnServer);
		V8Helpers::RegisterFunc(exports, "offServer", &OffServer);
		V8Helpers::RegisterFunc(exports, "emitServer", &EmitServer);
		V8Helpers::RegisterFunc(exports, "gameControlsEnabled", &GameControlsEnabled);
		V8Helpers::RegisterFunc(exports, "toggleGameControls", &ToggleGameControls);
		V8Helpers::RegisterFunc(exports, "toggleVoiceControls", &ToggleVoiceControls);
		V8Helpers::RegisterFunc(exports, "showCursor", &ShowCursor);
		
		V8Helpers::RegisterFunc(exports, "getCursorPos", &GetCursorPos);
		V8Helpers::RegisterFunc(exports, "setCursorPos", &SetCursorPos);
		V8Helpers::RegisterFunc(exports, "isMenuOpen", &IsMenuOpen);
		V8Helpers::RegisterFunc(exports, "isConsoleOpen", &IsConsoleOpen);
		//V8Helpers::RegisterFunc(exports, "drawRect2D", &DrawRect2D);

		V8Helpers::RegisterFunc(exports, "requestIpl", &RequestIPL);
		V8Helpers::RegisterFunc(exports, "removeIpl", &RemoveIPL);
		//V8Helpers::RegisterFunc(exports, "wait", &ScriptWait);
		//V8Helpers::RegisterFunc(exports, "isInSandbox", &IsInSandbox);
		V8Helpers::RegisterFunc(exports, "isInDebug", &IsInDebug);
		V8Helpers::RegisterFunc(exports, "setCamFrozen", &SetCamFrozen);

		V8Helpers::RegisterFunc(exports, "getLicenseHash", &GetLicenseHash);

		//Gxt texts functions
		V8Helpers::RegisterFunc(exports, "addGxtText", &AddGxtText);
		V8Helpers::RegisterFunc(exports, "removeGxtText", &RemoveGxtText);
		V8Helpers::RegisterFunc(exports, "getGxtText", &GetGxtText);

		//Voice functions
		V8Helpers::RegisterFunc(exports, "isVoiceActivityInputEnabled", &IsVoiceActivityInputEnabled);

		//Time managements functions
		V8Helpers::RegisterFunc(exports, "setMsPerGameMinute", &SetMsPerGameMinute);
		V8Helpers::RegisterFunc(exports, "getMsPerGameMinute", &GetMsPerGameMinute);

		//CEF rendering on texture
		V8Helpers::RegisterFunc(exports, "isTextureExistInArchetype", &IsTextureExistInArchetype);

		//Scaleform additionals
		V8Helpers::RegisterFunc(exports, "beginScaleformMovieMethodMinimap", &BeginScaleformMovieMethodMinimap);

#ifndef NDEBUG
	//   V8Helpers::RegisterFunc(exports, "getVehWheels", &GetVehWheels);
#endif

		V8Helpers::RegisterFunc(exports, "getLocale", &GetLocale);

		V8Helpers::RegisterFunc(exports, "setWeatherCycle", &SetWeatherCycle);
		V8Helpers::RegisterFunc(exports, "setWeatherSyncActive", &SetWeatherSyncActive);

		V8Helpers::RegisterFunc(exports, "setStat", &SetCharStat);
		V8Helpers::RegisterFunc(exports, "getStat", &GetCharStat);
		V8Helpers::RegisterFunc(exports, "resetStat", &ResetCharStat);

		V8Helpers::RegisterFunc(exports, "isKeyDown", &IsKeyDown);
		V8Helpers::RegisterFunc(exports, "isKeyToggled", &IsKeyToggled);

		V8Helpers::RegisterFunc(exports, "setConfigFlag", &SetConfigFlag);
		V8Helpers::RegisterFunc(exports, "getConfigFlag", &GetConfigFlag);
		V8Helpers::RegisterFunc(exports, "doesConfigFlagExist", &DoesConfigFlagExist);

		//   V8Helpers::RegisterFunc(exports, "getEntityMemoryByID", &GetEntityMemoryByID);

		// V8Helpers::RegisterFunc(exports, "setRotationVelocity", &SetAngularVelocity);
		// V8Helpers::RegisterFunc(exports, "setAngularVelocity", &SetAngularVelocity);

		V8Helpers::RegisterFunc(exports, "isInStreamerMode", &IsInStreamerMode);

		V8Helpers::RegisterFunc(exports, "takeScreenshot", &TakeScreenshot);
		V8Helpers::RegisterFunc(exports, "takeScreenshotGameOnly", &TakeScreenshotGameOnly);
	});
