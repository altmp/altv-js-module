#include "Convert.h"
#include "V8Helpers.h"

bool V8::SafeToBoolean(v8::Local<v8::Value> val, v8::Isolate* isolate, bool& out)
{
    out = val->ToBoolean(isolate)->Value();
    return true;
}

bool V8::SafeToInteger(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out)
{
    v8::MaybeLocal maybeVal = val->ToInteger(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8::SafeToUInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint64_t& out)
{
    auto check = val->ToInteger(ctx);
    if(check.IsEmpty() || check.ToLocalChecked()->Value() < 0) return false;  // Check for negative values
    v8::MaybeLocal maybeVal = val->ToBigInt(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Uint64Value();
    return true;
}

bool V8::SafeToInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out)
{
    v8::MaybeLocal maybeVal = val->ToBigInt(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Int64Value();
    return true;
}

bool V8::SafeToUInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint32_t& out)
{
    auto check = val->ToInteger(ctx);
    if(check.IsEmpty() || check.ToLocalChecked()->Value() < 0) return false;  // Check for negative values
    v8::MaybeLocal maybeVal = val->ToUint32(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8::SafeToInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int32_t& out)
{
    v8::MaybeLocal maybeVal = val->ToInt32(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8::SafeToNumber(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, double& out)
{
    v8::MaybeLocal maybeVal = val->ToNumber(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8::SafeToString(v8::Local<v8::Value> val, v8::Isolate* isolate, v8::Local<v8::Context> ctx, alt::String& out)
{
    v8::MaybeLocal maybeVal = val->ToString(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = *v8::String::Utf8Value(isolate, maybeVal.ToLocalChecked());
    return true;
}

bool V8::SafeToFunction(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Function>& out)
{
    if(val->IsFunction())
    {
        out = val.As<v8::Function>();
        return true;
    }

    return false;
}

bool V8::SafeToObject(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Object>& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked();
    return true;
}

bool V8::SafeToRGBA(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::RGBA& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        uint32_t r, g, b, a;
        if(SafeToUInt32(V8::Get(ctx, val, "r"), ctx, r) && SafeToUInt32(V8::Get(ctx, val, "g"), ctx, g) && SafeToUInt32(V8::Get(ctx, val, "b"), ctx, b) &&
           SafeToUInt32(V8::Get(ctx, val, "a"), ctx, a))
        {
            out = alt::RGBA{ uint8_t(r), uint8_t(g), uint8_t(b), uint8_t(a) };
            return true;
        }
    }

    return false;
}

bool V8::SafeToVector3(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector3f& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        double x, y, z;
        if(SafeToNumber(V8::Get(ctx, val, "x"), ctx, x) && SafeToNumber(V8::Get(ctx, val, "y"), ctx, y) && SafeToNumber(V8::Get(ctx, val, "z"), ctx, z))
        {
            out = alt::Vector3f{ float(x), float(y), float(z) };
            return true;
        }
    }

    return false;
}

bool V8::SafeToVector2(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector2f& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        double x, y;
        if(SafeToNumber(V8::Get(ctx, val, "x"), ctx, x) && SafeToNumber(V8::Get(ctx, val, "y"), ctx, y))
        {
            out = alt::Vector2f{ float(x), float(y) };
            return true;
        }
    }

    return false;
}

bool V8::SafeToArrayBuffer(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBuffer>& out)
{
    if(val->IsArrayBuffer())
    {
        out = val.As<v8::ArrayBuffer>();
        return true;
    }

    return false;
}

bool V8::SafeToArrayBufferView(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBufferView>& out)
{
    if(val->IsArrayBufferView())
    {
        out = val.As<v8::ArrayBufferView>();
        return true;
    }

    return false;
}

bool V8::SafeToArray(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Array>& out)
{
    if(val->IsArray())
    {
        out = val.As<v8::Array>();
        return true;
    }

    return false;
}
