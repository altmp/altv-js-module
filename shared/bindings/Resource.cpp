#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

extern V8Class v8Resource;

static void IsStartedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");
    V8_RETURN(resource->IsStarted());
}

static void TypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");
    V8_RETURN_STRING(resource->GetType());
}

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");
    V8_RETURN_STRING(resource->GetName());
}

static void MainGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");
    V8_RETURN_STRING(resource->GetMain());
}

static void ExportsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");
    V8_RETURN(V8Helpers::MValueToV8(resource->GetExports()));
}

static void DependenciesGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");

    const std::vector<std::string> deps = resource->GetDependencies();
    v8::Local<v8::Array> dependencies = v8::Array::New(isolate, deps.size());
    for(size_t i = 0; i < deps.size(); ++i)
    {
        dependencies->Set(ctx, i, V8Helpers::JSValue(deps[i]));
    }
    V8_RETURN(dependencies);
}

static void DependantsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");

    const std::vector<std::string> deps = resource->GetDependants();
    v8::Local<v8::Array> dependants = v8::Array::New(isolate, deps.size());
    for(size_t i = 0; i < deps.size(); ++i)
    {
        dependants->Set(ctx, i, V8Helpers::JSValue(deps[i]));
    }
    V8_RETURN(dependants);
}

static void RequiredPermissionsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");

    const std::vector<alt::Permission> perms = resource->GetRequiredPermissions();
    v8::Local<v8::Array> permissions = v8::Array::New(isolate, perms.size());
    for(size_t i = 0; i < perms.size(); ++i)
    {
        permissions->Set(ctx, i, V8Helpers::JSValue((int)perms[i]));
    }
    V8_RETURN(permissions);
}

static void OptionalPermissionsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");

    const std::vector<alt::Permission> perms = resource->GetOptionalPermissions();
    v8::Local<v8::Array> permissions = v8::Array::New(isolate, perms.size());
    for(size_t i = 0; i < perms.size(); ++i)
    {
        permissions->Set(ctx, i, V8Helpers::JSValue((int)perms[i]));
    }
    V8_RETURN(permissions);
}

static void ValidGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN_BOOLEAN(resource != nullptr);
}

#ifdef ALT_SERVER_API
static void PathGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");
    V8_RETURN_STRING(resource->GetPath());
}
#endif

static void ConfigGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_CHECK(resource, "Invalid resource");

    auto config = resource->GetConfig();
    v8::Local<v8::Value> val = V8Helpers::ConfigNodeToV8(config);
    V8_CHECK(!val.IsEmpty(), "Failed to convert config to V8 value");
    V8_RETURN(val);
}

// *** Static
static void GetByName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    alt::IResource* res = alt::ICore::Instance().GetResource(name);
    if(resource == nullptr) V8_RETURN_NULL();
    else
        V8_RETURN(resource->GetOrCreateResourceObject(res));
}

static void AllGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    const std::vector<alt::IResource*> resources = alt::ICore::Instance().GetAllResources();
    size_t size = resources.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        alt::IResource* res = resources[i];
        arr->Set(ctx, i, resource->GetOrCreateResourceObject(res));
    }

    V8_RETURN(arr);
}

static void CurrentGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetOrCreateResourceObject(resource->GetResource()));
}

extern V8Class v8Resource("Resource",
                          [](v8::Local<v8::FunctionTemplate> tpl)
                          {
                              v8::Isolate* isolate = v8::Isolate::GetCurrent();

                              tpl->InstanceTemplate()->SetInternalFieldCount(2);  // Set it to 2, so that our check for V8 entities works (as this isn't a base object)

    V8Helpers::SetAccessor(isolate, tpl, "isStarted", &IsStartedGetter);
    V8Helpers::SetAccessor(isolate, tpl, "type", &TypeGetter);
    V8Helpers::SetAccessor(isolate, tpl, "name", &NameGetter);
    V8Helpers::SetAccessor(isolate, tpl, "main", &MainGetter);
    V8Helpers::SetAccessor(isolate, tpl, "exports", &ExportsGetter);
    V8Helpers::SetAccessor(isolate, tpl, "dependencies", &DependenciesGetter);
    V8Helpers::SetAccessor(isolate, tpl, "dependants", &DependantsGetter);
    V8Helpers::SetAccessor(isolate, tpl, "requiredPermissions", &RequiredPermissionsGetter);
    V8Helpers::SetAccessor(isolate, tpl, "optionalPermissions", &OptionalPermissionsGetter);
    V8Helpers::SetAccessor(isolate, tpl, "valid", &ValidGetter);
#ifdef ALT_SERVER_API
                              V8Helpers::SetAccessor(isolate, tpl, "path", &PathGetter);
#endif
                              V8Helpers::SetAccessor(isolate, tpl, "config", &ConfigGetter);

                              V8Helpers::SetStaticMethod(isolate, tpl, "getByName", &GetByName);
                              V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                              V8Helpers::SetStaticAccessor(isolate, tpl, "current", &CurrentGetter);
                          });
