#pragma once
#include "v8.h"

void Emit(const v8::FunctionCallbackInfo<v8::Value>& info);
void On(const v8::FunctionCallbackInfo<v8::Value>& info);
void Once(const v8::FunctionCallbackInfo<v8::Value>& info);
