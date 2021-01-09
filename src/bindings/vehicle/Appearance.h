#pragma once

namespace V8::Vehicle
{
	void ModKitGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE();
		V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
		V8_RETURN_INTEGER(_this->GetModKit());
	}

	void ModKitSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
		V8_TO_INTEGER(value, _val);
		V8_CHECK(_this->SetModKit(_val), "wrong modkit");
	}

	void NeonActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

		bool left, right, front, back;
		_this->GetNeonActive(&left, &right, &front, &back);

		v8::Local<v8::Object> neonActive = v8::Object::New(isolate);
		V8::DefineOwnProperty(isolate, ctx, neonActive, "left", v8::Boolean::New(isolate, left));
		V8::DefineOwnProperty(isolate, ctx, neonActive, "right", v8::Boolean::New(isolate, right));
		V8::DefineOwnProperty(isolate, ctx, neonActive, "front", v8::Boolean::New(isolate, front));
		V8::DefineOwnProperty(isolate, ctx, neonActive, "back", v8::Boolean::New(isolate, back));

		V8_RETURN(neonActive);
	}

	void NeonActiveSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

		V8_TO_OBJECT(value, neonActive);
		V8_OBJECT_GET_BOOLEAN(neonActive, "left", left);
		V8_OBJECT_GET_BOOLEAN(neonActive, "right", right);
		V8_OBJECT_GET_BOOLEAN(neonActive, "front", front);
		V8_OBJECT_GET_BOOLEAN(neonActive, "back", back);

		_this->SetNeonActive(left, right, front, back);
	}

	/*void SetWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		uint8_t wheelType = info[0]->ToInteger(isolate)->Value();
		uint8_t wheelVariation = info[1]->ToInteger(isolate)->Value();

		vehicle->SetWheels(wheelType, wheelVariation);
	}

	void SetRearWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		uint8_t wheelVariation = info[0]->ToInteger(isolate)->Value();

		vehicle->SetRearWheels(wheelVariation);
	}

	void GetModsCount(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> category = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetModsCount(category->Value())));
	}

	void SetMod(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> category = info[0]->ToInteger(isolate);
		v8::Local<v8::Integer> id = info[1]->ToInteger(isolate);

		if (!vehicle->SetMod(category->Value(), id->Value()))
		{
			if (vehicle->GetModKit() == 0)
				V8Helpers::Throw(isolate, "modkit is not set");
			else
				V8Helpers::Throw(isolate, "mod doesn't exist");
		}
	}

	void GetMod(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> category = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetMod(category->Value())));
	}

	void SetExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> category = info[0]->ToInteger(isolate);
		v8::Local<v8::Boolean> state = info[1]->ToBoolean(isolate);

		vehicle->ToggleExtra(category->Value(), state->Value());
	}

	void GetExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> category = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsExtraOn(category->Value())));
	}

	void SetAppearanceData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::String::Utf8Value appearanceData(isolate, info[0]->ToString(isolate));

		vehicle->LoadAppearanceDataFromBase64(*appearanceData);
	}

	void GetAppearanceData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), vehicle->GetAppearanceDataBase64().CStr()));
	}*/
}
