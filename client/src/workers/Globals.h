#pragma once
#include "v8.h"

void Emit(const v8::FunctionCallbackInfo<v8::Value>& info);
void On(const v8::FunctionCallbackInfo<v8::Value>& info);
void Once(const v8::FunctionCallbackInfo<v8::Value>& info);
void Log(const v8::FunctionCallbackInfo<v8::Value>& info);
void LogWarning(const v8::FunctionCallbackInfo<v8::Value>& info);
void LogError(const v8::FunctionCallbackInfo<v8::Value>& info);

void NextTick(const v8::FunctionCallbackInfo<v8::Value>& info);
void SetTimeout(const v8::FunctionCallbackInfo<v8::Value>& info);
void SetInterval(const v8::FunctionCallbackInfo<v8::Value>& info);
void ClearTimer(const v8::FunctionCallbackInfo<v8::Value>& info);

void GetSharedArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& info);
