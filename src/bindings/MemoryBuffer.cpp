
#include "../CV8Resource.h"
#include "../helpers/V8Class.h"

//static void weakCallbackForObjectHolder(const v8::WeakCallbackInfo<v8::Object>& data) {
//	uint8_t* memory = (uint8_t*)data.GetInternalField(0);
//	delete data.GetParameter();
//}

//static void WeakCallback(v8::WeakCallbackData<v8::Object, int> data)
//{
//	Local<Object> val = data.GetValue();
//	int* ptr = retinterpret_cast<int*>(val->GetAlignedPointerFromINternalField(0));
//	delete ptr;
//	fprintf(stdout, "Deleted internal object!\n");
//}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	V8_CHECK(info.IsConstructCall(), "MemoryBuffer constructor is not a function");
	V8_CHECK(info.Length() == 1, "new MemoryBuffer(...) expects 1 arg");

    // Ask alt:V to add pattern searching to C++ SDK if you want this available
// 	if(info[0]->IsString())
// 	{
// #ifdef NDEBUG
// 		V8_CHECK(CGame::Instance().IsDebug() && IsDevOrInternalBranch(), "must be in debug mode and dev branch to use memory patterns");
// #endif
// 		V8_ARG_TO_STRING(0, str);
// 		auto mem = CMemory::Pattern(str.ToString()).Search(true);
// 		V8_CHECK(mem.IsValid(), "Pattern not found");
// 		info.This()->SetAlignedPointerInInternalField(0, mem.GetAddress<void*>());
// 		info.This()->SetInternalField(1, v8::Integer::NewFromUnsigned(isolate, UINT32_MAX));
// 	}
// 	else 
    {
		V8_CHECK(info[0]->IsNumber(), "size must be a number or a string pattern");
		uint32_t size = info[0]->Uint32Value(ctx).ToChecked();
		if(size == 0)
		{
			info.This()->SetAlignedPointerInInternalField(0, nullptr);
			info.This()->SetInternalField(1, v8::Integer::NewFromUnsigned(isolate, 0));
			return;
		}
		V8_CHECK(size <= 1024, "You can't allocate > 1KB");

		uint8_t* allocatedMemory = new uint8_t[size];
		memset(allocatedMemory, 0, size);
		info.This()->SetAlignedPointerInInternalField(0, allocatedMemory);
		info.This()->SetInternalField(1, v8::Integer::NewFromUnsigned(isolate, size));
	}

	/*v8::UniquePersistent<v8::Object> persistent(isolate, info.This());
	persistent.SetWeak(info.This(), weakCallbackForObjectHolder, v8::WeakCallbackType::kInternalFields);*/
}

static void FreeBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	uint8_t* memory = (uint8_t*)info.This()->GetAlignedPointerFromInternalField(0);
	if (memory != nullptr)
	{
		delete memory;
		info.This()->SetAlignedPointerInInternalField(0, nullptr);
		info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
		return;
	}
	info.GetReturnValue().Set(v8::Boolean::New(isolate, false));
}

static void GetAddress(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

#ifdef NDEBUG
	V8_CHECK(CGame::Instance().IsDebug() && IsDevOrInternalBranch(), "must be in debug mode and dev branch to get the address of memory buffers");
#endif

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	uintptr_t memory = (uintptr_t)info.This()->GetAlignedPointerFromInternalField(0);
	info.GetReturnValue().Set(v8::BigInt::New(isolate, memory));
}

template <typename T>
static void GetDataOfType(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	auto ctx = isolate->GetEnteredContext();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	bool isString = false;
	if (std::is_same_v<T, std::string>)
	{
		V8_CHECK(info.Length() == 2, "2 args expected");
		isString = true;
	}
	else
	{
		V8_CHECK(info.Length() == 1, "1 arg expected");
	}

	V8_CHECK(info[0]->IsNumber(), "offset must be a number");
	uint32_t offset = info[0]->Uint32Value(ctx).ToChecked();

	uint32_t strLength = 0;

	if (isString)
		strLength = info[1]->Uint32Value(ctx).ToChecked();

	uint8_t* memory = (uint8_t*)info.This()->GetAlignedPointerFromInternalField(0);
	uint16_t size = info.This()->GetInternalField(1)->Uint32Value(isolate->GetEnteredContext()).ToChecked();
	if (memory == nullptr || size == 0)
	{
		info.GetReturnValue().Set(v8::Null(isolate));
		return;
	}

#ifdef NDEBUG
	if(!(CGame::Instance().IsDebug() && std::string(ALTV_BRANCH)=="dev"))
#endif
	{
		if (isString)
		{
			V8_CHECK((offset + strLength) <= size, "Offset is out of bounds");
		}
		else
		{
			V8_CHECK((offset + sizeof(T)) <= size, "Offset is out of bounds");
		}
	}

	if (!isString)
	{
		if (std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>)
		{
			info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, *(uint32_t*)((uintptr_t)memory + offset)));
			return;
		}
		else if (std::is_same_v<T, uint64_t>)
		{
			info.GetReturnValue().Set(v8::BigInt::NewFromUnsigned(isolate, *(uint64_t*)((uintptr_t)memory + offset)));
			return;
		}
		else if (std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, int32_t>)
		{
			info.GetReturnValue().Set(v8::Integer::New(isolate, *(int32_t*)((uintptr_t)memory + offset)));
			return;
		}
		else if (std::is_same_v<T, int64_t>)
		{
			info.GetReturnValue().Set(v8::BigInt::New(isolate, *(int64_t*)((uintptr_t)memory + offset)));
			return;
		}
		else if (std::is_same_v<T, float> || std::is_same_v<T, double>)
		{
			info.GetReturnValue().Set(v8::Number::New(isolate, *(double*)((uintptr_t)memory + offset)));
			return;
		}
	}
	else
	{
		char* newString = new char[strLength + 1];
		memcpy_s(newString, strLength + 1, (void*)((uintptr_t)memory + offset), strLength);
		newString[strLength] = 0;
		info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, newString).ToLocalChecked());
		delete newString;
	}
}

extern V8Class v8MemoryBuffer("MemoryBuffer", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

	tpl->InstanceTemplate()->SetInternalFieldCount(2);

	proto->Set(isolate, "free", v8::FunctionTemplate::New(isolate, &FreeBuffer));
	proto->Set(isolate, "address", v8::FunctionTemplate::New(isolate, &GetAddress));
	proto->Set(isolate, "ubyte", v8::FunctionTemplate::New(isolate, &GetDataOfType<uint8_t>));
	proto->Set(isolate, "ushort", v8::FunctionTemplate::New(isolate, &GetDataOfType<uint16_t>));
	proto->Set(isolate, "uint", v8::FunctionTemplate::New(isolate, &GetDataOfType<uint32_t>));
	proto->Set(isolate, "ulong", v8::FunctionTemplate::New(isolate, &GetDataOfType<uint64_t>));
	proto->Set(isolate, "byte", v8::FunctionTemplate::New(isolate, &GetDataOfType<int8_t>));
	proto->Set(isolate, "short", v8::FunctionTemplate::New(isolate, &GetDataOfType<int16_t>));
	proto->Set(isolate, "int", v8::FunctionTemplate::New(isolate, &GetDataOfType<int32_t>));
	proto->Set(isolate, "long", v8::FunctionTemplate::New(isolate, &GetDataOfType<int64_t>));
	proto->Set(isolate, "float", v8::FunctionTemplate::New(isolate, &GetDataOfType<float>));
	proto->Set(isolate, "double", v8::FunctionTemplate::New(isolate, &GetDataOfType<double>));
	proto->Set(isolate, "string", v8::FunctionTemplate::New(isolate, &GetDataOfType<std::string>));
});
