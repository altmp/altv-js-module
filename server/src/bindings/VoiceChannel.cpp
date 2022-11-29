#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"

using namespace alt;

static void AddPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");

    channel->AddPlayer(player);
}

static void RemovePlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");

    channel->RemovePlayer(player);
}

static void IsPlayerInChannel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");

    V8_RETURN_BOOLEAN(channel->HasPlayer(player));
}

static void MutePlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");

    channel->MutePlayer(player);
}

static void UnmutePlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");

    channel->UnmutePlayer(player);
}

static void IsPlayerMuted(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");

    V8_RETURN_BOOLEAN(channel->IsPlayerMuted(player));
}

static void GetPlayers(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    auto players = channel->GetPlayers();
    size_t size = players.size();
    v8::Local<v8::Array> playersArr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; ++i)
    {
        playersArr->Set(ctx, i, resource->GetEntity(players[i], "Player")->GetJSVal(isolate));
    }
    V8_RETURN(playersArr);
}

static void GetPlayerCount(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(channel, IVoiceChannel);

    V8_RETURN((uint32_t)channel->GetPlayerCount());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_BOOLEAN(1, isSpatial);
    V8_ARG_TO_NUMBER(2, maxDistance);
    V8_BIND_BASE_OBJECT(ICore::Instance().CreateVoiceChannel(isSpatial, maxDistance), "Failed to create VoiceChannel, make sure voice chat is enabled");
}

extern V8Class v8BaseObject;
extern V8Class v8VoiceChannel("VoiceChannel",
                              v8BaseObject,
                              Constructor,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  V8Helpers::SetMethod(isolate, tpl, "addPlayer", &AddPlayer);
                                  V8Helpers::SetMethod(isolate, tpl, "removePlayer", &RemovePlayer);
                                  V8Helpers::SetMethod(isolate, tpl, "isPlayerInChannel", &IsPlayerInChannel);
                                  V8Helpers::SetMethod(isolate, tpl, "mutePlayer", &MutePlayer);
                                  V8Helpers::SetMethod(isolate, tpl, "unmutePlayer", &UnmutePlayer);
                                  V8Helpers::SetMethod(isolate, tpl, "isPlayerMuted", &IsPlayerMuted);

                                  V8Helpers::SetAccessor(isolate, tpl, "players", &GetPlayers);
                                  V8Helpers::SetAccessor(isolate, tpl, "playerCount", &GetPlayerCount);
                              });
