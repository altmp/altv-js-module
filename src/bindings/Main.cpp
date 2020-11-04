
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/objects/IPlayer.h"
#include "../helpers/V8Module.h"

#include "cpp-sdk/SDK.h"

#include "../helpers/Log.h"

using namespace alt;

static void OnServer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "onServer expects 2 args");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");
	V8_CHECK(info[1]->IsFunction(), "callback must be a function");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	v8::Local<v8::Function> callback = info[1].As<v8::Function>();

	resource->SubscribeRemote(evName, callback, V8::SourceLocation::GetCurrent(isolate));
}

static void OffServer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "onServer expects 2 args");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");
	V8_CHECK(info[1]->IsFunction(), "callback must be a function");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	v8::Local<v8::Function> callback = info[1].As<v8::Function>();

	resource->UnsubscribeRemote(evName, callback);
}

static void EmitServer(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() >= 1, "emitServer expects at least 1 arg");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");

	std::string name = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	alt::MValueArgs args;

	for (int i = 1; i < info.Length(); ++i)
		args.Push(V8Helpers::V8ToMValue(info[i]));

	alt::ICore::Instance().TriggerServerEvent(name, args);
}

/**
type: 'function',
name: 'gameControlsEnabled',
description: 'Returns a bool if is game controls enabled',
returns: {
	dataType: 'bool',
	description: 'If is game controls enabled'
}
*/
static void GameControlsEnabled(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), alt::ICore::Instance().AreControlsEnabled()));
}

/**
type: 'function',
name: 'toggleGameControls',
description: 'Toggles a game controls',
parameters: [
	{
		name: 'state',
		dataType: 'bool',
		description: '`true` for enable controls, `false` to disable controls'
	}
]
*/
static void ToggleGameControls(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "toggleGameControls expects 1 arg");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	CV8ResourceImpl *jsResource = static_cast<CV8ResourceImpl *>(resource);
	bool state = info[0]->ToBoolean(isolate)->Value();

	jsResource->ToggleGameControls(state);
}

static void ToggleVoiceControls(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "toggleVoiceControls expects 1 arg");

	/*V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	CV8ResourceImpl* jsResource = static_cast<CV8ResourceImpl*>(resource);*/

	bool state = info[0]->ToBoolean(isolate)->Value();

	// TODO: make it resource-bound
	//jsResource->ToggleGameControls(state);
	alt::ICore::Instance().ToggleVoiceControls(state);
}

static void ShowCursor(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() >= 1, "showCursor expects 1 or 2 args");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	CV8ResourceImpl *jsResource = static_cast<CV8ResourceImpl *>(resource);
	bool state = info[0]->ToBoolean(isolate)->Value();

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

static void CursorPosGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	alt::Vector2i pos = alt::ICore::Instance().GetCursorPosition();

	v8::Local<v8::Object> obj = v8::Object::New(isolate);

	obj->Set(ctx, v8::String::NewFromUtf8(isolate, "x").ToLocalChecked(), v8::Integer::New(isolate, pos[0]));
	obj->Set(ctx, v8::String::NewFromUtf8(isolate, "y").ToLocalChecked(), v8::Integer::New(isolate, pos[1]));

	info.GetReturnValue().Set(obj);
}

static void CursorPosSetter(v8::Local<v8::Name> name, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	V8_CHECK(val->IsObject(), "cursorPos must be a object");

	v8::Local<v8::Value> x = val.As<v8::Object>()->Get(ctx, v8::String::NewFromUtf8(isolate, "x").ToLocalChecked()).ToLocalChecked();
	v8::Local<v8::Value> y = val.As<v8::Object>()->Get(ctx, v8::String::NewFromUtf8(isolate, "y").ToLocalChecked()).ToLocalChecked();

	ICore::Instance().SetCursorPosition({x->ToInteger(ctx).ToLocalChecked()->Value(),
										 y->ToInteger(ctx).ToLocalChecked()->Value()});
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
	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 2, "2 args expected");

	v8::Local<v8::Integer> modelHash = info[0]->ToInteger(ctx).ToLocalChecked();
	std::string modelName = *v8::String::Utf8Value(info.GetIsolate(), info[1].As<v8::String>());

	void *texture = ICore::Instance().GetTextureFromDrawable(modelHash->Value(), modelName);

	info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), texture != nullptr));
}

static void RequestIPL(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	std::string iplName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	ICore::Instance().RequestIPL(iplName.c_str());
}

static void RemoveIPL(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	std::string iplName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	ICore::Instance().RemoveIPL(iplName.c_str());
}

static void GetLicenseHash(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	auto ctx = isolate->GetEnteredContext();

	auto licenseHash = ICore::Instance().GetLicenseHash();
	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, licenseHash.CStr()).ToLocalChecked());
}

static void SetCamFrozen(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::Boolean> state = info[0]->ToBoolean(isolate);

	ICore::Instance().SetCamFrozen(state->Value());
}

// static void IsInSandbox(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), CGame::Instance().IsSandbox()));
// }

static void IsInDebug(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), alt::ICore::Instance().IsDebug()));
}

static void IsVoiceActivityInputEnabled(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), ICore::Instance().IsVoiceActivationEnabled()));
}

static void AddGxtText(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 2, "2 args expected");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	std::string textValue = *v8::String::Utf8Value(info.GetIsolate(), info[1].As<v8::String>());

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");
	static_cast<CV8ResourceImpl *>(resource)->AddGxtText(ICore::Instance().Hash(key), textValue);
}

static void RemoveGxtText(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
	V8_CHECK(resource, "Invalid resource");
	static_cast<CV8ResourceImpl *>(resource)->RemoveGxtText(ICore::Instance().Hash(key));
}

static void GetGxtText(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	std::string key = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");
	std::string text = static_cast<CV8ResourceImpl *>(resource)->GetGxtText(ICore::Instance().Hash(key));
	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, text.c_str()).ToLocalChecked());
}

static void GetMsPerGameMinute(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	int32_t ms = ICore::Instance().GetMsPerGameMinute();
	info.GetReturnValue().Set(v8::Integer::New(isolate, ms));
}

static void SetMsPerGameMinute(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	v8::Local<v8::Integer> ms = info[0]->ToInteger(ctx).ToLocalChecked();
	ICore::Instance().SetMsPerGameMinute(ms->Value());
}

static void BeginScaleformMovieMethodMinimap(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	std::string methodName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	bool result = ICore::Instance().BeginScaleformMovieMethodMinimap(methodName.c_str());
	info.GetReturnValue().Set(v8::Boolean::New(isolate, result));
}

static void GetLocale(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	auto locale = ICore::Instance().GetLocale();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, locale.CStr()).ToLocalChecked());
}

static void SetWeatherCycle(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

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
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsBoolean(), "activeState must be boolean");

	v8::Local<v8::Boolean> isActive = info[0].As<v8::Boolean>();

	ICore::Instance().SetWeatherSyncActive(isActive->Value());
}

static void SetCharStat(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "statName must be string");

	std::string statName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	IStatData *targetStat = alt::ICore::Instance().GetStatData(statName);
	V8_CHECK(targetStat != nullptr, "stat with this name not found");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be edited");

	if (!strcmp(targetStat->GetStatType(), "INT"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		int32_t intValue = info[1]->ToInt32(ctx).ToLocalChecked()->Value();
		targetStat->SetInt32Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "INT64"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		int64_t intValue = info[1]->ToBigInt(ctx).ToLocalChecked()->Int64Value();
		targetStat->SetInt64Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "TEXTLABEL"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		int32_t intValue = info[1]->ToInt32(ctx).ToLocalChecked()->Value();
		targetStat->SetInt32Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "FLOAT"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		float floatValue = info[1]->ToNumber(ctx).ToLocalChecked()->Value();
		targetStat->SetFloatValue(floatValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "BOOL"))
	{
		V8_CHECK(info[1]->IsBoolean(), "value must be boolean");
		bool boolValue = info[1]->ToBoolean(isolate)->Value();
		targetStat->SetBoolValue(boolValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "STRING"))
	{
		V8_CHECK(info[1]->IsString(), "value must be string");
		std::string stringValue = *v8::String::Utf8Value(info.GetIsolate(), info[1].As<v8::String>());
		targetStat->SetStringValue(stringValue.c_str());
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT8"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		uint8_t intValue = info[1]->ToUint32(ctx).ToLocalChecked()->Value();
		targetStat->SetUInt8Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT16"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		uint16_t intValue = info[1]->ToUint32(ctx).ToLocalChecked()->Value();
		targetStat->SetUInt16Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT32"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		uint32_t intValue = info[1]->ToUint32(ctx).ToLocalChecked()->Value();
		targetStat->SetUInt32Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	else if (
		!strcmp(targetStat->GetStatType(), "UINT64") ||
		!strcmp(targetStat->GetStatType(), "POS") ||
		!strcmp(targetStat->GetStatType(), "DATE") ||
		!strcmp(targetStat->GetStatType(), "PACKED") ||
		!strcmp(targetStat->GetStatType(), "USERID"))
	{
		V8_CHECK(info[1]->IsNumber(), "value must be number");
		uint64_t intValue = info[1]->ToBigInt(ctx).ToLocalChecked()->Uint64Value();
		targetStat->SetUInt64Value(intValue);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}

	info.GetReturnValue().Set(v8::Boolean::New(isolate, false));
}

static void GetCharStat(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "statName must be string");

	std::string statName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	IStatData *targetStat = alt::ICore::Instance().GetStatData(statName);
	V8_CHECK(targetStat != nullptr, "stat with this name not found");

	V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be readed");

	if (!strcmp(targetStat->GetStatType(), "INT"))
	{
		int32_t intValue = targetStat->GetInt32Value();
		info.GetReturnValue().Set(v8::Integer::New(isolate, intValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "INT64"))
	{
		int64_t intValue = targetStat->GetInt64Value();
		info.GetReturnValue().Set(v8::BigInt::New(isolate, intValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "TEXTLABEL"))
	{
		int32_t intValue = targetStat->GetInt32Value();
		info.GetReturnValue().Set(v8::Integer::New(isolate, intValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "FLOAT"))
	{
		float floatValue = targetStat->GetFloatValue();
		info.GetReturnValue().Set(v8::Number::New(isolate, floatValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "BOOL"))
	{
		bool boolValue = targetStat->GetBoolValue();
		info.GetReturnValue().Set(v8::Boolean::New(isolate, boolValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "STRING"))
	{
		const char *stringValue = targetStat->GetStringValue();
		info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, stringValue).ToLocalChecked());
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT8"))
	{
		uint8_t intValue = targetStat->GetUInt8Value();
		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, intValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT16"))
	{
		uint16_t intValue = targetStat->GetUInt16Value();
		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, intValue));
		return;
	}
	else if (!strcmp(targetStat->GetStatType(), "UINT32"))
	{
		uint32_t intValue = targetStat->GetUInt32Value();
		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, intValue));
		return;
	}
	else if (
		!strcmp(targetStat->GetStatType(), "UINT64") ||
		!strcmp(targetStat->GetStatType(), "POS") ||
		!strcmp(targetStat->GetStatType(), "DATE") ||
		!strcmp(targetStat->GetStatType(), "PACKED") ||
		!strcmp(targetStat->GetStatType(), "USERID"))
	{
		uint64_t intValue = targetStat->GetUInt64Value();
		info.GetReturnValue().Set(v8::BigInt::NewFromUnsigned(isolate, intValue));
		return;
	}

	info.GetReturnValue().Set(v8::Null(isolate));
}

static void ResetCharStat(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "statName must be string");

	std::string statName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
	IStatData *targetStat = alt::ICore::Instance().GetStatData(statName);
	V8_CHECK(targetStat != nullptr, "stat with this name not found");

	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be reseted");
	targetStat->Reset();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
}

static void IsMenuOpen(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, ICore::Instance().IsMenuOpen()));
}

static void IsConsoleOpen(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	info.GetReturnValue().Set(v8::Boolean::New(isolate, ICore::Instance().IsConsoleOpen()));
}

static void IsKeyDown(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	uint32_t keycode = info[0]->ToUint32(ctx).ToLocalChecked()->Value();

	info.GetReturnValue().Set(v8::Boolean::New(isolate, alt::ICore::Instance().GetKeyState(keycode).IsDown()));
}

static void IsKeyToggled(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	uint32_t keycode = info[0]->ToUint32(ctx).ToLocalChecked()->Value();

	info.GetReturnValue().Set(v8::Boolean::New(isolate, alt::ICore::Instance().GetKeyState(keycode).IsToggled()));
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
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsUint32(), "1st arg must be permission id");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	auto permnum = info[0].As<v8::Uint32>()->Value();
	V8_CHECK(permnum < (uint32_t)alt::Permission::All, "Invalid permission");
	auto perm = (alt::Permission)permnum;

	auto state = alt::ICore::Instance().GetPermissionState(perm);
	info.GetReturnValue().Set(v8::Uint32::NewFromUnsigned(isolate, (uint8_t)state));
}

static void IsInStreamerMode(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	info.GetReturnValue().Set(v8::Boolean::New(isolate, alt::ICore::Instance().IsInStreamerMode()));
}

static void TakeScreenshot(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	static std::list<v8::UniquePersistent<v8::Promise::Resolver>> promises;
	static v8::Isolate *_isolate = v8::Isolate::GetCurrent();

	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	auto ctx = isolate->GetEnteredContext();

	V8_CHECK(ICore::Instance().IsDebug(), "Must be in debug mode");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	auto &api = alt::ICore::Instance();
	auto state = api.GetPermissionState(alt::Permission::ScreenCapture);
	V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
	V8_CHECK(state != alt::PermissionState::Unspecified, "Permissions not specified");

	auto &persistent = promises.emplace_back(v8::UniquePersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

	api.TakeScreenshot([](alt::StringView base64, const void *userData) {
		v8::Isolate *isolate = _isolate;
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
	},
					   &persistent);

	info.GetReturnValue().Set(persistent.Get(isolate)->GetPromise());
}

static void TakeScreenshotGameOnly(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	static std::list<v8::UniquePersistent<v8::Promise::Resolver>> promises;
	static v8::Isolate *_isolate = v8::Isolate::GetCurrent();

	v8::Isolate *isolate = v8::Isolate::GetCurrent();
	auto ctx = isolate->GetEnteredContext();

	V8_CHECK(ICore::Instance().IsDebug(), "Must be in debug mode");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	auto &api = alt::ICore::Instance();
	auto state = api.GetPermissionState(alt::Permission::ScreenCapture);
	V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
	V8_CHECK(state != alt::PermissionState::Unspecified, "Permissions not specified");

	auto &persistent = promises.emplace_back(v8::UniquePersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

	api.TakeScreenshotGameOnly([](alt::StringView base64, const void *userData) {
		v8::Isolate *isolate = _isolate;
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
	},
							   &persistent);

	info.GetReturnValue().Set(persistent.Get(isolate)->GetPromise());
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
	 //  v8Blip,
	 //  v8AreaBlip,
	 //  v8RadiusBlip,
	 //  v8PointBlip,
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
