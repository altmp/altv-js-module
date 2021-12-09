
#include "cpp-sdk/ICore.h"
#include "V8ResourceImpl.h"
#include "V8Helpers.h"
#include <climits>

bool V8Helpers::TryCatch(const std::function<bool()>& fn)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();
    v8::TryCatch tryCatch(isolate);

    V8ResourceImpl* v8resource = V8ResourceImpl::Get(context);
    alt::IResource* resource = v8resource->GetResource();

    if(!fn())
    {
        v8::Local<v8::Value> exception = tryCatch.Exception();
        v8::Local<v8::Message> message = tryCatch.Message();

        if(!message.IsEmpty() && !context.IsEmpty())
        {
            v8::MaybeLocal<v8::String> maybeSourceLine = message->GetSourceLine(context);
            v8::Maybe<int32_t> line = message->GetLineNumber(context);
            v8::ScriptOrigin origin = message->GetScriptOrigin();

            if(!origin.ResourceName()->IsUndefined())
            {
                Log::Error << "[V8] Exception at " << resource->GetName() << ":" << *v8::String::Utf8Value(isolate, origin.ResourceName()) << ":" << line.ToChecked() << Log::Endl;

                if(!maybeSourceLine.IsEmpty())
                {
                    v8::Local<v8::String> sourceLine = maybeSourceLine.ToLocalChecked();

                    if(sourceLine->Length() <= 80)
                    {
                        Log::Error << "  " << *v8::String::Utf8Value(isolate, sourceLine) << Log::Endl;
                    }
                    else
                    {
                        Log::Error << "  " << std::string{ *v8::String::Utf8Value(isolate, sourceLine), 80 } << "..." << Log::Endl;
                    }
                }

                v8resource->DispatchErrorEvent(
                  *v8::String::Utf8Value(isolate, message->Get()), *v8::String::Utf8Value(isolate, origin.ResourceName()), line.IsNothing() ? -1 : line.ToChecked());
            }
            else
            {
                Log::Error << "[V8] Exception at " << resource->GetName() << Log::Endl;
            }

            v8::MaybeLocal<v8::Value> stackTrace = tryCatch.StackTrace(context);
            if(!stackTrace.IsEmpty() && stackTrace.ToLocalChecked()->IsString())
            {
                v8::String::Utf8Value stackTraceStr(isolate, stackTrace.ToLocalChecked().As<v8::String>());
                Log::Error << "  " << *stackTraceStr << Log::Endl;
            }
        }
        else if(!exception.IsEmpty())
        {
            Log::Error << "[V8] Exception: " << *v8::String::Utf8Value(isolate, exception) << Log::Endl;
        }
        else
        {
            Log::Error << "[V8] Exception occured" << Log::Endl;
        }

        return false;
    }

    return true;
}

void V8Helpers::RegisterFunc(v8::Local<v8::Object> exports, const std::string& _name, v8::FunctionCallback cb, void* data)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    v8::Local<v8::String> name = V8::JSValue(_name);

    v8::Local<v8::Function> fn = v8::Function::New(ctx, cb, v8::External::New(isolate, data)).ToLocalChecked();
    fn->SetName(name);

    exports->Set(ctx, name, fn);
}

void V8Helpers::FunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto fn = static_cast<alt::MValueFunction*>(info.Data().As<v8::External>()->Value());

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);

    Log::Debug << "FunctionCallback " << resource->GetResource()->GetName() << " " << V8ResourceImpl::Get(isolate->GetEnteredOrMicrotaskContext())->GetResource()->GetName() << Log::Endl;

    alt::MValueArgs args;

    for(uint64_t i = 0; i < info.Length(); ++i) args.Push(V8Helpers::V8ToMValue(info[i]));

    alt::MValue res = (*fn)->Call(args);

    info.GetReturnValue().Set(V8Helpers::MValueToV8(res));
}

void V8Helpers::SetAccessor(v8::Local<v8::Template> tpl, v8::Isolate* isolate, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter)
{
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), getter, setter);
}

void V8::DefineOwnProperty(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> val, const char* name, v8::Local<v8::Value> value, v8::PropertyAttribute attributes)
{
    val->DefineOwnProperty(ctx, v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), value, attributes);
}

void V8::DefineOwnProperty(
  v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> val, v8::Local<v8::String> name, v8::Local<v8::Value> value, v8::PropertyAttribute attributes)
{
    val->DefineOwnProperty(ctx, name, value, attributes);
}

void V8::SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter)
{
    tpl->PrototypeTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(),
                                          getter,
                                          setter,
                                          v8::Local<v8::Value>(),
                                          v8::AccessControl::DEFAULT,
                                          setter != nullptr ? v8::PropertyAttribute::None : v8::PropertyAttribute::ReadOnly);
}

void V8::SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback)
{
    tpl->PrototypeTemplate()->Set(isolate, name, v8::FunctionTemplate::New(isolate, callback));
}

void V8::SetStaticAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter)
{
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(), getter, setter);
}

void V8::SetStaticMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback)
{
    tpl->Set(isolate, name, v8::FunctionTemplate::New(isolate, callback));
}

v8::Local<v8::Value> V8::Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, const char* name)
{
    return obj->Get(ctx, v8::String::NewFromUtf8(ctx->GetIsolate(), name, v8::NewStringType::kInternalized).ToLocalChecked()).ToLocalChecked();
}

v8::Local<v8::Value> V8::Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, v8::Local<v8::Name> name)
{
    return obj->Get(ctx, name).ToLocalChecked();
}

void V8::SetFunction(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> target, const char* name, v8::FunctionCallback cb, void* userData)
{
    v8::Local<v8::String> _name = v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked();

    v8::Local<v8::Function> fn = v8::Function::New(ctx, cb, v8::External::New(isolate, userData)).ToLocalChecked();
    fn->SetName(_name);

    target->Set(ctx, _name, fn);
}

v8::Local<v8::Value> V8::New(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Function> constructor, std::vector<v8::Local<v8::Value>>& args)
{
    v8::Local<v8::Value> obj;

    V8Helpers::TryCatch([&] {
        v8::MaybeLocal<v8::Value> maybeObj = constructor->CallAsConstructor(ctx, args.size(), args.data());

        if(maybeObj.IsEmpty()) return false;

        obj = maybeObj.ToLocalChecked();
        return true;
    });

    return obj;
}

inline static std::string GetStackFrameScriptName(v8::Local<v8::StackFrame> frame)
{
    v8::Local<v8::Value> name = frame->GetScriptName();
    if(!name.IsEmpty()) return *v8::String::Utf8Value(v8::Isolate::GetCurrent(), name);
    else if(frame->IsEval())
        return "[eval]";
    else if(frame->IsWasm())
        return "[wasm]";
    else if(!frame->IsUserJavaScript())
        return "[internal]";
    else
        return "[unknown]";
}

V8::SourceLocation V8::SourceLocation::GetCurrent(v8::Isolate* isolate)
{
    v8::Local<v8::StackTrace> stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 5);
    auto ctx = isolate->GetEnteredOrMicrotaskContext();

    for(int i = 0; i < stackTrace->GetFrameCount(); i++)
    {
        v8::Local<v8::StackFrame> frame = stackTrace->GetFrame(isolate, i);
        if(frame->GetScriptName().IsEmpty() && !frame->IsEval() && !frame->IsWasm() && frame->IsUserJavaScript()) continue;

        std::string name = GetStackFrameScriptName(frame);
        int line = frame->GetLineNumber();
        return SourceLocation{ std::move(name), line, ctx };
    }

    return SourceLocation{ "[unknown]", 0, ctx };
}

V8::SourceLocation::SourceLocation(std::string&& _fileName, int _line, v8::Local<v8::Context> ctx) : fileName(_fileName), line(_line)
{
    context.Reset(ctx->GetIsolate(), ctx);
}

std::string V8::SourceLocation::ToString()
{
    auto isolate = v8::Isolate::GetCurrent();

    std::stringstream stream;
    stream << "[";
    // Check if not inside a worker
    if(!(*static_cast<bool*>(isolate->GetData(v8::Isolate::GetNumberOfDataSlots() - 1))))
    {
        stream << V8ResourceImpl::Get(context.Get(v8::Isolate::GetCurrent()))->GetResource()->GetName().CStr() << ":";
    }
    stream << fileName << ":" << line << "]";
    return stream.str();
}

V8::StackTrace V8::StackTrace::GetCurrent(v8::Isolate* isolate)
{
    v8::Local<v8::StackTrace> stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 5);
    auto ctx = isolate->GetEnteredOrMicrotaskContext();
    std::vector<Frame> frames;
    for(int i = 0; i < stackTrace->GetFrameCount(); i++)
    {
        v8::Local<v8::StackFrame> frame = stackTrace->GetFrame(isolate, i);
        Frame frameData;
        frameData.file = GetStackFrameScriptName(frame);
        frameData.line = frame->GetLineNumber();
        if(frame->GetFunctionName().IsEmpty()) frameData.function = "[anonymous]";
        else
            frameData.function = *v8::String::Utf8Value(isolate, frame->GetFunctionName());

        frames.push_back(std::move(frameData));
    }

    return StackTrace{ std::move(frames), ctx };
}

V8::StackTrace::StackTrace(std::vector<Frame>&& frames, v8::Local<v8::Context> ctx) : frames(frames), context(ctx->GetIsolate(), ctx) {}

void V8::StackTrace::Print(uint32_t offset)
{
    auto& frames = GetFrames();
    size_t size = frames.size();

    for(size_t i = offset; i < size; i++)
    {
        const Frame& frame = frames[i];
        Log::Error << "  at " << frame.function << " (" << frame.file << ":" << frame.line << ")" << Log::Endl;
    }
}

void V8::StackTrace::Print(v8::Isolate* isolate)
{
    V8::StackTrace trace = V8::StackTrace::GetCurrent(isolate);
    trace.Print();
}

v8::Local<v8::String> V8::Vector3_XKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> xKey{ isolate, v8::String::NewFromUtf8(isolate, "x", v8::NewStringType::kInternalized).ToLocalChecked() };

    return xKey.Get(isolate);
}

v8::Local<v8::String> V8::Vector3_YKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> yKey{ isolate, v8::String::NewFromUtf8(isolate, "y", v8::NewStringType::kInternalized).ToLocalChecked() };

    return yKey.Get(isolate);
}

v8::Local<v8::String> V8::Vector3_ZKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> zKey{ isolate, v8::String::NewFromUtf8(isolate, "z", v8::NewStringType::kInternalized).ToLocalChecked() };

    return zKey.Get(isolate);
}

v8::Local<v8::String> V8::RGBA_RKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> rKey{ isolate, v8::String::NewFromUtf8(isolate, "r", v8::NewStringType::kInternalized).ToLocalChecked() };

    return rKey.Get(isolate);
}

v8::Local<v8::String> V8::RGBA_GKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> gKey{ isolate, v8::String::NewFromUtf8(isolate, "g", v8::NewStringType::kInternalized).ToLocalChecked() };

    return gKey.Get(isolate);
}

v8::Local<v8::String> V8::RGBA_BKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> bKey{ isolate, v8::String::NewFromUtf8(isolate, "b", v8::NewStringType::kInternalized).ToLocalChecked() };

    return bKey.Get(isolate);
}

v8::Local<v8::String> V8::RGBA_AKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> aKey{ isolate, v8::String::NewFromUtf8(isolate, "a", v8::NewStringType::kInternalized).ToLocalChecked() };

    return aKey.Get(isolate);
}

v8::Local<v8::String> V8::Fire_PosKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> aKey{ isolate, v8::String::NewFromUtf8(isolate, "pos", v8::NewStringType::kInternalized).ToLocalChecked() };

    return aKey.Get(isolate);
}

v8::Local<v8::String> V8::Fire_WeaponKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> aKey{ isolate, v8::String::NewFromUtf8(isolate, "weapon", v8::NewStringType::kInternalized).ToLocalChecked() };

    return aKey.Get(isolate);
}

std::vector<V8::EventCallback*> V8::EventHandler::GetCallbacks(V8ResourceImpl* impl, const alt::CEvent* e)
{
    return callbacksGetter(impl, e);
}

std::vector<v8::Local<v8::Value>> V8::EventHandler::GetArgs(V8ResourceImpl* impl, const alt::CEvent* e)
{
    std::vector<v8::Local<v8::Value>> args;
    argsGetter(impl, e, args);
    return args;
}

V8::EventHandler* V8::EventHandler::Get(const alt::CEvent* e)
{
    auto& _all = all();
    auto it = _all.find(e->GetType());

    return (it != _all.end()) ? it->second : nullptr;
}

void V8::EventHandler::Register(alt::CEvent::Type type, EventHandler* handler)
{
    auto& _all = all();
    if(_all.count(type) == 0)
    {
        _all.insert({ type, handler });
    }
    else
    {
        Log::Error << "Handler for " << (int)type << " already defined" << Log::Endl;
    }
}

V8::EventHandler::CallbacksGetter V8::LocalEventHandler::GetCallbacksGetter(const std::string& name)
{
    return [name](V8ResourceImpl* resource, const alt::CEvent*) -> std::vector<EventCallback*> { return resource->GetLocalHandlers(name); };
}

V8::EventHandler::EventHandler(alt::CEvent::Type type, CallbacksGetter&& _handlersGetter, ArgsGetter&& _argsGetter)
    : callbacksGetter(std::move(_handlersGetter)), argsGetter(std::move(_argsGetter)), type(type)
{
    Register(type, this);
}

// Temp issue fix for https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
void V8::EventHandler::Reference()
{
    Log::Info << "[V8] Registered handler for " << std::to_string((int)type) << Log::Endl;
}

std::string V8::Stringify(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val)
{
    v8::Local<v8::String> str;
    if(!val->ToString(ctx).ToLocal(&str)) return std::string();
    if(val->IsObject() && strcmp(*v8::String::Utf8Value(ctx->GetIsolate(), str), "[object Object]") == 0)
    {
        v8::MaybeLocal<v8::String> maybe = v8::JSON::Stringify(ctx, val);
        v8::Local<v8::String> stringified;
        if(maybe.ToLocal(&stringified)) str = stringified;
    }

    if(str.IsEmpty()) return std::string();
    std::string result = *v8::String::Utf8Value(ctx->GetIsolate(), str);
    return result;
}

alt::String V8::GetJSValueTypeName(v8::Local<v8::Value> val)
{
    if(val->IsUndefined()) return "undefined";
    if(val->IsNull()) return "null";
    if(val->IsNumber()) return "number";
    if(val->IsString()) return "string";
    if(val->IsArray()) return "array";
    if(val->IsBoolean()) return "bool";
    if(val->IsBigInt()) return "bigint";
    if(val->IsArrayBuffer()) return "arraybuffer";
    if(val->IsArrayBufferView()) return "arraybufferview";
    if(val->IsDate()) return "date";
    if(val->IsArgumentsObject()) return "arguments";
    if(val->IsAsyncFunction()) return "asyncfunction";
    if(val->IsExternal()) return "external";
    if(val->IsDataView()) return "dataview";
    if(val->IsSymbol()) return "symbol";
    if(val->IsFunction()) return "function";
    if(val->IsRegExp()) return "regexp";
    if(val->IsGeneratorFunction()) return "generatorfunction";
    if(val->IsPromise()) return "promise";
    if(val->IsProxy()) return "proxy";
    if(val->IsMap()) return "map";
    if(val->IsSet()) return "set";
    if(val->IsWeakMap()) return "weakmap";
    if(val->IsWeakSet()) return "weakset";
    if(val->IsTypedArray()) return "typedarray";
    if(val->IsProxy()) return "proxy";
    if(val->IsObject()) return "object";
    else
        return "unknown";
}
