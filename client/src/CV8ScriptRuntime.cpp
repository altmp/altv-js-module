
#include "CV8ScriptRuntime.h"
#include "inspector/CV8InspectorClient.h"
#include "inspector/CV8InspectorChannel.h"
#include "V8Module.h"
#include "events/Events.h"
#include "CProfiler.h"

CV8ScriptRuntime::CV8ScriptRuntime()
{
    // !!! Don't change these without adjusting bytecode module !!!
    v8::V8::SetFlagsFromString("--harmony-import-assertions --short-builtin-calls --no-lazy --no-flush-bytecode");
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::InitializeICU((alt::ICore::Instance().GetClientPath() + "/libs/icudtl.dat").c_str());
    v8::V8::Initialize();

    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    create_params.allow_atomics_wait = false;

    isolate = v8::Isolate::New(create_params);
    isolate->SetFatalErrorHandler([](const char* location, const char* message) { Log::Error << "[V8] " << location << ": " << message << Log::Endl; });

    isolate->SetOOMErrorHandler(
      [](const char* location, bool isHeap)
      {
          if(!isHeap) return;
          Log::Error << "[V8] " << location << ": Heap out of memory. Forward this to the server developers." << Log::Endl;
          Log::Error << "[V8] The current heap limit can be shown with the 'heap' console command. Consider increasing your system RAM." << Log::Endl;
      });

    isolate->AddNearHeapLimitCallback(
      [](void*, size_t current, size_t initial)
      {
          Log::Warning << "[V8] The remaining V8 heap space is approaching critical levels. Forward this to the server developers." << Log::Endl;
          Log::Warning << "[V8] Initial heap limit: " << CV8ScriptRuntime::FormatBytes(initial) << " | Current heap limit: " << CV8ScriptRuntime::FormatBytes(current) << Log::Endl;

          // Increase the heap limit by 100MB if the heap limit has not exceeded 4GB
          uint64_t currentLimitMb = (current / 1024) / 1024;
          if(currentLimitMb < 4096) return current + (100 * 1024 * 1024);
          else
              return current;
      },
      nullptr);

    isolate->SetPromiseRejectCallback(
      [](v8::PromiseRejectMessage message)
      {
          v8::Isolate* isolate = v8::Isolate::GetCurrent();
          v8::Local<v8::Value> value = message.GetValue();
          v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

          CV8ResourceImpl* resource = static_cast<CV8ResourceImpl*>(V8ResourceImpl::Get(ctx));
          if(resource)
          {
              if(message.GetEvent() == v8::PromiseRejectEvent::kPromiseRejectWithNoHandler)
              {
                  resource->OnPromiseRejectedWithNoHandler(message);
              }
              else if(message.GetEvent() == v8::PromiseRejectEvent::kPromiseHandlerAddedAfterReject)
              {
                  resource->OnPromiseHandlerAdded(message);
              }
              else if(message.GetEvent() == v8::PromiseRejectEvent::kPromiseRejectAfterResolved)
              {
                  Log::Warning << "[V8] Promise rejected after being resolved in resource " << resource->GetResource()->GetName() << " ("
                               << *v8::String::Utf8Value(isolate, value->ToString(ctx).ToLocalChecked()) << ")" << Log::Endl;
              }
              else if(message.GetEvent() == v8::PromiseRejectEvent::kPromiseResolveAfterResolved)
              {
                  Log::Warning << "[V8] Promise resolved after being resolved in resource " << resource->GetResource()->GetName() << " ("
                               << *v8::String::Utf8Value(isolate, value->ToString(ctx).ToLocalChecked()) << ")" << Log::Endl;
              }
          }
          else
          {
              Log::Error << "You're not supposed to ever see this" << Log::Endl;
          }
      });

    isolate->SetHostImportModuleDynamicallyCallback(
      [](v8::Local<v8::Context> context, v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> importAssertions)
      {
          v8::Isolate* isolate = context->GetIsolate();

          auto referrerVal = referrer->GetResourceName();
          if(referrerVal->IsUndefined()) return v8::MaybeLocal<v8::Promise>();

          std::string referrerUrl = *v8::String::Utf8Value(isolate, referrer->GetResourceName());
          auto resource = static_cast<CV8ResourceImpl*>(V8ResourceImpl::Get(context));

          auto resolver = v8::Promise::Resolver::New(context).ToLocalChecked();

          V8Helpers::CPersistent<v8::Promise::Resolver> presolver(isolate, resolver);
          V8Helpers::CPersistent<v8::String> pspecifier(isolate, specifier);
          V8Helpers::CPersistent<v8::Module> preferrerModule(isolate, resource->GetModuleFromPath(referrerUrl));
          V8Helpers::CPersistent<v8::FixedArray> pimportAssertions(isolate, importAssertions);

          // careful what we take in by value in the lambda
          // it is possible pass v8::Local but should not be done
          // make a V8Helpers::CPersistent out of it and pass that
          auto domodule = [isolate, presolver, pspecifier, preferrerModule, pimportAssertions]
          {
              auto referrerModule = preferrerModule.Get(isolate);
              auto resolver = presolver.Get(isolate);
              auto specifier = pspecifier.Get(isolate);
              auto importAssertions = pimportAssertions.Get(isolate);

              auto ctx = resolver->GetCreationContext().ToLocalChecked();
              v8::Context::Scope ctxs(ctx);

              auto mmodule = ResolveModule(ctx, specifier, importAssertions, referrerModule);
              if(mmodule.IsEmpty())
              {
                  resolver->Reject(ctx, v8::Exception::ReferenceError(V8Helpers::JSValue("Could not resolve module")));
                  return;
              }

              auto mod = mmodule.ToLocalChecked();
              V8Helpers::TryCatch(
                [&]
                {
                    if(mod->GetStatus() == v8::Module::Status::kUninstantiated)
                    {
                        auto result = mod->InstantiateModule(ctx, ResolveModule);
                        if(result.IsNothing() || !result.FromJust())
                        {
                            resolver->Reject(ctx, v8::Exception::ReferenceError(V8Helpers::JSValue("Error instantiating module")));
                            return false;
                        }
                    }

                    if((mod->GetStatus() != v8::Module::Status::kEvaluated && mod->GetStatus() != v8::Module::Status::kErrored) && mod->Evaluate(ctx).IsEmpty())
                    {
                        resolver->Reject(ctx, v8::Exception::ReferenceError(V8Helpers::JSValue("Error evaluating module")));
                        return false;
                    }

                    resolver->Resolve(ctx, mod->GetModuleNamespace());
                    return true;
                });
          };

          if(Instance().resourcesLoaded && resource->GetResource()->IsStarted() || resource->IsPreloading())
          {
              // instantly resolve the module
              domodule();
          }
          else
          {
              // put it in the queue to resolve after all resource are loaded
              resource->dynamicImports.emplace_back(domodule);
          }

          return v8::MaybeLocal<v8::Promise>(resolver->GetPromise());
      });

    isolate->SetHostInitializeImportMetaObjectCallback(
      [](v8::Local<v8::Context> context, v8::Local<v8::Module>, v8::Local<v8::Object> meta)
      { meta->CreateDataProperty(context, V8Helpers::JSValue("url"), V8Helpers::JSValue(V8Helpers::GetCurrentSourceOrigin(context->GetIsolate()))); });

    isolate->AddMessageListener(
      [](v8::Local<v8::Message> message, v8::Local<v8::Value> error)
      {
          v8::Isolate* isolate = v8::Isolate::GetCurrent();

          switch(message->ErrorLevel())
          {
              case v8::Isolate::kMessageError:
              {
                  std::string errorMsg = *v8::String::Utf8Value(isolate, message->Get());
                  if(errorMsg.empty()) errorMsg = *v8::String::Utf8Value(isolate, error);
                  if(errorMsg.empty()) errorMsg = "<unknown>";
                  Log::Error << "[V8] Error: " << errorMsg << Log::Endl;
                  break;
              }
              default: break;
          }
      });

    isolate->SetMicrotasksPolicy(v8::MicrotasksPolicy::kExplicit);

    isolate->SetCaptureStackTraceForUncaughtExceptions(true, 5);

    /*{
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolate_scope(isolate);
            v8::HandleScope handle_scope(isolate);

            inspectorClient.reset(new alt::CV8InspectorClient);
            inspectorChannel.reset(new alt::CV8InspectorChannel);

            inspector = v8_inspector::V8Inspector::create(isolate, inspectorClient.get());

            v8_inspector::StringView inspectorView{ (uint8_t*)inspectorViewStr, strlen(inspectorViewStr) };
            inspectorSession = inspector->connect(1, inspectorChannel.get(), inspectorView);
    }*/

    profiler = v8::CpuProfiler::New(isolate);
    profiler->SetUsePreciseSampling(true);
    profiler->SetSamplingInterval(profilerSamplingInterval);
    v8::CpuProfiler::UseDetailedSourcePositionsForProfiling(isolate);

    // IsWorker data slot
    isolate->SetData(v8::Isolate::GetNumberOfDataSlots() - 1, new bool(false));

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);

        V8Class::LoadAll(isolate);

        extern V8Module altModule, nativesModule, sharedModule;
        V8Module::Add(isolate, { altModule, nativesModule, sharedModule });
    }

    RegisterEvents();

    ProcessConfigOptions();

    IRuntimeEventHandler::Start();
}

void CV8ScriptRuntime::ProcessConfigOptions()
{
    Config::Value::ValuePtr moduleConfig = alt::ICore::Instance().GetClientConfig()["js-module"];
    if(!moduleConfig->IsDict()) return;

    Config::Value::ValuePtr profiler = moduleConfig["profiler"];
    CProfiler::Instance().SetIsEnabled(profiler->AsBool(false));
}

void CV8ScriptRuntime::OnDispose()
{
    while(isolate->IsInUse()) isolate->Exit();
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;

    if(CProfiler::Instance().IsEnabled()) CProfiler::Instance().Dump(alt::ICore::Instance().GetClientPath());

    CV8ScriptRuntime::SetInstance(nullptr);
    delete this;
}

void CV8ScriptRuntime::Init()
{
    IRuntimeEventHandler::Reset();
}

static std::string Base64Decode(const std::string& in)
{
    std::string out;
    std::vector<int> T(256, -1);
    for(int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for(unsigned char c : in)
    {
        if(T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if(valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}
v8::MaybeLocal<v8::Module>
  CV8ScriptRuntime::ResolveModule(v8::Local<v8::Context> ctx, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> importAssertions, v8::Local<v8::Module> referrer)
{
    auto isolate = ctx->GetIsolate();
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
    if(!resource)
    {
        V8Helpers::Throw(isolate, "Invalid resource");
        return v8::MaybeLocal<v8::Module>{};
    }

    std::string _specifier = *v8::String::Utf8Value{ isolate, specifier };
    if(_specifier == resource->GetResource()->GetName())
    {
        V8Helpers::Throw(isolate, "Cannot import the resource itself (self-importing)");
        return v8::MaybeLocal<v8::Module>{};
    }

    if(importAssertions->Length() > 0)
    {
        std::string assertionKey = *v8::String::Utf8Value(isolate, importAssertions->Get(ctx, 0).As<v8::Value>().As<v8::String>());
        if(assertionKey != "type")
        {
            V8Helpers::Throw(isolate, "Invalid import assertion format");
            return v8::MaybeLocal<v8::Module>();
        }

        v8::Local<v8::Data> assertionValue = importAssertions->Get(ctx, 1);
        v8::Local<v8::Value> typeValue;
        std::string typeValueStr;
        if(assertionValue->IsValue() && assertionValue.As<v8::Value>()->IsString()) typeValueStr = *v8::String::Utf8Value(isolate, assertionValue.As<v8::Value>().As<v8::String>());
        if(typeValueStr.empty())
        {
            V8Helpers::Throw(isolate, "Invalid import assertion format");
            return v8::MaybeLocal<v8::Module>();
        }

        v8::Local<v8::Module> mod;
        bool result = V8Helpers::TryCatch(
          [&]
          {
              v8::MaybeLocal<v8::Module> maybeModule;
              std::string specifierStr = *v8::String::Utf8Value(isolate, specifier);

              // Check assertion type
              if(typeValueStr == "base64")
              {
                  // Handle as base64 source string
                  std::string sourceStr = Base64Decode(specifierStr);
                  maybeModule = static_cast<CV8ResourceImpl*>(resource)->ResolveCode(sourceStr, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
              }
              else if(typeValueStr == "source")
              {
                  // Handle as module source code
                  maybeModule = static_cast<CV8ResourceImpl*>(resource)->ResolveCode(specifierStr, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
              }
              else if(typeValueStr == "json")
              {
                  // Handle as JSON file

                  // Read the JSON file
                  auto path =
                    alt::ICore::Instance().Resolve(static_cast<CV8ResourceImpl*>(resource)->GetResource(), specifierStr, static_cast<CV8ResourceImpl*>(resource)->GetModulePath(referrer));
                  if(!path.pkg || !path.pkg->FileExists(path.fileName))
                  {
                      V8Helpers::Throw(isolate, "Invalid JSON file path");
                      return false;
                  }
                  alt::IPackage::File* file = path.pkg->OpenFile(path.fileName);
                  std::string src(path.pkg->GetFileSize(file), '\0');
                  path.pkg->ReadFile(file, src.data(), src.size());
                  path.pkg->CloseFile(file);

                  // Parse the JSON first to check if its valid
                  v8::MaybeLocal<v8::Value> result = v8::JSON::Parse(ctx, V8Helpers::JSValue(src));
                  if(result.IsEmpty())
                  {
                      V8Helpers::Throw(isolate, "Invalid JSON syntax");
                      return false;
                  }

                  maybeModule = static_cast<CV8ResourceImpl*>(resource)->CreateSyntheticModule((path.prefix + path.fileName), result.ToLocalChecked());
                  return true;
              }
              else
              {
                  V8Helpers::Throw(isolate, "Invalid import assertion type");
                  return false;
              }

              // Run the module
              if(maybeModule.IsEmpty())
              {
                  V8Helpers::Throw(isolate, "Failed to resolve module");
                  return false;
              }
              mod = maybeModule.ToLocalChecked();
              v8::Maybe<bool> result = mod->InstantiateModule(ctx, CV8ScriptRuntime::ResolveModule);
              if(result.IsNothing() || result.ToChecked() == false)
              {
                  V8Helpers::Throw(isolate, "Failed to instantiate module");
                  return false;
              }

              auto returnValue = mod->Evaluate(ctx);
              if(returnValue.IsEmpty())
              {
                  V8Helpers::Throw(isolate, "Failed to evaluate module");
                  return false;
              }

              return true;
          });
        if(!result) return v8::MaybeLocal<v8::Module>();
        else
            return v8::MaybeLocal<v8::Module>(mod);
    }

    return static_cast<CV8ResourceImpl*>(resource)->ResolveModule(_specifier, referrer, resource->GetResource());
}

void CV8ScriptRuntime::OnEntityStreamIn(alt::IEntity* entity)
{
    switch(entity->GetType())
    {
        case alt::IEntity::Type::PLAYER:
        {
            streamedInPlayers.insert({ entity->GetID(), dynamic_cast<alt::IPlayer*>(entity) });
            break;
        }
        case alt::IEntity::Type::VEHICLE:
        {
            streamedInVehicles.insert({ entity->GetID(), dynamic_cast<alt::IVehicle*>(entity) });
            break;
        }
    }
}

void CV8ScriptRuntime::OnEntityStreamOut(alt::IEntity* entity)
{
    switch(entity->GetType())
    {
        case alt::IEntity::Type::PLAYER:
        {
            streamedInPlayers.erase(entity->GetID());
            break;
        }
        case alt::IEntity::Type::VEHICLE:
        {
            streamedInVehicles.erase(entity->GetID());
            break;
        }
    }
}
