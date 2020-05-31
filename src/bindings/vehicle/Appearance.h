#pragma once

namespace V8::Vehicle
{
	void ModKitsCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetModKitsCount()));
	}

	void ModKitGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetModKit()));
	}

	void ModKitSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		V8_CHECK(vehicle->SetModKit(value->ToInteger(info.GetIsolate())->Value()), "wrong modkit");
	}

	void PrimaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetPrimaryColor()));
	}

	void PrimaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetPrimaryColor(value->ToInteger(info.GetIsolate())->Value());
	}

	void SecondaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetSecondaryColor()));
	}

	void SecondaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetSecondaryColor(value->ToInteger(info.GetIsolate())->Value());
	}

	void CustomPrimaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		alt::RGBA color = vehicle->GetPrimaryColorRGB();
		info.GetReturnValue().Set(resource->CreateRGBA(color));
	}

	void CustomPrimaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		V8_CHECK(value->IsObject(), "object expected");

		v8::Local<v8::Object> color = value.As<v8::Object>();

		v8::Local<v8::Integer> r = V8::Get(ctx, color, "r")->ToInteger(isolate);
		v8::Local<v8::Integer> g = V8::Get(ctx, color, "g")->ToInteger(isolate);
		v8::Local<v8::Integer> b = V8::Get(ctx, color, "b")->ToInteger(isolate);

		vehicle->SetPrimaryColorRGB({ (uint8_t)r->Value(), (uint8_t)g->Value(), (uint8_t)b->Value(), 255 });
	}

	void CustomSecondaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		V8_CHECK(value->IsObject(), "object expected");

		v8::Local<v8::Object> color = value.As<v8::Object>();

		v8::Local<v8::Integer> r = V8::Get(ctx, color, "r")->ToInteger(isolate);
		v8::Local<v8::Integer> g = V8::Get(ctx, color, "g")->ToInteger(isolate);
		v8::Local<v8::Integer> b = V8::Get(ctx, color, "b")->ToInteger(isolate);

		vehicle->SetSecondaryColorRGB({ (uint8_t)r->Value(), (uint8_t)g->Value(), (uint8_t)b->Value(), 255 });
	}

	void CustomSecondaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		alt::RGBA color = vehicle->GetSecondaryColorRGB();
		info.GetReturnValue().Set(resource->CreateRGBA(color));
	}

	void TireSmokeColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		alt::RGBA color = vehicle->GetTireSmokeColor();
		info.GetReturnValue().Set(resource->CreateRGBA(color));
	}

	void TireSmokeColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		V8_CHECK(value->IsObject(), "object expected");

		v8::Local<v8::Object> color = value.As<v8::Object>();

		v8::Local<v8::Integer> r = V8::Get(ctx, color, "r")->ToInteger(isolate);
		v8::Local<v8::Integer> g = V8::Get(ctx, color, "g")->ToInteger(isolate);
		v8::Local<v8::Integer> b = V8::Get(ctx, color, "b")->ToInteger(isolate);

		vehicle->SetTireSmokeColor({ (uint8_t)r->Value(), (uint8_t)g->Value(), (uint8_t)b->Value(), 255 });
	}

	void NeonColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		alt::RGBA color = vehicle->GetNeonColor();
		info.GetReturnValue().Set(resource->CreateRGBA(color));
	}

	void NeonColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		V8_CHECK(value->IsObject(), "object expected");

		v8::Local<v8::Object> color = value.As<v8::Object>();

		v8::Local<v8::Integer> r = V8::Get(ctx, color, "r")->ToInteger(isolate);
		v8::Local<v8::Integer> g = V8::Get(ctx, color, "g")->ToInteger(isolate);
		v8::Local<v8::Integer> b = V8::Get(ctx, color, "b")->ToInteger(isolate);

		vehicle->SetNeonColor({ (uint8_t)r->Value(), (uint8_t)g->Value(), (uint8_t)b->Value(), 255 });
	}

	void PearlColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetPearlColor()));
	}

	void PearlColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetPearlColor(value->ToInteger(info.GetIsolate())->Value());
	}

	void WheelColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetWheelColor()));
	}

	void WheelColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWheelColor(value->ToInteger(info.GetIsolate())->Value());
	}

	void InteriorColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetInteriorColor()));
	}

	void InteriorColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetInteriorColor(value->ToInteger(info.GetIsolate())->Value());
	}

	void DashboardColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetDashboardColor()));
	}

	void DashboardColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetDashboardColor(value->ToInteger(isolate)->Value());
	}

	void CustomTiresGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->GetCustomTires()));
	}

	void CustomTiresSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetCustomTires(value->ToBoolean(isolate)->Value());
	}

	void DarknessGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetSpecialDarkness()));
	}

	void DarknessSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetSpecialDarkness(value->ToInteger(isolate)->Value());
	}

	void WindowTintGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetWindowTint()));
	}

	void WindowTintSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWindowTint(value->ToInteger(isolate)->Value());
	}

	void NeonActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Object> neonActive = v8::Object::New(isolate);

		bool left, right, front, back;
		vehicle->GetNeonActive(&left, &right, &front, &back);

		neonActive->Set(v8::String::NewFromUtf8(isolate, "left"), v8::Boolean::New(isolate, left));
		neonActive->Set(v8::String::NewFromUtf8(isolate, "right"), v8::Boolean::New(isolate, right));
		neonActive->Set(v8::String::NewFromUtf8(isolate, "front"), v8::Boolean::New(isolate, front));
		neonActive->Set(v8::String::NewFromUtf8(isolate, "back"), v8::Boolean::New(isolate, back));

		info.GetReturnValue().Set(neonActive);
	}

	void NeonActiveSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		V8_CHECK(value->IsObject(), "object expected");

		v8::Local<v8::Object> neonActive = value.As<v8::Object>();

		v8::Local<v8::Boolean> left = V8::Get(ctx, neonActive, "left")->ToBoolean(isolate);
		v8::Local<v8::Boolean> right = V8::Get(ctx, neonActive, "right")->ToBoolean(isolate);
		v8::Local<v8::Boolean> front = V8::Get(ctx, neonActive, "front")->ToBoolean(isolate);
		v8::Local<v8::Boolean> back = V8::Get(ctx, neonActive, "back")->ToBoolean(isolate);

		vehicle->SetNeonActive(left->Value(), right->Value(), front->Value(), back->Value());
	}

	void DirtLevelGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetDirtLevel()));
	}

	void DirtLevelSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetDirtLevel(value->ToInteger(info.GetIsolate())->Value());
	}

	void NumberPlateIndexGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetNumberplateIndex()));
	}

	void NumberPlateIndexSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetNumberplateIndex(value->ToInteger(info.GetIsolate())->Value());
	}

	void NumberPlateTextGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), vehicle->GetNumberplateText().CStr()));
	}

	void NumberPlateTextSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::String::Utf8Value plateText(isolate, value->ToString(isolate));

		vehicle->SetNumberplateText(*plateText);
	}

	void LiveryGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetLivery()));
	}

	void LiverySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetLivery(value->ToInteger(info.GetIsolate())->Value());
	}

	void RoofLiveryGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetRoofLivery()));
	}

	void RoofLiverySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetRoofLivery(value->ToInteger(info.GetIsolate())->Value());
	}

	void WheelTypeGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetWheelType()));
	}

	void FrontWheelsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetWheelVariation()));
	}

	void RearWheelsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetRearWheelVariation()));
	}

	void SetWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
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
	}
}
