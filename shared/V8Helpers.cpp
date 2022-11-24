
#include "cpp-sdk/ICore.h"
#include "V8ResourceImpl.h"
#include "V8Helpers.h"
#ifdef ALT_CLIENT
    #include "CV8Resource.h"
#endif

#include <climits>
#include <thread>
#include <chrono>
#include <sstream>

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
            std::string name = *v8::String::Utf8Value(isolate, origin.ResourceName());

            // Only relevant for client
            bool isBytecodeFile = false;
#ifdef ALT_CLIENT
            isBytecodeFile = static_cast<CV8ResourceImpl*>(v8resource)->GetModuleData(name).isBytecode;
#endif

            if(!origin.ResourceName()->IsUndefined())
            {
                if(line.IsNothing() || isBytecodeFile)
                {
                    Log::Error << "[V8] Exception at " << resource->GetName() << ":" << name << Log::Endl;
                }
                else
                    Log::Error << "[V8] Exception at " << resource->GetName() << ":" << name << ":" << line.ToChecked() << Log::Endl;
            }
            else
            {
                Log::Error << "[V8] Exception at " << resource->GetName() << Log::Endl;
            }

            if(!maybeSourceLine.IsEmpty() && !isBytecodeFile)
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

            std::string trace;
            v8::MaybeLocal<v8::Value> stackTrace = tryCatch.StackTrace(context);
            if(!stackTrace.IsEmpty() && stackTrace.ToLocalChecked()->IsString())
            {
                trace = *v8::String::Utf8Value(isolate, stackTrace.ToLocalChecked().As<v8::String>());
                Log::Error << "  " << trace << Log::Endl;
            }

            if(!exception.IsEmpty())
            {
                Log::Error << *v8::String::Utf8Value(isolate, exception) << Log::Endl;
            }

            std::string sourceFile = origin.ResourceName()->IsUndefined() ? "<unknown>" : *v8::String::Utf8Value(isolate, origin.ResourceName());
            v8resource->DispatchErrorEvent(*v8::String::Utf8Value(isolate, message->Get()), sourceFile, line.IsNothing() ? -1 : line.ToChecked(), trace);
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

v8::Local<v8::Value> V8Helpers::Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, const char* name)
{
    return obj->Get(ctx, v8::String::NewFromUtf8(ctx->GetIsolate(), name, v8::NewStringType::kInternalized).ToLocalChecked()).ToLocalChecked();
}

v8::Local<v8::Value> V8Helpers::Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, v8::Local<v8::Name> name)
{
    return obj->Get(ctx, name).ToLocalChecked();
}

v8::Local<v8::Value> V8Helpers::New(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Function> constructor, std::vector<v8::Local<v8::Value>>& args)
{
    v8::Local<v8::Value> obj;

    V8Helpers::TryCatch(
      [&]
      {
          v8::MaybeLocal<v8::Value> maybeObj = constructor->CallAsConstructor(ctx, args.size(), args.data());

          if(maybeObj.IsEmpty()) return false;

          obj = maybeObj.ToLocalChecked();
          return true;
      });

    return obj;
}

v8::Local<v8::Object> V8Helpers::CreateCustomObject(v8::Isolate* isolate,
                                                    void* data,
                                                    v8::GenericNamedPropertyGetterCallback getter,
                                                    v8::GenericNamedPropertySetterCallback setter,
                                                    v8::GenericNamedPropertyDeleterCallback deleter,
                                                    v8::GenericNamedPropertyEnumeratorCallback enumerator,
                                                    v8::GenericNamedPropertyQueryCallback query)
{
    v8::Local<v8::ObjectTemplate> objTemplate = v8::ObjectTemplate::New(isolate);
    v8::NamedPropertyHandlerConfiguration config;
    config.getter = getter;
    config.setter = setter;
    config.deleter = deleter;
    config.query = query;
    config.enumerator = enumerator;
    config.data = v8::External::New(isolate, data);
    config.flags = v8::PropertyHandlerFlags::kHasNoSideEffect;
    objTemplate->SetHandler(config);

    v8::Local<v8::Object> obj = objTemplate->NewInstance(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked();
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

V8Helpers::SourceLocation V8Helpers::SourceLocation::GetCurrent(v8::Isolate* isolate, V8ResourceImpl* resource)
{
    v8::Local<v8::StackTrace> stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 5);
    auto ctx = isolate->GetEnteredOrMicrotaskContext();

    for(int i = 0; i < stackTrace->GetFrameCount(); i++)
    {
        v8::Local<v8::StackFrame> frame = stackTrace->GetFrame(isolate, i);
        if(frame->GetScriptName().IsEmpty() && !frame->IsEval() && !frame->IsWasm() && frame->IsUserJavaScript()) continue;

        std::string name = GetStackFrameScriptName(frame);
        bool isBytecode = false;
#ifdef ALT_CLIENT_API
        isBytecode = resource ? static_cast<CV8ResourceImpl*>(resource)->GetModuleData(name).isBytecode : false;
#endif
        int line = isBytecode ? 0 : frame->GetLineNumber();
        return SourceLocation{ std::move(name), line, ctx };
    }

    return SourceLocation{ "[unknown]", 0, ctx };
}

V8Helpers::SourceLocation::SourceLocation(std::string&& _fileName, int _line, v8::Local<v8::Context> ctx) : fileName(_fileName), line(_line)
{
    context.Reset(ctx->GetIsolate(), ctx);
}

std::string V8Helpers::SourceLocation::ToString()
{
    auto isolate = v8::Isolate::GetCurrent();

    std::stringstream stream;
    stream << "[";
    // Check if not inside a worker
    if(!(*static_cast<bool*>(isolate->GetData(v8::Isolate::GetNumberOfDataSlots() - 1))))
    {
        stream << V8ResourceImpl::Get(context.Get(v8::Isolate::GetCurrent()))->GetResource()->GetName() << ":";
    }
    stream << fileName << ":" << line << "]";
    return stream.str();
}

V8Helpers::StackTrace V8Helpers::StackTrace::GetCurrent(v8::Isolate* isolate, V8ResourceImpl* resource)
{
    v8::Local<v8::StackTrace> stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 5);
    auto ctx = isolate->GetEnteredOrMicrotaskContext();
    std::vector<Frame> frames;
    for(int i = 0; i < stackTrace->GetFrameCount(); i++)
    {
        v8::Local<v8::StackFrame> frame = stackTrace->GetFrame(isolate, i);
        Frame frameData;
        frameData.file = GetStackFrameScriptName(frame);
        bool isBytecode = false;
#ifdef ALT_CLIENT_API
        isBytecode = resource ? static_cast<CV8ResourceImpl*>(resource)->GetModuleData(frameData.file).isBytecode : false;
#endif
        frameData.line = isBytecode ? 0 : frame->GetLineNumber();
        if(frame->GetFunctionName().IsEmpty()) frameData.function = "[anonymous]";
        else
            frameData.function = *v8::String::Utf8Value(isolate, frame->GetFunctionName());

        frames.push_back(std::move(frameData));
    }

    return StackTrace{ std::move(frames), ctx };
}

V8Helpers::StackTrace::StackTrace(std::vector<Frame>&& frames, v8::Local<v8::Context> ctx) : frames(frames), context(ctx->GetIsolate(), ctx) {}

void V8Helpers::StackTrace::Print(uint32_t offset) const
{
    Log::Error << ToString() << Log::Endl;
}

std::string V8Helpers::StackTrace::ToString(uint32_t offset) const
{
    std::stringstream stream;
    auto& frames = GetFrames();
    size_t size = frames.size();

    for(size_t i = offset; i < size; i++)
    {
        const Frame& frame = frames[i];
        stream << "  at " << frame.function << " (" << frame.file << ":" << frame.line << ")"
               << "\n";
    }
    return stream.str();
}

void V8Helpers::StackTrace::Print(v8::Isolate* isolate)
{
    V8Helpers::StackTrace trace = V8Helpers::StackTrace::GetCurrent(isolate);
    trace.Print();
}

v8::Local<v8::String> V8Helpers::Vector3_XKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> xKey{ isolate, v8::String::NewFromUtf8(isolate, "x", v8::NewStringType::kInternalized).ToLocalChecked() };

    return xKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::Vector3_YKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> yKey{ isolate, v8::String::NewFromUtf8(isolate, "y", v8::NewStringType::kInternalized).ToLocalChecked() };

    return yKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::Vector3_ZKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> zKey{ isolate, v8::String::NewFromUtf8(isolate, "z", v8::NewStringType::kInternalized).ToLocalChecked() };

    return zKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::RGBA_RKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> rKey{ isolate, v8::String::NewFromUtf8(isolate, "r", v8::NewStringType::kInternalized).ToLocalChecked() };

    return rKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::RGBA_GKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> gKey{ isolate, v8::String::NewFromUtf8(isolate, "g", v8::NewStringType::kInternalized).ToLocalChecked() };

    return gKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::RGBA_BKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> bKey{ isolate, v8::String::NewFromUtf8(isolate, "b", v8::NewStringType::kInternalized).ToLocalChecked() };

    return bKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::RGBA_AKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> aKey{ isolate, v8::String::NewFromUtf8(isolate, "a", v8::NewStringType::kInternalized).ToLocalChecked() };

    return aKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::Fire_PosKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> aKey{ isolate, v8::String::NewFromUtf8(isolate, "pos", v8::NewStringType::kInternalized).ToLocalChecked() };

    return aKey.Get(isolate);
}

v8::Local<v8::String> V8Helpers::Fire_WeaponKey(v8::Isolate* isolate)
{
    static v8::Persistent<v8::String> aKey{ isolate, v8::String::NewFromUtf8(isolate, "weapon", v8::NewStringType::kInternalized).ToLocalChecked() };

    return aKey.Get(isolate);
}

std::vector<V8Helpers::EventCallback*> V8Helpers::EventHandler::GetCallbacks(V8ResourceImpl* impl, const alt::CEvent* e)
{
    return callbacksGetter(impl, e);
}

std::vector<v8::Local<v8::Value>> V8Helpers::EventHandler::GetArgs(V8ResourceImpl* impl, const alt::CEvent* e)
{
    std::vector<v8::Local<v8::Value>> args;
    argsGetter(impl, e, args);
    return args;
}

V8Helpers::EventHandler* V8Helpers::EventHandler::Get(const alt::CEvent* e)
{
    auto& _all = all();
    auto it = _all.find(e->GetType());

    return (it != _all.end()) ? it->second : nullptr;
}

alt::CEvent::Type V8Helpers::EventHandler::GetTypeForEventName(const std::string& event)
{
    // TODO: Shitty temp workaround
#ifdef ALT_CLIENT_API
    if(event == "keyup" || event == "keydown") return alt::CEvent::Type::KEYBOARD_EVENT;
#endif
#ifdef ALT_SERVER_API
    if(event == "entityEnterColshape" || event == "entityLeaveColshape") return alt::CEvent::Type::COLSHAPE_EVENT;
#endif
    auto result = eventNameToHandlerMap().find(event);
    if(result == eventNameToHandlerMap().end()) return alt::CEvent::Type::NONE;
    return result->second->GetType();
}

void V8Helpers::EventHandler::Register(alt::CEvent::Type type, EventHandler* handler)
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

V8Helpers::EventHandler::CallbacksGetter V8Helpers::LocalEventHandler::GetCallbacksGetter(const std::string& name)
{
    return [name](V8ResourceImpl* resource, const alt::CEvent*) -> std::vector<EventCallback*> { return resource->GetLocalHandlers(name); };
}

V8Helpers::EventHandler::EventHandler(alt::CEvent::Type type, CallbacksGetter&& _handlersGetter, ArgsGetter&& _argsGetter)
    : callbacksGetter(std::move(_handlersGetter)), argsGetter(std::move(_argsGetter)), type(type)
{
    Register(type, this);
}

// Temp issue fix for https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
void V8Helpers::EventHandler::Reference()
{
    // Log::Info << "[V8] Registered handler for " << std::to_string((int)type) << Log::Endl;
    (void)type;
}

std::string V8Helpers::Stringify(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val)
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

std::string V8Helpers::GetJSValueTypeName(v8::Local<v8::Value> val)
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
    if(val->IsModuleNamespaceObject()) return "modulenamespace";
    if(val->IsObject()) return "object";
    else
        return "unknown";
}

v8::MaybeLocal<v8::Value> V8Helpers::CallFunctionWithTimeout(v8::Local<v8::Function> fn, v8::Local<v8::Context> ctx, std::vector<v8::Local<v8::Value>>& args, uint32_t timeout)
{
    v8::Isolate* isolate = ctx->GetIsolate();
    /*std::shared_ptr<bool> hasTimedOut{ new bool(false) };
    std::shared_ptr<bool> hasFinished{ new bool(false) };
    std::thread([=]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        if(*hasFinished || isolate->IsExecutionTerminating()) return;
        *hasTimedOut = true;
        isolate->TerminateExecution();
    }).detach();*/

    v8::MaybeLocal<v8::Value> result = fn->Call(ctx, v8::Undefined(isolate), args.size(), args.data());
    /**hasFinished = true;
    if(*hasTimedOut)
    {
        Log::Error << "[V8] Script execution timed out" << Log::Endl;
        return v8::MaybeLocal<v8::Value>();
    }*/
    return result;
}
