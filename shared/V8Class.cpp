
#include "V8Helpers.h"
#include "V8Class.h"

v8::Local<v8::Value> V8Class::New(v8::Local<v8::Context> ctx, std::vector<v8::Local<v8::Value>>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::FunctionTemplate> _tpl = tplMap.at(isolate).Get(isolate);
    v8::Local<v8::Value> obj;

    V8Helpers::TryCatch([&] {
        v8::Local<v8::Function> func;
        if(!_tpl->GetFunction(ctx).ToLocal(&func))
        {
            Log::Error << "V8Class::New Invalid constructor called" << Log::Endl;
            return false;
        }

        v8::MaybeLocal<v8::Value> maybeObj = func->CallAsConstructor(ctx, args.size(), args.data());

        if(!maybeObj.IsEmpty())
        {
            obj = maybeObj.ToLocalChecked();
            return true;
        }
        else
        {
            obj = v8::Null(isolate);
            return false;
        }
    });

    return obj;
}

v8::Local<v8::Value> V8Class::CreateInstance(v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::vector<v8::Local<v8::Value>> args)
{
    v8::Local<v8::Function> constructor = JSValue(isolate, ctx);
    v8::Local<v8::Value> obj;

    V8Helpers::TryCatch([&] {
        v8::MaybeLocal<v8::Value> maybeObj = constructor->CallAsConstructor(ctx, args.size(), args.data());

        if(maybeObj.IsEmpty()) return false;

        obj = maybeObj.ToLocalChecked();
        return true;
    });

    return obj;
}
