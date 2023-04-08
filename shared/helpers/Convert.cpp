#include "Convert.h"
#include "V8Helpers.h"

bool V8Helpers::SafeToBoolean(v8::Local<v8::Value> val, v8::Isolate* isolate, bool& out)
{
    out = val->ToBoolean(isolate)->Value();
    return true;
}

bool V8Helpers::SafeToInteger(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out)
{
    v8::MaybeLocal maybeVal = val->ToInteger(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8Helpers::SafeToUInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint64_t& out)
{
    auto check = val->ToInteger(ctx);
    if(check.IsEmpty() || check.ToLocalChecked()->Value() < 0) return false;  // Check for negative values
    v8::MaybeLocal maybeVal = val->ToBigInt(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Uint64Value();
    return true;
}

bool V8Helpers::SafeToInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out)
{
    v8::MaybeLocal maybeVal = val->ToBigInt(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Int64Value();
    return true;
}

bool V8Helpers::SafeToUInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint32_t& out)
{
    auto check = val->ToInteger(ctx);
    if(check.IsEmpty() || check.ToLocalChecked()->Value() < 0) return false;  // Check for negative values
    v8::MaybeLocal maybeVal = val->ToUint32(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8Helpers::SafeToInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int32_t& out)
{
    v8::MaybeLocal maybeVal = val->ToInt32(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8Helpers::SafeToNumber(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, double& out)
{
    v8::MaybeLocal maybeVal = val->ToNumber(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked()->Value();
    return true;
}

bool V8Helpers::SafeToString(v8::Local<v8::Value> val, v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::string& out)
{
    v8::MaybeLocal maybeVal = val->ToString(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = *v8::String::Utf8Value(isolate, maybeVal.ToLocalChecked());
    return true;
}

bool V8Helpers::SafeToFunction(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Function>& out)
{
    if(val->IsFunction())
    {
        out = val.As<v8::Function>();
        return true;
    }

    return false;
}

bool V8Helpers::SafeToObject(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Object>& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(maybeVal.IsEmpty()) return false;
    out = maybeVal.ToLocalChecked();
    return true;
}

bool V8Helpers::SafeToRGBA(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::RGBA& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        uint32_t r, g, b, a;
        if(SafeToUInt32(V8Helpers::Get(ctx, val, "r"), ctx, r) && SafeToUInt32(V8Helpers::Get(ctx, val, "g"), ctx, g) && SafeToUInt32(V8Helpers::Get(ctx, val, "b"), ctx, b) &&
           SafeToUInt32(V8Helpers::Get(ctx, val, "a"), ctx, a))
        {
            out = alt::RGBA{ uint8_t(r), uint8_t(g), uint8_t(b), uint8_t(a) };
            return true;
        }
    }

    return false;
}

bool V8Helpers::SafeToVector3(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector3f& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        double x, y, z;
        if(SafeToNumber(V8Helpers::Get(ctx, val, "x"), ctx, x) && SafeToNumber(V8Helpers::Get(ctx, val, "y"), ctx, y) && SafeToNumber(V8Helpers::Get(ctx, val, "z"), ctx, z))
        {
            out = alt::Vector3f{ float(x), float(y), float(z) };
            return true;
        }
    }

    return false;
}

bool V8Helpers::SafeToVector3Int(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector3i& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        int x, y, z;
        if(SafeToInt32(V8Helpers::Get(ctx, val, "x"), ctx, x) && SafeToInt32(V8Helpers::Get(ctx, val, "y"), ctx, y) && SafeToInt32(V8Helpers::Get(ctx, val, "z"), ctx, z))
        {
            out = alt::Vector3i{ x, y, z };
            return true;
        }
    }

    return false;
}

bool V8Helpers::SafeToVector2(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector2f& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        double x, y;
        if(SafeToNumber(V8Helpers::Get(ctx, val, "x"), ctx, x) && SafeToNumber(V8Helpers::Get(ctx, val, "y"), ctx, y))
        {
            out = alt::Vector2f{ float(x), float(y) };
            return true;
        }
    }

    return false;
}

bool V8Helpers::SafeToVector2Int(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector2i& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        int x, y;
        if(SafeToInt32(V8Helpers::Get(ctx, val, "x"), ctx, x) && SafeToInt32(V8Helpers::Get(ctx, val, "y"), ctx, y))
        {
            out = alt::Vector2i{ x, y };
            return true;
        }
    }

    return false;
}

bool V8Helpers::SafeToQuaternion(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Quaternion& out)
{
    v8::MaybeLocal maybeVal = val->ToObject(ctx);
    if(!maybeVal.IsEmpty())
    {
        v8::Local val = maybeVal.ToLocalChecked();

        double x, y, z, w;
        if(SafeToNumber(V8Helpers::Get(ctx, val, "x"), ctx, x) && SafeToNumber(V8Helpers::Get(ctx, val, "y"), ctx, y) && SafeToNumber(V8Helpers::Get(ctx, val, "z"), ctx, z) && SafeToNumber(V8Helpers::Get(ctx, val, "w"), ctx, w))
        {
            out = alt::Quaternion{ (float) x, (float) y, (float) z, (float) w };
            return true;
        }
    }

    return false;
}

bool V8Helpers::SafeToArrayBuffer(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBuffer>& out)
{
    if(val->IsArrayBuffer())
    {
        out = val.As<v8::ArrayBuffer>();
        return true;
    }

    return false;
}

bool V8Helpers::SafeToArrayBufferView(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBufferView>& out)
{
    if(val->IsArrayBufferView())
    {
        out = val.As<v8::ArrayBufferView>();
        return true;
    }

    return false;
}

bool V8Helpers::SafeToArray(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Array>& out)
{
    if(val->IsArray())
    {
        out = val.As<v8::Array>();
        return true;
    }

    return false;
}

v8::Local<v8::Value> V8Helpers::ConfigNodeToV8(Config::Value::ValuePtr node)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    v8::Local<v8::Value> out;
    switch(node->GetType())
    {
        case Config::Value::Type::STRING:
        {
            out = V8Helpers::JSValue(node->AsString());
            break;
        }
        case Config::Value::Type::NUMBER:
        {
            out = V8Helpers::JSValue(node->AsNumber());
            break;
        }
        case Config::Value::Type::BOOL:
        {
            out = V8Helpers::JSValue(node->AsBool());
            break;
        }
        case Config::Value::Type::LIST:
        {
            v8::Local<v8::Array> arr = v8::Array::New(isolate, node->GetSize());
            for(size_t i = 0; i < node->GetSize(); i++)
            {
                v8::Local<v8::Value> val = ConfigNodeToV8(node[i]);
                if(val.IsEmpty()) continue;
                arr->Set(ctx, i, val);
            }
            out = arr;
            break;
        }
        case Config::Value::Type::DICT:
        {
            v8::Local<v8::Object> obj = v8::Object::New(isolate);
            for(auto& pair : node->AsDict())
            {
                v8::Local<v8::Value> val = ConfigNodeToV8(pair.second);
                if(val.IsEmpty()) continue;
                obj->Set(ctx, V8Helpers::JSValue(pair.first), val);
            }
            out = obj;
            break;
        }
        default: return v8::Local<v8::Value>();
    }
    return out;
}
