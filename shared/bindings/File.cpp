
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"

static void StaticExists(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, _path);

#ifdef ALT_CLIENT
    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    auto path = alt::ICore::Instance().Resolve(resource, _path, origin);
    V8_CHECK(path.pkg, "invalid asset pack");
    bool exists = path.pkg->FileExists(path.fileName);
#else
    bool exists = alt::ICore::Instance().FileExists(_path);
#endif

    V8_RETURN_BOOLEAN(exists);
}

// todo: refactor this
static void StaticRead(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_IRESOURCE();

    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, name);

    std::string encoding = "utf-8";

    if(info.Length() >= 2)
    {
        V8_ARG_TO_STD_STRING(2, _encoding);
        encoding = _encoding;
    }

#ifdef ALT_CLIENT
    alt::String origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    auto path = alt::ICore::Instance().Resolve(resource, name, origin);
    V8_CHECK(path.pkg, "invalid asset pack");

    alt::IPackage::File* file = path.pkg->OpenFile(path.fileName);
    V8_CHECK(file, "file does not exist");

    alt::String data(path.pkg->GetFileSize(file));
    path.pkg->ReadFile(file, data.GetData(), data.GetSize());
    path.pkg->CloseFile(file);
#else
    alt::String data = alt::ICore::Instance().FileRead(name);
#endif  // ALT_CLIENT

    if(encoding == "utf-8")
    {
        V8_RETURN(V8Helpers::JSValue(data));
    }
    else if(encoding == "utf-16")
    {
        V8_RETURN(V8Helpers::JSValue((uint16_t*)data.GetData()));
    }
    else if(encoding == "binary")
    {
        v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(isolate, data.GetSize());
        auto contents = buffer->GetBackingStore();

        std::memcpy(contents->Data(), data.GetData(), data.GetSize());

        V8_RETURN(buffer);
    }
}

extern V8Class v8File("File", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticMethod(isolate, tpl, "exists", StaticExists);
    V8Helpers::SetStaticMethod(isolate, tpl, "read", StaticRead);
});
