
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/objects/IPlayer.h"
#include "V8Module.h"

#include "cpp-sdk/SDK.h"

#include "Log.h"

using namespace alt;

static void OnServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->SubscribeGenericRemote(callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, eventName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->SubscribeRemote(eventName, callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
    }
}

static void OnceServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->SubscribeGenericRemote(callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource), true);
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, eventName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->SubscribeRemote(eventName, callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource), true);
    }
}

static void OffServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->UnsubscribeGenericRemote(callback);
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, evName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->UnsubscribeRemote(evName, callback);
    }
}

static void EmitServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, eventName);

    alt::MValueArgs args;

    for(int i = 1; i < info.Length(); ++i) args.Push(V8Helpers::V8ToMValue(info[i], false));

    alt::ICore::Instance().TriggerServerEvent(eventName, args);
}

static void EmitServerRaw(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, eventName);

    alt::MValueArgs args;

    for(int i = 1; i < info.Length(); ++i)
    {
        v8::TryCatch tryCatch(isolate);
        alt::MValueByteArray result = V8Helpers::V8ToRawBytes(info[i]);
        if(tryCatch.HasCaught())
        {
            tryCatch.ReThrow();
            return;
        }
        V8_CHECK(!result.IsEmpty(), "Failed to serialize value");
        args.Push(result);
    }

    alt::ICore::Instance().TriggerServerEvent(eventName, args);
}

static void GameControlsEnabled(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_RETURN_BOOLEAN(alt::ICore::Instance().AreControlsEnabled());
}

static void ToggleGameControls(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, state);

    resource->ToggleGameControls(state);
}

static void ToggleVoiceControls(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, state);

    alt::ICore::Instance().ToggleVoiceControls(state);
}

static void ShowCursor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, state);

    if(!resource->ToggleCursor(state))
    {
        if(alt::ICore::Instance().IsDebug())
        {
            V8Helpers::Throw(isolate, "Cursor state can't go < 0");
        }
        else
        {
            Log::Warning << "Cursor state can't go < 0" << Log::Endl;
        }
    }
}

static void IsCursorVisible(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_RETURN_BOOLEAN(resource->CursorVisible());
}

static void GetCursorPos(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(0, 1);

    V8_ARG_TO_BOOLEAN_OPT(1, normalized, false);

    alt::Vector2f cursorPos = alt::ICore::Instance().GetCursorPosition(normalized);
    V8_RETURN_VECTOR2(cursorPos);
}

static void SetCursorPos(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(1, 2);

    V8_ARG_TO_OBJECT(1, pos);
    V8_OBJECT_GET_NUMBER(pos, "x", x);
    V8_OBJECT_GET_NUMBER(pos, "y", y);
    V8_ARG_TO_BOOLEAN_OPT(2, normalized, false);

    ICore::Instance().SetCursorPosition({ x, y }, normalized);
}

static void IsTextureExistInArchetype(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, modelHash);
    V8_ARG_TO_STRING(2, modelName);

    V8_RETURN_BOOLEAN(nullptr != ICore::Instance().GetTextureFromDrawable(modelHash, modelName));
}

static void RequestIPL(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, iplName);

    ICore::Instance().RequestIPL(iplName);
}

static void RemoveIPL(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, iplName);

    ICore::Instance().RemoveIPL(iplName);
}

static void GetLicenseHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_RETURN_STRING(ICore::Instance().GetLicenseHash());
}

static void SetCamFrozen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, state);

    ICore::Instance().SetCamFrozen(state);
}

static void IsCamFrozen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN(ICore::Instance().IsCamFrozen());
}

static void IsInDebug(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsDebug());
}

static void AddGxtText(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();
    V8_CHECK_ARGS_LEN(2);

    uint32_t gxtHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, key);
        gxtHash = alt::ICore::Instance().Hash(key);
    }
    else
    {
        V8_ARG_TO_UINT(1, hash);
        gxtHash = hash;
    }

    V8_ARG_TO_STRING(2, textValue);

    resource->AddGxtText(gxtHash, textValue);
}

static void RemoveGxtText(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    resource->RemoveGxtText(ICore::Instance().Hash(key));
}

static void GetGxtText(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_RETURN_STRING(resource->GetGxtText(ICore::Instance().Hash(key)));
}

static void GetMsPerGameMinute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_RETURN_INT(ICore::Instance().GetMsPerGameMinute());
}

static void SetMsPerGameMinute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, ms);

    ICore::Instance().SetMsPerGameMinute(ms);
}

static void BeginScaleformMovieMethodMinimap(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, methodName);

    V8_RETURN_BOOLEAN(ICore::Instance().BeginScaleformMovieMethodMinimap(methodName));
}

static void GetLocale(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_STRING(ICore::Instance().GetLocale());
}

static void SetWeatherCycle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_ARRAY(1, weathers);
    V8_ARG_TO_ARRAY(2, multipliers);

    V8_CHECK(weathers->Length() < 256, "Weathers array size must be <= 255");
    V8_CHECK(multipliers->Length() < 256, "Multipliers array size must be <= 255");
    V8_CHECK(weathers->Length() == multipliers->Length(), "Weathers and multipliers array has to be the same size");

    Array<uint8_t> weathersVec;
    Array<uint8_t> multipliersVec;

    for(int i = 0; i < weathers->Length(); ++i)
    {
        V8_TO_INTEGER(weathers->Get(ctx, i).ToLocalChecked(), weatherNum);
        V8_CHECK(weatherNum >= 0 && weatherNum <= 14, "weather ids must be >= 0 && <= 14");
        weathersVec.Push(weatherNum);

        V8_TO_INTEGER(multipliers->Get(ctx, i).ToLocalChecked(), multiplierNum);
        V8_CHECK(multiplierNum > 0 && multiplierNum < 256, "multipliers must be > 0 && <= 255");
        multipliersVec.Push(multiplierNum);
    }

    ICore::Instance().SetWeatherCycle(weathersVec, multipliersVec);
}

static void SetWeatherSyncActive(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, isActive);

    ICore::Instance().SetWeatherSyncActive(isActive);
}

static void SetCharStat(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, statName);

    IStatData* targetStat = alt::ICore::Instance().GetStatData(statName);
    V8_CHECK(targetStat != nullptr, "stat with this name not found");

    V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be edited");

    if(!strcmp(targetStat->GetStatType(), "INT"))
    {
        V8_ARG_TO_INT(2, value);
        targetStat->SetInt32Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "INT64"))
    {
        V8_ARG_TO_INT64(2, value);
        targetStat->SetInt64Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "TEXTLABEL"))
    {
        V8_ARG_TO_INT32(2, value);
        targetStat->SetInt32Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "FLOAT"))
    {
        V8_ARG_TO_NUMBER(2, value);
        targetStat->SetFloatValue(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "BOOL"))
    {
        V8_ARG_TO_BOOLEAN(2, value);
        targetStat->SetBoolValue(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "STRING"))
    {
        V8_ARG_TO_STRING(2, value);
        targetStat->SetStringValue(value.c_str());
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT8"))
    {
        V8_ARG_TO_UINT(2, value);
        targetStat->SetUInt8Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT16"))
    {
        V8_ARG_TO_UINT(2, value);
        targetStat->SetUInt16Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT32"))
    {
        V8_ARG_TO_UINT(2, value);
        targetStat->SetUInt32Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT64") || !strcmp(targetStat->GetStatType(), "POS") || !strcmp(targetStat->GetStatType(), "DATE") ||
            !strcmp(targetStat->GetStatType(), "PACKED") || !strcmp(targetStat->GetStatType(), "USERID"))
    {
        V8_ARG_TO_UINT64(2, value);
        targetStat->SetUInt64Value(value);
        V8_RETURN_BOOLEAN(true);
        return;
    }

    V8_RETURN_BOOLEAN(false);
}

static void GetCharStat(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, statName);

    IStatData* targetStat = alt::ICore::Instance().GetStatData(statName);
    V8_CHECK(targetStat != nullptr, "stat with this name not found");

    V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be readed");

    if(!strcmp(targetStat->GetStatType(), "INT"))
    {
        V8_RETURN_INT(targetStat->GetInt32Value());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "INT64"))
    {
        V8_RETURN_INT64(targetStat->GetInt64Value());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "TEXTLABEL"))
    {
        V8_RETURN_INT(targetStat->GetInt32Value());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "FLOAT"))
    {
        V8_RETURN_NUMBER(targetStat->GetFloatValue());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "BOOL"))
    {
        V8_RETURN_BOOLEAN(targetStat->GetBoolValue());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "STRING"))
    {
        V8_RETURN_RAW_STRING(targetStat->GetStringValue());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT8"))
    {
        V8_RETURN_UINT(targetStat->GetUInt8Value());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT16"))
    {
        V8_RETURN_UINT(targetStat->GetUInt16Value());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT32"))
    {
        V8_RETURN_UINT(targetStat->GetUInt32Value());
        return;
    }
    else if(!strcmp(targetStat->GetStatType(), "UINT64") || !strcmp(targetStat->GetStatType(), "POS") || !strcmp(targetStat->GetStatType(), "DATE") ||
            !strcmp(targetStat->GetStatType(), "PACKED") || !strcmp(targetStat->GetStatType(), "USERID"))
    {
        V8_RETURN_UINT64(targetStat->GetUInt64Value());
        return;
    }

    V8_RETURN_NULL();
}

static void ResetCharStat(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, statName);

    IStatData* targetStat = alt::ICore::Instance().GetStatData(statName);
    V8_CHECK(targetStat != nullptr, "stat with this name not found");

    V8_CHECK(strcmp(targetStat->GetStatType(), "NONE") != 0 && strcmp(targetStat->GetStatType(), "PROFILE_SETTING") != 0, "target stat can't be reseted");
    targetStat->Reset();
    V8_RETURN_BOOLEAN(true);
}

static void IsMenuOpen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_RETURN_BOOLEAN(ICore::Instance().IsMenuOpen());
}

static void IsConsoleOpen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_RETURN_BOOLEAN(ICore::Instance().IsConsoleOpen());
}

static void IsKeyDown(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, keycode);

    V8_RETURN_BOOLEAN(alt::ICore::Instance().GetKeyState(keycode).IsDown());
}

static void IsKeyToggled(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, keycode);

    V8_RETURN_BOOLEAN(alt::ICore::Instance().GetKeyState(keycode).IsToggled());
}

static void SetConfigFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, flag);
    V8_ARG_TO_BOOLEAN(2, state);

    ICore::Instance().SetConfigFlag(flag, state);
}

static void GetConfigFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, flag);

    V8_RETURN_BOOLEAN(ICore::Instance().GetConfigFlag(flag));
}

static void DoesConfigFlagExist(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, flag);

    V8_RETURN_BOOLEAN(ICore::Instance().DoesConfigFlagExist(flag));
}

static void LoadYtyp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, path);

    V8_RETURN_BOOLEAN(ICore::Instance().LoadYtyp(path));
}

static void UnloadYtyp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, path);

    V8_RETURN_BOOLEAN(ICore::Instance().UnloadYtyp(path));
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
// 	V8_ARG_TO_INT(1, id);

// 	::CEntity *addr = funcs::GetEntityFromScriptID<::CEntity *>(id);

// 	auto buf = v8MemoryBuffer.CreateInstance(ctx);
// 	buf->SetAlignedPointerInInternalField(0, addr);
// 	buf->SetInternalField(1, v8::Integer::NewFromUnsigned(isolate, UINT32_MAX));

// 	V8_RETURN(buf);
// }

static void SetAngularVelocity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(4);

    V8_ARG_TO_INT(1, id);
    V8_ARG_TO_NUMBER(2, x);
    V8_ARG_TO_NUMBER(3, y);
    V8_ARG_TO_NUMBER(4, z);

    alt::ICore::Instance().SetAngularVelocity(id, { x, y, z, 0.0 });
}

static void GetPermissionState(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, permnum);

    V8_RETURN_INT((uint8_t)alt::ICore::Instance().GetPermissionState((alt::Permission)permnum));
}

static void IsInStreamerMode(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsInStreamerMode());
}

static void TakeScreenshot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    static std::list<v8::Global<v8::Promise::Resolver>> promises;
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    auto& persistent = promises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

    alt::PermissionState state = alt::ICore::Instance().TakeScreenshot(
      [&persistent, resource](const std::string& base64)
      {
          resource->RunOnNextTick(
            [&persistent, resource, base64Str = base64]()
            {
                if(!resource->GetResource()->IsStarted())
                {
                    promises.remove(persistent);
                    return;
                }
                persistent.Get(resource->GetIsolate())->Resolve(resource->GetContext(), V8Helpers::JSValue(base64Str));
                promises.remove(persistent);
            });
      });
    V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
    V8_CHECK(state != alt::PermissionState::Unspecified, "Permissions not specified");

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void TakeScreenshotGameOnly(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    static std::list<v8::Global<v8::Promise::Resolver>> promises;
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    auto& persistent = promises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

    alt::PermissionState state = alt::ICore::Instance().TakeScreenshotGameOnly(
      [&persistent, resource](const std::string& base64)
      {
          resource->RunOnNextTick(
            [&persistent, resource, base64Str = base64]()
            {
                if(!resource->GetResource()->IsStarted())
                {
                    promises.remove(persistent);
                    return;
                }
                persistent.Get(resource->GetIsolate())->Resolve(resource->GetContext(), V8Helpers::JSValue(base64Str));
                promises.remove(persistent);
            });
      });
    V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
    V8_CHECK(state != alt::PermissionState::Unspecified, "Permissions not specified");

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

static void IsGameFocused(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsGameFocused());
}

static void LoadModel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, hash);

    alt::ICore::Instance().LoadModel(hash);
}

static void LoadModelAsync(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, hash);

    alt::ICore::Instance().LoadModelAsync(hash);
}

static void GetHeadshotBase64(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, id);

    V8_RETURN_STRING(alt::ICore::Instance().HeadshotToBase64(id));
}

static void SetPedDlcClothes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(5, 6);

    V8_ARG_TO_INT(1, scriptId);
    V8_ARG_TO_UINT(2, dlc);
    V8_ARG_TO_INT(3, component);
    V8_ARG_TO_INT(4, drawable);
    V8_ARG_TO_INT(5, texture);

    V8_CHECK(drawable < 128, "Drawable can't be higher than 127");

    uint8_t palette;
    if(info.Length() == 5)
    {
        palette = 2;
    }
    else if(info.Length() == 6)
    {
        V8_ARG_TO_INT(6, paletteArg);
        palette = paletteArg;
    }

    alt::ICore::Instance().SetDlcClothes(scriptId, component, drawable, texture, palette, dlc);
}

static void SetPedDlcProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(5);

    V8_ARG_TO_INT(1, scriptId);
    V8_ARG_TO_UINT(2, dlc);
    V8_ARG_TO_INT(3, component);
    V8_ARG_TO_INT(4, drawable);
    V8_ARG_TO_INT(5, texture);

    alt::ICore::Instance().SetDlcProps(scriptId, component, drawable, texture, dlc);
}

static void ClearPedProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_INT(1, scriptId);
    V8_ARG_TO_INT(2, component);

    alt::ICore::Instance().ClearProps(scriptId, component);
}

static void SetWatermarkPosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, pos);

    alt::ICore::Instance().SetWatermarkPosition(pos);
}

static void GetFps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_RETURN(alt::ICore::Instance().GetFps());
}

static void GetPing(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_RETURN(alt::ICore::Instance().GetPing());
}

static void GetTotalPacketsSent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT64(alt::ICore::Instance().GetTotalPacketsSent());
}

static void GetTotalPacketsLost(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT64(alt::ICore::Instance().GetTotalPacketsLost());
}

static void GetServerIp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_STRING(alt::ICore::Instance().GetServerIp());
}

static void GetServerPort(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN(alt::ICore::Instance().GetServerPort());
}

static void HasLocalMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, key);

    V8_RETURN(alt::ICore::Instance().HasLocalMetaData(key));
}

static void GetLocalMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, key);

    V8_RETURN_MVALUE(alt::ICore::Instance().GetLocalMetaData(key));
}

static void CopyToClipboard(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    std::string text;
    if(info[0]->IsNull())
    {
        text = "";
    }
    else if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, textStr);
        text = textStr;
    }

    alt::PermissionState state = alt::ICore::Instance().CopyToClipboard(text);
    V8_CHECK(state != alt::PermissionState::Denied, "No permissions");
    V8_CHECK(state != alt::PermissionState::Unspecified, "Permission not specified");
    V8_CHECK(state != alt::PermissionState::Failed, "Failed to copy to clipboard");
}

static void ToggleRmlControls(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BOOLEAN(1, state);
    alt::ICore::Instance().ToggleRmlControls(state);
}

static void AreRmlControlsEnabled(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_BOOLEAN(alt::ICore::Instance().AreRmlControlsEnabled());
}

static void LoadRmlFont(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN_MIN_MAX(2, 4);

    V8_ARG_TO_STRING(1, path);
    V8_ARG_TO_STRING(2, name);
    V8_ARG_TO_BOOLEAN_OPT(3, italic, false);
    V8_ARG_TO_BOOLEAN_OPT(4, bold, false);

    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    alt::ICore::Instance().LoadRmlFontFace(resource->GetResource(), path, origin, name, italic, bold);
}

static void WorldToScreen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 3);

    alt::Vector3f vec;
    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x);
        V8_ARG_TO_NUMBER(2, y);
        V8_ARG_TO_NUMBER(3, z);
        vec = { x, y, z };
    }
    else
    {
        V8_ARG_TO_VECTOR3(1, val);
        vec = val;
    }

    V8_RETURN_VECTOR3(alt::ICore::Instance().WorldToScreen(vec));
}

static void ScreenToWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    alt::Vector2f vec;
    if(info.Length() == 2)
    {
        V8_ARG_TO_NUMBER(1, x);
        V8_ARG_TO_NUMBER(2, y);
        vec = { x, y };
    }
    else
    {
        V8_ARG_TO_VECTOR2(1, val);
        vec = val;
    }

    V8_RETURN_VECTOR3(alt::ICore::Instance().ScreenToWorld(vec));
}

static void GetCamPos(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN_VECTOR3(alt::ICore::Instance().GetCamPos());
}

static void GetScreenResolution(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN_VECTOR2(alt::ICore::Instance().GetScreenResolution());
}

static void SetMinimapComponentPosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(5, 7);

    V8_ARG_TO_STRING(1, name);
    V8_ARG_TO_STRING(2, alignX);
    V8_ARG_TO_STRING(3, alignY);
    alt::Vector2f pos;
    alt::Vector2f size;
    if(info.Length() == 5)
    {
        V8_ARG_TO_VECTOR2(4, posVec);
        V8_ARG_TO_VECTOR2(5, sizeVec);
        pos = posVec;
        size = sizeVec;
    }
    else
    {
        V8_ARG_TO_NUMBER(4, posX);
        V8_ARG_TO_NUMBER(5, posY);
        V8_ARG_TO_NUMBER(6, sizeX);
        V8_ARG_TO_NUMBER(7, sizeY);
        pos = { posX, posY };
        size = { sizeX, sizeY };
    }

    alt::ICore::Instance().SetMinimapComponentPosition(name, alignX[0], alignY[0], pos, size);
}

static void SetMinimapIsRectangle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BOOLEAN(1, isRectangle);
    alt::ICore::Instance().SetMinimapIsRectangle(isRectangle);
}

static void LoadDefaultIpls(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    alt::ICore::Instance().LoadDefaultIpls();
}

static void IsPointOnScreen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 3);

    alt::Vector3f vec;
    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x);
        V8_ARG_TO_NUMBER(2, y);
        V8_ARG_TO_NUMBER(3, z);
        vec = { x, y, z };
    }
    else
    {
        V8_ARG_TO_VECTOR3(1, val);
        vec = val;
    }

    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsPointOnScreen(vec));
}

extern V8Module sharedModule;
extern V8Class v8Player, v8Player, v8Vehicle, v8WebView, v8HandlingData, v8LocalStorage, v8MemoryBuffer, v8MapZoomData, v8Discord, v8Voice, v8WebSocketClient, v8Checkpoint, v8HttpClient,
  v8Audio, v8LocalPlayer, v8Profiler, v8Worker, v8RmlDocument, v8RmlElement, v8WeaponData, v8FocusData, v8Object;
extern V8Module altModule("alt",
                          &sharedModule,
                          { v8Player,     v8Vehicle, v8WebView,     v8HandlingData, v8LocalStorage, v8MemoryBuffer, v8MapZoomData, v8Discord,    v8Voice,     v8WebSocketClient, v8Checkpoint,
                            v8HttpClient, v8Audio,   v8LocalPlayer, v8Profiler,     v8Worker,       v8RmlDocument,  v8RmlElement,  v8WeaponData, v8FocusData, v8Object },
                          [](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)
                          {
                              v8::Isolate* isolate = ctx->GetIsolate();

                              V8Helpers::RegisterFunc(exports, "onServer", &OnServer);
                              V8Helpers::RegisterFunc(exports, "onceServer", &OnceServer);
                              V8Helpers::RegisterFunc(exports, "offServer", &OffServer);
                              V8Helpers::RegisterFunc(exports, "emitServer", &EmitServer);
                              V8Helpers::RegisterFunc(exports, "emitServerRaw", &EmitServerRaw);
                              V8Helpers::RegisterFunc(exports, "gameControlsEnabled", &GameControlsEnabled);
                              V8Helpers::RegisterFunc(exports, "toggleGameControls", &ToggleGameControls);
                              V8Helpers::RegisterFunc(exports, "toggleVoiceControls", &ToggleVoiceControls);
                              V8Helpers::RegisterFunc(exports, "showCursor", &ShowCursor);
                              V8Helpers::RegisterFunc(exports, "isCursorVisible", &IsCursorVisible);

                              V8Helpers::RegisterFunc(exports, "getCursorPos", &GetCursorPos);
                              V8Helpers::RegisterFunc(exports, "setCursorPos", &SetCursorPos);
                              V8Helpers::RegisterFunc(exports, "isMenuOpen", &IsMenuOpen);
                              V8Helpers::RegisterFunc(exports, "isConsoleOpen", &IsConsoleOpen);
                              // V8Helpers::RegisterFunc(exports, "drawRect2D", &DrawRect2D);

                              V8Helpers::RegisterFunc(exports, "requestIpl", &RequestIPL);
                              V8Helpers::RegisterFunc(exports, "removeIpl", &RemoveIPL);
                              // V8Helpers::RegisterFunc(exports, "wait", &ScriptWait);
                              V8Helpers::RegisterFunc(exports, "setCamFrozen", &SetCamFrozen);
                              V8Helpers::RegisterFunc(exports, "isCamFrozen", &IsCamFrozen);

                              V8Helpers::RegisterFunc(exports, "getLicenseHash", &GetLicenseHash);

                              // Gxt texts functions
                              V8Helpers::RegisterFunc(exports, "addGxtText", &AddGxtText);
                              V8Helpers::RegisterFunc(exports, "removeGxtText", &RemoveGxtText);
                              V8Helpers::RegisterFunc(exports, "getGxtText", &GetGxtText);

                              // Time managements functions
                              V8Helpers::RegisterFunc(exports, "setMsPerGameMinute", &SetMsPerGameMinute);
                              V8Helpers::RegisterFunc(exports, "getMsPerGameMinute", &GetMsPerGameMinute);

                              // CEF rendering on texture
                              V8Helpers::RegisterFunc(exports, "isTextureExistInArchetype", &IsTextureExistInArchetype);

                              // Scaleform additionals
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

                              V8Helpers::RegisterFunc(exports, "setRotationVelocity", &SetAngularVelocity);
                              // V8Helpers::RegisterFunc(exports, "setAngularVelocity", &SetAngularVelocity);

                              V8Helpers::RegisterFunc(exports, "isInStreamerMode", &IsInStreamerMode);
                              V8Helpers::RegisterFunc(exports, "getPermissionState", &GetPermissionState);

                              V8Helpers::RegisterFunc(exports, "takeScreenshot", &TakeScreenshot);
                              V8Helpers::RegisterFunc(exports, "takeScreenshotGameOnly", &TakeScreenshotGameOnly);

                              V8Helpers::RegisterFunc(exports, "isGameFocused", &IsGameFocused);

                              V8Helpers::RegisterFunc(exports, "loadModel", &LoadModel);
                              V8Helpers::RegisterFunc(exports, "loadModelAsync", &LoadModelAsync);

                              V8Helpers::RegisterFunc(exports, "loadYtyp", &LoadYtyp);
                              V8Helpers::RegisterFunc(exports, "unloadYtyp", &UnloadYtyp);

                              V8Helpers::RegisterFunc(exports, "getHeadshotBase64", &GetHeadshotBase64);

                              V8Helpers::RegisterFunc(exports, "setPedDlcClothes", &SetPedDlcClothes);
                              V8Helpers::RegisterFunc(exports, "setPedDlcProp", &SetPedDlcProps);
                              V8Helpers::RegisterFunc(exports, "clearPedProp", &ClearPedProps);

                              V8Helpers::RegisterFunc(exports, "setWatermarkPosition", &SetWatermarkPosition);

                              V8Helpers::RegisterFunc(exports, "getFps", &GetFps);
                              V8Helpers::RegisterFunc(exports, "getPing", &GetPing);

                              V8Helpers::RegisterFunc(exports, "getServerIp", &GetServerIp);
                              V8Helpers::RegisterFunc(exports, "getServerPort", &GetServerPort);

                              V8Helpers::RegisterFunc(exports, "getTotalPacketsSent", &GetTotalPacketsSent);
                              V8Helpers::RegisterFunc(exports, "getTotalPacketsLost", &GetTotalPacketsLost);

                              V8Helpers::RegisterFunc(exports, "hasLocalMeta", &HasLocalMeta);
                              V8Helpers::RegisterFunc(exports, "getLocalMeta", &GetLocalMeta);

                              V8Helpers::RegisterFunc(exports, "copyToClipboard", &CopyToClipboard);

                              V8Helpers::RegisterFunc(exports, "toggleRmlControls", &ToggleRmlControls);
                              V8Helpers::RegisterFunc(exports, "rmlControlsEnabled", &AreRmlControlsEnabled);
                              V8Helpers::RegisterFunc(exports, "loadRmlFont", &LoadRmlFont);

                              V8Helpers::RegisterFunc(exports, "worldToScreen", &WorldToScreen);
                              V8Helpers::RegisterFunc(exports, "screenToWorld", &ScreenToWorld);
                              V8Helpers::RegisterFunc(exports, "getCamPos", &GetCamPos);
                              V8Helpers::RegisterFunc(exports, "getScreenResolution", &GetScreenResolution);

                              V8Helpers::RegisterFunc(exports, "setMinimapComponentPosition", &SetMinimapComponentPosition);
                              V8Helpers::RegisterFunc(exports, "setMinimapIsRectangle", &SetMinimapIsRectangle);

                              V8Helpers::RegisterFunc(exports, "loadDefaultIpls", &LoadDefaultIpls);

                              V8Helpers::RegisterFunc(exports, "isPointOnScreen", &IsPointOnScreen);

                              V8_OBJECT_SET_BOOLEAN(exports, "isWorker", false);
                          });
