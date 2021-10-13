#include "CWorker.h"
#include "Globals.h"
#include "libplatform/libplatform.h"
#include "../CV8ScriptRuntime.h"
#include "../CV8Resource.h"

#include <functional>

CWorker::CWorker(const std::string& filePath, CV8ResourceImpl* resource) : filePath(filePath), resource(resource) {}

void CWorker::Start()
{
    thread = std::thread(std::bind(&CWorker::Thread, this));
    thread.detach();
}

void CWorker::Destroy()
{
    shouldTerminate = true;
}

void CWorker::EmitToWorker(const std::string& eventName, std::vector<alt::MValue>& args)
{
    std::unique_lock<std::mutex> lock(worker_queueLock);
    worker_queuedEvents.push(std::make_pair(eventName, args));
}

void CWorker::EmitToMain(const std::string& eventName, std::vector<alt::MValue>& args)
{
    std::unique_lock<std::mutex> lock(main_queueLock);
    main_queuedEvents.push(std::make_pair(eventName, args));
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

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    HandleWorkerEventQueue();
    v8::platform::PumpMessageLoop(CV8ScriptRuntime::Instance().GetPlatform(), isolate);

    return true;
}

bool CWorker::SetupIsolate()
{
    // Create the isolate
    v8::Isolate::CreateParams params;
    params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(params);

    // Set up locker and scopes
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    // Create and set up the context
    context = v8::Context::New(isolate);
    v8::Context::Scope scope(context);
    SetupGlobals(context->Global());

    // Load code
    auto path = alt::ICore::Instance().Resolve(resource->GetResource(), filePath, "");
    if(!path.pkg)
    {
        Log::Error << "Worker file not found" << Log::Endl;
        return false;
    }
    auto file = path.pkg->OpenFile(path.fileName);
    std::string src(path.pkg->GetFileSize(file), '\0');
    path.pkg->ReadFile(file, src.data(), src.size());
    path.pkg->CloseFile(file);

    // Compile the code
    v8::ScriptOrigin scriptOrigin(isolate, V8::JSValue(path.fileName.ToString()), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());
    v8::ScriptCompiler::Source source(V8::JSValue(src), scriptOrigin);
    auto maybeModule = v8::ScriptCompiler::CompileModule(isolate, &source);
    if(maybeModule.IsEmpty())
    {
        Log::Error << "Failed to compile worker module" << Log::Endl;
        return false;
    }
    auto module = maybeModule.ToLocalChecked();

    // Start the code
    v8::Maybe<bool> result =
      module->InstantiateModule(context, [](v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer) {
          // todo: implement imports
          return v8::MaybeLocal<v8::Module>();
      });
    if(result.IsNothing() || result.ToChecked() == false)
    {
        Log::Error << "Failed to instantiate worker module" << Log::Endl;
        return false;
    }

    // Evaluate the code
    auto returnValue = module->Evaluate(context);
    if(returnValue.IsEmpty())
    {
        Log::Error << "Failed to evaluate worker module" << Log::Endl;
        return false;
    }

    return true;
}

void CWorker::DestroyIsolate()
{
    // todo: clean up isolate and stuff
    // Call destroy handler
    if(!destroyHandler.IsEmpty()) destroyHandler->Call(context, v8::Undefined(isolate), 0, nullptr);
    isolate->Dispose();
}

void CWorker::SetupGlobals(v8::Local<v8::Object> global)
{
    V8Helpers::RegisterFunc(global, "emit", &Emit);
    V8Helpers::RegisterFunc(global, "on", &On);
    V8Helpers::RegisterFunc(global, "once", &Once);
    V8Helpers::RegisterFunc(global, "setDestroyHandler", &::SetDestroyHandler);
}

void CWorker::HandleMainEventQueue()
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetEnteredOrMicrotaskContext();
    std::unique_lock<std::mutex> lock(main_queueLock);

    while(!main_queuedEvents.empty())
    {
        // Get the event at the front of the queue
        auto& event = main_queuedEvents.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args(event.second.size());
        for(auto& arg : event.second)
        {
            args.push_back(V8Helpers::MValueToV8(arg));
        }

        // Call all handlers with the arguments
        auto handlers = main_eventHandlers.equal_range(event.first);
        for(auto it = handlers.first; it != handlers.second; it++)
        {
            it->second.fn.Get(isolate)->Call(context, v8::Undefined(isolate), args.size(), args.data());
            if(it->second.once) main_eventHandlers.erase(it);
        }

        // Pop the event from the queue
        main_queuedEvents.pop();
    }
}

void CWorker::HandleWorkerEventQueue()
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetEnteredOrMicrotaskContext();
    std::unique_lock<std::mutex> lock(worker_queueLock);

    while(!worker_queuedEvents.empty())
    {
        // Get the event at the front of the queue
        auto& event = worker_queuedEvents.front();

        // Create a vector of the event arguments
        std::vector<v8::Local<v8::Value>> args(event.second.size());
        for(auto& arg : event.second)
        {
            args.push_back(V8Helpers::MValueToV8(arg));
        }

        // Call all handlers with the arguments
        auto handlers = worker_eventHandlers.equal_range(event.first);
        for(auto it = handlers.first; it != handlers.second; it++)
        {
            it->second.fn.Get(isolate)->Call(context, v8::Undefined(isolate), args.size(), args.data());
            if(it->second.once) worker_eventHandlers.erase(it);
        }

        // Pop the event from the queue
        worker_queuedEvents.pop();
    }
}

void CWorker::SetDestroyHandler(v8::Local<v8::Function> handler)
{
    destroyHandler = handler;
}
