#include "stdafx.h"

#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"

static void StaticExists(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "fileName must be a string");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	std::string _path = *v8::String::Utf8Value(isolate, info[0].As<v8::String>());

#ifdef ALT_CLIENT
	V8ResourceImpl::PathInfo path = resource->Resolve(_path, "");
	V8_CHECK(path.pkg, "invalid asset pack");
	bool exists = path.pkg->FileExists(path.fileName);
#else
	bool exists = alt::ICore::Instance().FileExists(_path);
#endif

	info.GetReturnValue().Set(v8::Boolean::New(isolate, exists));
}

static void StaticRead(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8_CHECK(info.Length() >= 1, "at least 1 arg expected");
	V8_CHECK(info[0]->IsString(), "fileName must be a string");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	std::string name = *v8::String::Utf8Value(isolate, info[0].As<v8::String>());

	std::string encoding = "utf-8";

	if (info.Length() >= 2)
	{
		V8_CHECK(info[1]->IsString(), "encoding must be a string");
		encoding = *v8::String::Utf8Value(isolate, info[1].As<v8::String>());
	}

#ifdef ALT_CLIENT
	V8ResourceImpl::PathInfo path = resource->Resolve(name, "");
	V8_CHECK(path.pkg, "invalid asset pack");

	alt::IPackage::File* file = path.pkg->OpenFile(path.fileName);
	V8_CHECK(file, "file does not exist");

	alt::String data(path.pkg->GetFileSize(file));
	path.pkg->ReadFile(file, data.GetData(), data.GetSize());
	path.pkg->CloseFile(file);
#else
	alt::String data = alt::ICore::Instance().FileRead(name);
#endif // ALT_CLIENT

	if (encoding == "utf-8")
	{
		info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, data.GetData(), v8::NewStringType::kNormal, data.GetSize()).ToLocalChecked());
	}
	else if (encoding == "utf-16")
	{
		info.GetReturnValue().Set(v8::String::NewFromTwoByte(isolate, (uint16_t*)data.GetData(), v8::NewStringType::kNormal, data.GetSize() / 2).ToLocalChecked());
	}
	else if (encoding == "binary")
	{
		v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(isolate, data.GetSize());
		v8::ArrayBuffer::Contents contents = buffer->GetContents();

		std::memcpy(contents.Data(), data.GetData(), data.GetSize());

		info.GetReturnValue().Set(buffer);
	}
}

static V8Class v8File("File", "", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetStaticMethod(isolate, tpl, "exists", StaticExists);
	V8::SetStaticMethod(isolate, tpl, "read", StaticRead);
});
