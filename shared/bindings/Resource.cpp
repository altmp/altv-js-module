#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

extern V8Class v8Resource;

static void IsStartedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN(resource->IsStarted());
}

static void TypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN_ALT_STRING(resource->GetType());
}

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN_ALT_STRING(resource->GetName());
}

static void MainGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN_ALT_STRING(resource->GetMain());
}

static void ExportsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN(V8Helpers::MValueToV8(resource->GetExports()));
}

static void DependenciesGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);

    const alt::Array<alt::StringView> deps = resource->GetDependencies();
    v8::Local<v8::Array> dependencies = v8::Array::New(isolate, deps.GetSize());
    for(size_t i = 0; i < deps.GetSize(); ++i)
    {
        dependencies->Set(ctx, i, V8Helpers::JSValue(deps[i]));
    }
    V8_RETURN(dependencies);
}

static void DependantsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);

    const alt::Array<alt::StringView> deps = resource->GetDependants();
    v8::Local<v8::Array> dependants = v8::Array::New(isolate, deps.GetSize());
    for(size_t i = 0; i < deps.GetSize(); ++i)
    {
        dependants->Set(ctx, i, V8Helpers::JSValue(deps[i]));
    }
    V8_RETURN(dependants);
}

static void RequiredPermissionsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);

    const alt::Array<alt::Permission> perms = resource->GetRequiredPermissions();
    v8::Local<v8::Array> permissions = v8::Array::New(isolate, perms.GetSize());
    for(size_t i = 0; i < perms.GetSize(); ++i)
    {
        permissions->Set(ctx, i, V8Helpers::JSValue((int)perms[i]));
    }
    V8_RETURN(permissions);
}

static void OptionalPermissionsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);

    const alt::Array<alt::Permission> perms = resource->GetOptionalPermissions();
    v8::Local<v8::Array> permissions = v8::Array::New(isolate, perms.GetSize());
    for(size_t i = 0; i < perms.GetSize(); ++i)
    {
        permissions->Set(ctx, i, V8Helpers::JSValue((int)perms[i]));
    }
    V8_RETURN(permissions);
}

#ifdef ALT_SERVER_API
static void PathGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, resource, alt::IResource);
    V8_RETURN_ALT_STRING(resource->GetPath());
}
#endif

// *** Static

// Helper
static inline v8::Local<v8::Object> CreateResource(v8::Local<v8::Context> ctx, alt::IResource* resource)
{
    v8::Local<v8::Object> obj = v8Resource.CreateInstance(ctx);
    obj->SetInternalField(0, v8::External::New(ctx->GetIsolate(), resource));
    return obj;
}

static void GetByName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STD_STRING(1, name);
    alt::IResource* resource = alt::ICore::Instance().GetResource(name);
    if(resource == nullptr) V8_RETURN_NULL();
    else
        V8_RETURN(CreateResource(ctx, resource));
}

static void AllGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    const std::vector<alt::IResource*> resources = alt::ICore::Instance().GetAllResources();
    size_t size = resources.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        alt::IResource* resource = resources[i];
        arr->Set(ctx, i, CreateResource(ctx, resource));
    }

    V8_RETURN(arr);
}

static void CurrentGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    alt::IResource* resource = V8ResourceImpl::GetResource(ctx);
    V8_RETURN(CreateResource(ctx, resource));
}

extern V8Class v8Resource("Resource", [](v8::Local<v8::FunctionTemplate> tpl) {
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
#ifdef ALT_SERVER_API
    V8Helpers::SetAccessor(isolate, tpl, "path", &PathGetter);
#endif

    V8Helpers::SetStaticMethod(isolate, tpl, "getByName", &GetByName);
    V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8Helpers::SetStaticAccessor(isolate, tpl, "current", &CurrentGetter);
});
