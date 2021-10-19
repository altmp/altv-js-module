#include "CWorker.h"
#include "Globals.h"
#include "libplatform/libplatform.h"
#include "../CV8ScriptRuntime.h"
#include "../CV8Resource.h"
#include "V8Module.h"
#include "WorkerTimer.h"

#include <functional>

CWorker::CWorker(alt::String& filePath, alt::String& origin, CV8ResourceImpl* resource) : filePath(filePath), origin(origin), resource(resource) {}

void CWorker::Start()
{
    thread = std::thread(std::bind(&CWorker::Thread, this));
    thread.detach();
}

void CWorker::EmitToWorker(const std::string& eventName, std::vector<alt::MValue>& args)
{
    std::unique_lock<std::mutex> lock(worker_queueLock);
    worker_queuedEvents.push(std::make_pair(eventName, std::move(args)));
}

void CWorker::EmitToMain(const std::string& eventName, std::vector<alt::MValue>& args)
{
    std::unique_lock<std::mutex> lock(main_queueLock);
    main_queuedEvents.push(std::make_pair(eventName, std::move(args)));
}

void CWorker::SubscribeToWorker(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    auto isolate = v8::Isolate::GetCurrent();
    worker_eventHandlers.insert({ eventName, V8::EventCallback(isolate, callback, V8::SourceLocation::GetCurrent(isolate), once) });
}

void CWorker::SubscribeToMain(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    auto isolate = v8::Isolate::GetCurrent();
    main_eventHandlers.insert({ eventName, V8::EventCallback(isolate, callback, V8::SourceLocation::GetCurrent(isolate), once) });
}

void CWorker::Thread()
{
    bool result = SetupIsolate();
    if(!result) shouldTerminate = true;
    else
    {
        // Isolate is set up, the worker is now ready
        isReady = true;
        EmitToMain("load", std::vector<alt::MValue>());
    }

    while(true)
    {
        // Sleep for a short while to not overload the thread
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        if(!EventLoop()) break;
    }

    DestroyIsolate();
    delete this;  // ! IMPORTANT TO DO THIS LAST !
}

bool CWorker::EventLoop()
{
    if(shouldTerminate) return false;
    if(isPaused) return true;

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(context.Get(isolate));

    // Timers
    for(auto& id : oldTimers) timers.erase(id);
    oldTimers.clear();

    for(auto& p : timers)
    {
        int64_t time = GetTime();
        if(!p.second->Update(time)) RemoveTimer(p.first);
    }

    auto error = TryCatch([&]() {
        microtaskQueue->PerformCheckpoint(isolate);
        HandleWorkerEventQueue();
        v8::platform::PumpMessageLoop(CV8ScriptRuntime::Instance().GetPlatform(), isolate);
    });
    if(!error.empty())
    {
        EmitError(error);
    }

    promiseRejections.ProcessQueue(this);

    return true;
}

bool CWorker::SetupIsolate()
{
    // Create the isolate
    v8::Isolate::CreateParams params;
    params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(params);

    isolate->SetFatalErrorHandler([](const char* location, const char* message) { Log::Error << "[Worker] " << location << ": " << message << Log::Endl; });

    isolate->SetPromiseRejectCallback([](v8::PromiseRejectMessage message) {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));
        if(!worker) return;

        v8::Local<v8::Value> value = message.GetValue();
        if(value.IsEmpty()) value = v8::Undefined(isolate);
        auto location = V8::SourceLocation::GetCurrent(isolate);

        switch(message.GetEvent())
        {
            case v8::kPromiseRejectWithNoHandler:
            {
                worker->OnPromiseRejectedWithNoHandler(message);
                break;
            }
            case v8::kPromiseHandlerAddedAfterReject:
            {
                worker->OnPromiseHandlerAdded(message);
                break;
            }
            case v8::kPromiseRejectAfterResolved:
            {
                std::ostringstream stream;
                stream << location.ToString() << " Promise rejected after being resolved (" << *v8::String::Utf8Value(isolate, value->ToString(ctx).ToLocalChecked()) << ")";
                worker->EmitError(stream.str());
                break;
            }
            case v8::kPromiseResolveAfterResolved:
            {
                std::ostringstream stream;
                stream << location.ToString() << " Promise resolved after being resolved (" << *v8::String::Utf8Value(isolate, value->ToString(ctx).ToLocalChecked()) << ")";
                worker->EmitError(stream.str());
                break;
            }
        }
    });

    // Disable creating shared array buffers in Workers
    isolate->SetSharedArrayBufferConstructorEnabledCallback([](v8::Local<v8::Context>) { return false; });

    // IsWorker data slot
    isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 1, new bool(true));

    // Set up locker and scopes
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    microtaskQueue = v8::MicrotaskQueue::New(isolate, v8::MicrotasksPolicy::kExplicit);

    // Create and set up the context
    auto ctx = v8::Context::New(isolate, nullptr, v8::MaybeLocal<v8::ObjectTemplate>(), v8::MaybeLocal<v8::Value>(), v8::DeserializeInternalFieldsCallback(), microtaskQueue.get());
    context.Reset(isolate, ctx);
    v8::Context::Scope scope(ctx);
    SetupGlobals(ctx->Global());
    ctx->SetAlignedPointerInEmbedderData(2, this);

    // Load code
    auto path = alt::ICore::Instance().Resolve(resource->GetResource(), filePath, origin);
    if(!path.pkg || !path.pkg->FileExists(path.fileName))
    {
        EmitError("Worker file not found");
        return false;
    }
    auto file = path.pkg->OpenFile(path.fileName);
    std::string src(path.pkg->GetFileSize(file), '\0');
    auto size = path.pkg->ReadFile(file, src.data(), src.size());
    path.pkg->CloseFile(file);

    // Compile the code
    auto error = TryCatch([&]() {
        v8::ScriptOrigin scriptOrigin(isolate, V8::JSValue(path.fileName.ToString()), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());
        v8::ScriptCompiler::Source source(V8::JSValue(src), scriptOrigin);
        auto maybeModule = v8::ScriptCompiler::CompileModule(isolate, &source);
        if(maybeModule.IsEmpty())
        {
            EmitError("Failed to compile worker module");
            return;
        }
        auto module = maybeModule.ToLocalChecked();

        // Start the code
        v8::Maybe<bool> result =
          module->InstantiateModule(ctx, [](v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer) {
              // todo: implement imports
              return v8::MaybeLocal<v8::Module>();
          });
        if(result.IsNothing() || result.ToChecked() == false)
        {
            EmitError("Failed to instantiate worker module");
            return;
        }

        // Evaluate the code
        auto returnValue = module->Evaluate(ctx);
        if(returnValue.IsEmpty())
        {
            EmitError("Failed to evaluate worker module");
            return;
        }
    });
    if(!error.empty())
    {
        EmitError(error);
        return false;
    }

    return true;
}

void CWorker::DestroyIsolate()
{
    while(isolate->IsInUse()) isolate->Exit();
    context.Reset();
    isolate->Dispose();
}

extern V8Module altModule;
void CWorker::SetupGlobals(v8::Local<v8::Object> global)
{
    V8_NEW_OBJECT(alt);
    V8Helpers::RegisterFunc(alt, "emit", &Emit);
    V8Helpers::RegisterFunc(alt, "on", &On);
    V8Helpers::RegisterFunc(alt, "once", &Once);
    V8Helpers::RegisterFunc(alt, "log", &::Log);
    V8Helpers::RegisterFunc(alt, "logWarning", &::LogWarning);
    V8Helpers::RegisterFunc(alt, "logError", &::LogError);
    V8Helpers::RegisterFunc(alt, "nextTick", &NextTick);
    V8Helpers::RegisterFunc(alt, "setInterval", &SetInterval);
    V8Helpers::RegisterFunc(alt, "setTimeout", &SetTimeout);
    V8Helpers::RegisterFunc(alt, "clearNextTick", &ClearTimer);
    V8Helpers::RegisterFunc(alt, "clearInterval", &ClearTimer);
    V8Helpers::RegisterFunc(alt, "clearTimeout", &ClearTimer);
    global->Set(context.Get(isolate), V8_NEW_STRING("alt"), alt);

    auto console = global->Get(context.Get(isolate), V8_NEW_STRING("console")).ToLocalChecked().As<v8::Object>();
    if(!console.IsEmpty())
    {
        V8Helpers::RegisterFunc(console, "log", &::Log);
        V8Helpers::RegisterFunc(console, "warn", &::LogWarning);
        V8Helpers::RegisterFunc(console, "error", &::LogError);
    }

    V8Helpers::RegisterFunc(global, "setInterval", &SetInterval);
    V8Helpers::RegisterFunc(global, "setTimeout", &SetTimeout);
    V8Helpers::RegisterFunc(global, "clearInterval", &ClearTimer);
    V8Helpers::RegisterFunc(global, "clearTimeout", &ClearTimer);
}

void CWorker::EmitError(const std::string& error)
{
    Log::Error << "[Worker] " << error << Log::Endl;
    std::vector<alt::MValue> args = { alt::ICore::Instance().CreateMValueString(error) };
    EmitToMain("error", args);
}

static inline void RunEventQueue(CWorker::EventQueue& queue, CWorker::EventHandlerMap& eventHandlers, std::mutex& queueMutex)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetEnteredOrMicrotaskContext();
    std::unique_lock<std::mutex> lock(queueMutex);

    while(!queue.empty())
    {
        // Get the event at the front of the queue
        auto& event = queue.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args;
        args.reserve(event.second.size());
        for(auto& arg : event.second)
        {
            args.push_back(V8Helpers::MValueToV8(arg));
        }

        // Call all handlers with the arguments
        auto handlers = eventHandlers.equal_range(event.first);
        for(auto it = handlers.first; it != handlers.second; it++)
        {
            it->second.fn.Get(isolate)->Call(context, v8::Undefined(isolate), args.size(), args.data());
            if(it->second.once) eventHandlers.erase(it);
        }

        // Pop the event from the queue
        queue.pop();
    }
}
void CWorker::HandleMainEventQueue()
{
    RunEventQueue(main_queuedEvents, main_eventHandlers, main_queueLock);
}

void CWorker::HandleWorkerEventQueue()
{
    RunEventQueue(worker_queuedEvents, worker_eventHandlers, worker_queueLock);
}

CWorker::TimerId CWorker::CreateTimer(v8::Local<v8::Function> callback, uint32_t interval, bool once, V8::SourceLocation&& location)
{
    TimerId id = nextTimerId++;
    timers.insert({ id, new WorkerTimer(this, isolate, context.Get(isolate), GetTime(), callback, interval, once, std::move(location)) });
    return id;
}

std::string CWorker::TryCatch(const std::function<void()>& func)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::TryCatch tryCatch(isolate);
    std::ostringstream stream;

    func();

    // Check if an error occured
    v8::Local<v8::Value> exception = tryCatch.Exception();
    v8::Local<v8::Message> message = tryCatch.Message();
    if(!message.IsEmpty())
    {
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        v8::MaybeLocal<v8::String> maybeSourceLine = message->GetSourceLine(ctx);
        v8::Maybe<int32_t> line = message->GetLineNumber(ctx);
        v8::ScriptOrigin origin = message->GetScriptOrigin();

        // Location
        stream << "[" << *v8::String::Utf8Value(isolate, origin.ResourceName()) << ":" << (line.IsNothing() ? 0 : line.FromJust()) << "] ";

        // Add stack trace if exists
        v8::MaybeLocal<v8::Value> stackTrace = tryCatch.StackTrace(ctx);
        if(!stackTrace.IsEmpty() && stackTrace.ToLocalChecked()->IsString())
        {
            v8::String::Utf8Value stackTraceStr(isolate, stackTrace.ToLocalChecked().As<v8::String>());
            stream << *stackTraceStr;
        }

        return stream.str();
    }
    else if(!exception.IsEmpty())
    {
        stream << *v8::String::Utf8Value(isolate, exception);
        return stream.str();
    }
    else
        return std::string();
}
