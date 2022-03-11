#include "stdafx.h"

#include "CNodeResourceImpl.h"
#include "CNodeScriptRuntime.h"
#include "V8Module.h"
#include "V8Helpers.h"

static void ResourceLoaded(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, name);

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);
    if(resource && resource->GetType() == "js")
    {
        CNodeResourceImpl* _resource = static_cast<CNodeResourceImpl*>(resource->GetImpl());
        _resource->Started(info[1]);
    }
}

static const char bootstrap_code[] = R"(
'use strict';

(async () => {
  const alt = process._linkedBinding('alt');
  const path = require('path');
  const asyncESM = require('internal/process/esm_loader');
  const url = require('internal/url');
  const { pathToFileURL } = url;
  const { translators } = require('internal/modules/esm/translators');

  let _exports = null;
  const loader = asyncESM.esmLoader;

  try {
    // Supress the annoying warning from NodeJS
    const __emitWarning = process.emitWarning;
    process.emitWarning = () => {};
    const { ModuleWrap } = require('internal/test/binding').internalBinding('module_wrap');
    process.emitWarning = __emitWarning;

    // Set our custom translator for the 'alt' protocol that loads alt:V resources
    translators.set('alt', async function(url) {
      const name = url.slice(4); // Remove 'alt:' scheme
      const exports = process._linkedBinding('alt').getResourceExports(name);
      return new ModuleWrap(url, undefined, Object.keys(exports), function() {
        for (const exportName in exports) {
          // We might trigger a getter -> dont fail.
          let value;
          try {
            value = exports[exportName];
          } catch {}
          this.setExport(exportName, value);
        }
      });
    });

    loader.addCustomLoaders({
        resolve(specifier, context, defaultResolve) {
            if (alt.hasResource(specifier)) return {
                url: 'alt:' + specifier
            };
            return defaultResolve(specifier, context, defaultResolve);
        },
        load(url, context, defaultLoad) {
            if(url.startsWith('alt:')) {
                const name = url.slice(4); // Remove 'alt:' scheme
                return {
                    format: 'alt',
                    source: null,
                }
            }
            return defaultLoad(url, context, defaultLoad);
        },
    });

    // Get the path to the main file for this resource, and load it
    const _path = path.resolve(alt.getResourcePath(alt.resourceName), alt.getResourceMain(alt.resourceName));
    _exports = await loader.import("file://" + _path, "", {});
    if ('start' in _exports) {
      const start = _exports.start;
      if (typeof start === 'function') {
        await start();
      }
    }
  } catch (e) {
    console.error(e);
  }

  __resourceLoaded(alt.resourceName, _exports);
})();
)";

bool CNodeResourceImpl::Start()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    v8::Local<v8::String> resourceName = V8Helpers::JSValue(resource->GetName());

    v8::Local<v8::Context> _context = node::NewContext(isolate, global);
    v8::Context::Scope scope(_context);

    _context->Global()->Set(_context, V8Helpers::JSValue("__resourceLoaded"), v8::Function::New(_context, &ResourceLoaded).ToLocalChecked());

    _context->SetAlignedPointerInEmbedderData(1, resource);
    context.Reset(isolate, _context);

    V8ResourceImpl::Start();

    node::EnvironmentFlags::Flags flags = (node::EnvironmentFlags::Flags)(node::EnvironmentFlags::kOwnsProcessState & node::EnvironmentFlags::kNoCreateInspector);

    uvLoop = uv_loop_new();

    nodeData = node::CreateIsolateData(isolate, uvLoop, runtime->GetPlatform());
    std::vector<std::string> argv = { "altv-resource" };
    env = node::CreateEnvironment(nodeData, _context, argv, argv, flags);

    node::IsolateSettings is;
    node::SetIsolateUpForNode(isolate, is);

    node::LoadEnvironment(env, bootstrap_code);

    asyncResource.Reset(isolate, v8::Object::New(isolate));
    asyncContext = node::EmitAsyncInit(isolate, asyncResource.Get(isolate), "CNodeResourceImpl");

    while(!envStarted && !startError)
    {
        runtime->OnTick();
        OnTick();
    }

    DispatchStartEvent(startError);

    return !startError;
}

bool CNodeResourceImpl::Stop()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    {
        v8::Context::Scope scope(GetContext());
        DispatchStopEvent();

        node::EmitAsyncDestroy(isolate, asyncContext);
        asyncResource.Reset();
    }

    node::EmitBeforeExit(env);
    node::EmitExit(env);
    node::RunAtExit(env);

    // TODO: async stop function

    // node::Stop(env);

    node::FreeEnvironment(env);
    node::FreeIsolateData(nodeData);

    envStarted = false;

    return true;
}

void CNodeResourceImpl::Started(v8::Local<v8::Value> _exports)
{
    if(!_exports->IsNullOrUndefined())
    {
        alt::MValueDict exports = V8Helpers::V8ToMValue(_exports).As<alt::IMValueDict>();
        resource->SetExports(exports);
        envStarted = true;
    }
    else
    {
        startError = true;
    }
}

bool CNodeResourceImpl::OnEvent(const alt::CEvent* e)
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());
    // env->PushAsyncCallbackScope();

    V8Helpers::EventHandler* handler = V8Helpers::EventHandler::Get(e);
    if(!handler) return true;

    // Generic event handler
    {
        auto evType = e->GetType();
        if(evType == alt::CEvent::Type::CLIENT_SCRIPT_EVENT || evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
        {
            std::vector<V8Helpers::EventCallback*> callbacks;
            const char* eventName;

            if(evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(true));
                eventName = static_cast<const alt::CServerScriptEvent*>(e)->GetName().CStr();
            }
            else if(evType == alt::CEvent::Type::CLIENT_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(false));
                eventName = static_cast<const alt::CClientScriptEvent*>(e)->GetName().CStr();
            }

            if(callbacks.size() != 0)
            {
                auto evArgs = handler->GetArgs(this, e);
                evArgs.insert(evArgs.begin(), V8Helpers::JSValue(eventName));

                node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);
                InvokeEventHandlers(e, callbacks, evArgs);
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> callbacks = handler->GetCallbacks(this, e);
    if(callbacks.size() > 0)
    {
        std::vector<v8::Local<v8::Value>> args = handler->GetArgs(this, e);

        node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);
        InvokeEventHandlers(e, callbacks, args);
    }

    // env->PopAsyncCallbackScope();
    return true;
}

void CNodeResourceImpl::OnTick()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());
    node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);

    uv_run(uvLoop, UV_RUN_NOWAIT);
    V8ResourceImpl::OnTick();
}

bool CNodeResourceImpl::MakeClient(alt::IResource::CreationInfo* info, alt::Array<alt::String>)
{
    if(resource->GetClientType() == "jsb") info->type = "js";
    return true;
}
