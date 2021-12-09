#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"

namespace V8Helpers
{
    alt::MValueByteArray V8ToRawBytes(v8::Local<v8::Value> val);
    v8::MaybeLocal<v8::Value> RawBytesToV8(alt::MValueByteArrayConst bytes);
}  // namespace V8Helpers
