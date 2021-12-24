#include "CWorker.h"
#include "Globals.h"
#include "libplatform/libplatform.h"
#include "../CV8ScriptRuntime.h"
#include "../CV8Resource.h"
#include "V8Module.h"
#include "WorkerTimer.h"
#include "V8FastFunction.h"

#include <functional>

CWorker::CWorker(alt::String& filePath, alt::String& origin, CV8ResourceImpl* resource) : filePath(filePath), origin(origin), resource(resource) {}

void CWorker::Start()
{
    thread = std::thread(std::bind(&CWorker::Thread, this));
    thread.detach();
    CV8ScriptRuntime::Instance().AddActiveWorker();
}

void CWorker::Destroy()
{
    shouldTerminate = true;
    if(isolate && !isPaused) CV8ScriptRuntime::Instance().RemoveActiveWorker();
}

void CWorker::EmitToWorker(const std::string& eventName, std::vector<V8Helpers::Serialization::Value>& args)
{
    std::scoped_lock lock(worker_queueLock);
    worker_queuedEvents.push(std::make_pair(eventName, std::move(args)));
}

void CWorker::EmitToMain(const std::string& eventName, std::vector<V8Helpers::Serialization::Value>& args)
{
    std::scoped_lock lock(main_queueLock);
    main_queuedEvents.push(std::make_pair(eventName, std::move(args)));
}

void CWorker::SubscribeToWorker(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    auto isolate = v8::Isolate::GetCurrent();
    worker_eventHandlers.insert({ eventName, V8Helpers::EventCallback(isolate, callback, V8Helpers::SourceLocation::GetCurrent(isolate), once) });
}

void CWorker::SubscribeToMain(const std::string& eventName, v8::Local<v8::Function> callback, bool once)
{
    auto isolate = v8::Isolate::GetCurrent();
    main_eventHandlers.insert({ eventName, V8Helpers::EventCallback(isolate, callback, V8Helpers::SourceLocation::GetCurrent(isolate), once) });
}

void CWorker::Thread()
{
    bool result = SetupIsolate();
    if(!result) shouldTerminate = true;
    else
    {
        // Isolate is set up, the worker is now ready
        isReady = true;
        EmitToMain("load", std::vector<V8Helpers::Serialization::Value>());

        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Context::Scope context_scope(context.Get(isolate));
        while(true)
        {
            // Sleep for a short while to not overload the thread
            std::this_thread::sleep_for(std::chrono::microseconds(500));
            if(!EventLoop()) break;
        }
    }
    DestroyIsolate();
    delete this;  // ! IMPORTANT TO DO THIS LAST !
}

bool CWorker::EventLoop()
{
    if(shouldTerminate) return false;
    if(isPaused) return true;

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
        auto location = V8Helpers::SourceLocation::GetCurrent(isolate);

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

    isolate->SetHostImportModuleDynamicallyCallback(
      [](v8::Local<v8::Context> context, v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> assertions) {
          v8::Isolate* isolate = context->GetIsolate();
          v8::MaybeLocal<v8::Promise::Resolver> maybeResolver = v8::Promise::Resolver::New(context);
          if(maybeResolver.IsEmpty()) return v8::MaybeLocal<v8::Promise>();
          v8::Local<v8::Promise::Resolver> resolver = maybeResolver.ToLocalChecked();

          CWorker* worker = static_cast<CWorker*>(context->GetAlignedPointerFromEmbedderData(2));
          v8::Local<v8::Module> referrerModule = worker->GetModuleFromPath(*v8::String::Utf8Value(isolate, referrer->GetResourceName()));
          if(referrerModule.IsEmpty()) resolver->Reject(context, v8::Exception::ReferenceError(V8Helpers::JSValue("Could not resolve referrer module")));
          else
          {
              v8::MaybeLocal<v8::Module> maybeModule = CWorker::Import(context, specifier, assertions, referrerModule);
              if(maybeModule.IsEmpty()) resolver->Reject(context, v8::Exception::ReferenceError(V8Helpers::JSValue("Could not resolve module")));
              else
              {
                  v8::Local<v8::Module> module = maybeModule.ToLocalChecked();
                  if((module->GetStatus() != v8::Module::Status::kEvaluated && module->GetStatus() != v8::Module::Status::kErrored) &&
                     !module->InstantiateModule(context, Import).ToChecked())
                      resolver->Reject(context, v8::Exception::ReferenceError(V8Helpers::JSValue("Error instantiating module")));

                  if((module->GetStatus() != v8::Module::Status::kEvaluated && module->GetStatus() != v8::Module::Status::kErrored) && module->Evaluate(context).IsEmpty())
                      resolver->Reject(context, v8::Exception::ReferenceError(V8Helpers::JSValue("Error evaluating module")));

                  else
                      resolver->Resolve(context, module->GetModuleNamespace());
              }
          }

          return v8::MaybeLocal<v8::Promise>(resolver->GetPromise());
      });

    isolate->SetHostInitializeImportMetaObjectCallback([](v8::Local<v8::Context> context, v8::Local<v8::Module>, v8::Local<v8::Object> meta) {
        meta->CreateDataProperty(context, V8Helpers::JSValue("url"), V8Helpers::JSValue(V8Helpers::GetCurrentSourceOrigin(context->GetIsolate())));
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
    ctx->SetAlignedPointerInEmbedderData(1, resource->GetResource());
    ctx->SetAlignedPointerInEmbedderData(2, this);
    SetupGlobals(ctx->Global());

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

    bool failed = false;
    // Compile the code
    auto error = TryCatch([&]() {
        std::string fullPath = (path.prefix + path.fileName).ToString();
        v8::ScriptOrigin scriptOrigin(isolate, V8Helpers::JSValue(fullPath), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());
        v8::ScriptCompiler::Source source(V8Helpers::JSValue(src), scriptOrigin);
        auto maybeModule = v8::ScriptCompiler::CompileModule(isolate, &source);
        if(maybeModule.IsEmpty())
        {
            EmitError("Failed to compile worker module");
            failed = true;
            return;
        }
        auto module = maybeModule.ToLocalChecked();

        modules.emplace(fullPath, v8::UniquePersistent<v8::Module>{ isolate, module });

        // Start the code
        v8::Maybe<bool> result = module->InstantiateModule(ctx, Import);
        if(result.IsNothing() || result.ToChecked() == false)
        {
            EmitError("Failed to instantiate worker module");
            failed = true;
            modules.erase(fullPath);
            return;
        }

        // Evaluate the code
        auto returnValue = module->Evaluate(ctx);
        if(returnValue.IsEmpty())
        {
            EmitError("Failed to evaluate worker module");
            failed = true;
            modules.erase(fullPath);
            return;
        }
    });
    if(!error.empty() || failed)
    {
        if(!error.empty()) EmitError(error);
        return false;
    }

    return true;
}

void CWorker::DestroyIsolate()
{
    while(isolate->IsInUse()) isolate->Exit();
    V8Module::Clear(isolate);
    V8Class::UnloadAll(isolate);
    V8FastFunction::UnloadAll(isolate);
    context.Reset();
    v8::platform::NotifyIsolateShutdown(CV8ScriptRuntime::Instance().GetPlatform(), isolate);
    isolate->Dispose();
}

extern void StaticRequire(const v8::FunctionCallbackInfo<v8::Value>& info);
extern V8Module altWorker;
void CWorker::SetupGlobals(v8::Local<v8::Object> global)
{
    V8Class::LoadAll(isolate);
    V8Module::Add(isolate, { altWorker });

    auto alt = altWorker.GetExports(isolate, context.Get(isolate));
    auto console = global->Get(context.Get(isolate), V8Helpers::JSValue("console")).ToLocalChecked().As<v8::Object>();
    if(!console.IsEmpty())
    {
        console->Set(context.Get(isolate), V8Helpers::JSValue("log"), alt->Get(context.Get(isolate), V8Helpers::JSValue("log")).ToLocalChecked());
        console->Set(context.Get(isolate), V8Helpers::JSValue("warn"), alt->Get(context.Get(isolate), V8Helpers::JSValue("logWarning")).ToLocalChecked());
        console->Set(context.Get(isolate), V8Helpers::JSValue("error"), alt->Get(context.Get(isolate), V8Helpers::JSValue("logError")).ToLocalChecked());
    }

    V8Helpers::RegisterFunc(global, "setInterval", &SetInterval);
    V8Helpers::RegisterFunc(global, "setTimeout", &SetTimeout);
    V8Helpers::RegisterFunc(global, "clearInterval", &ClearTimer);
    V8Helpers::RegisterFunc(global, "clearTimeout", &ClearTimer);

    global->Set(context.Get(isolate), V8Helpers::JSValue("__internal_get_exports"), v8::Function::New(context.Get(isolate), &StaticRequire).ToLocalChecked());
}

v8::MaybeLocal<v8::Module> CWorker::Import(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray>, v8::Local<v8::Module> referrer)
{
    CWorker* worker = static_cast<CWorker*>(context->GetAlignedPointerFromEmbedderData(2));
    std::string importName = *v8::String::Utf8Value(worker->GetIsolate(), specifier);
    alt::IResource* resource = worker->GetResource()->GetResource();

    return worker->ResolveModule(importName, referrer, resource);
}

void CWorker::EmitError(const std::string& error)
{
    Log::Error << "[Worker] " << error << Log::Endl;
    std::vector<V8Helpers::Serialization::Value> args = { V8Helpers::Serialization::Serialize(context.Get(isolate), V8Helpers::JSValue(error)) };
    EmitToMain("error", args);
}

static inline void RunEventQueue(CWorker::EventQueue& queue, CWorker::EventHandlerMap& eventHandlers, std::mutex& queueMutex)
{
    if(queue.empty()) return;

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetEnteredOrMicrotaskContext();
    std::scoped_lock lock(queueMutex);

    // Clear removed event handlers
    for(auto it = eventHandlers.begin(); it != eventHandlers.end();)
    {
        if(it->second.removed) it = eventHandlers.erase(it);
        else
            ++it;
    }

    while(!queue.empty())
    {
        // Get the event at the front of the queue
        auto& event = queue.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args;
        args.reserve(event.second.size());
        for(auto& arg : event.second)
        {
            auto value = V8Helpers::Serialization::Deserialize(context, arg);
            if(value.IsEmpty())
            {
                Log::Error << "Failed to deserialize worker event argument" << Log::Endl;
                continue;
            }
            args.push_back(value.ToLocalChecked());
        }

        // Call all handlers with the arguments
        auto handlers = eventHandlers.equal_range(event.first);
        for(auto it = handlers.first; it != handlers.second; it++)
        {
            V8Helpers::CallFunctionWithTimeout(it->second.fn.Get(isolate), context, args);
            if(it->second.once) it->second.removed = true;
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

CWorker::TimerId CWorker::CreateTimer(v8::Local<v8::Function> callback, uint32_t interval, bool once, V8Helpers::SourceLocation&& location)
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
    if(tryCatch.HasCaught())
    {
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
            return std::string("An error occured");
    }
    else
        return std::string();
}

// Shared array buffers
CWorker::BufferId CWorker::nextBufferId = 0;
std::unordered_map<CWorker::BufferId, std::shared_ptr<v8::BackingStore>> CWorker::sharedArrayBuffers = std::unordered_map<CWorker::BufferId, std::shared_ptr<v8::BackingStore>>();

CWorker::BufferId CWorker::AddSharedArrayBuffer(v8::Local<v8::SharedArrayBuffer> buffer)
{
    BufferId id = ++nextBufferId;
    sharedArrayBuffers.insert({ id, buffer->GetBackingStore() });
    return id;
}
bool CWorker::RemoveSharedArrayBuffer(CWorker::BufferId index)
{
    if(sharedArrayBuffers.count(index) == 0) return false;
    sharedArrayBuffers.erase(index);
    return true;
}

v8::Local<v8::SharedArrayBuffer> CWorker::GetSharedArrayBuffer(v8::Isolate* isolate, CWorker::BufferId index)
{
    if(sharedArrayBuffers.count(index) == 0) return v8::Local<v8::SharedArrayBuffer>();
    return v8::SharedArrayBuffer::New(isolate, sharedArrayBuffers.at(index));
}
