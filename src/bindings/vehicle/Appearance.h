#pragma once

namespace V8::Vehicle
{
	void ModKitsCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_INTEGER(vehicle->GetModKitsCount());
	}

	void ModKitGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_INTEGER(vehicle->GetModKit());
	}

	void ModKitSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, modkit);

		V8_CHECK(vehicle->SetModKit(modkit), "Invalid modkit");
	}

	void PrimaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_INTEGER(vehicle->GetPrimaryColor());
	}

	void PrimaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetPrimaryColor(color);
	}

	void SecondaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_INTEGER(vehicle->GetSecondaryColor());
	}

	void SecondaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetSecondaryColor(color);
	}

	void CustomPrimaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT_RESOURCE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN(resource->CreateRGBA(vehicle->GetPrimaryColorRGB()));
	}

	void CustomPrimaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_OBJECT(value, color);
		V8_OBJECT_GET_INTEGER(color, "r", r);
		V8_OBJECT_GET_INTEGER(color, "g", g);
		V8_OBJECT_GET_INTEGER(color, "b", b);
		
		vehicle->SetPrimaryColorRGB({ r, g, b, 255 });
	}

	void CustomSecondaryColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_OBJECT(value, color);
		V8_OBJECT_GET_INTEGER(color, "r", r);
		V8_OBJECT_GET_INTEGER(color, "g", g);
		V8_OBJECT_GET_INTEGER(color, "b", b);
		
		vehicle->SetSecondaryColorRGB({ r, g, b, 255 });
	}

	void CustomSecondaryColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT_RESOURCE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN(resource->CreateRGBA(vehicle->GetSecondaryColorRGB()));
	}

	void TireSmokeColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT_RESOURCE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN(resource->CreateRGBA(vehicle->GetTireSmokeColor()));
	}

	void TireSmokeColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_OBJECT(value, color);
		V8_OBJECT_GET_INTEGER(color, "r", r);
		V8_OBJECT_GET_INTEGER(color, "g", g);
		V8_OBJECT_GET_INTEGER(color, "b", b);
		
		vehicle->SetTireSmokeColor({ r, g, b, 255 });
	}

	void NeonColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT_RESOURCE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN(resource->CreateRGBA(vehicle->GetNeonColor()));
	}

	void NeonColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_OBJECT(value, color);
		V8_OBJECT_GET_INTEGER(color, "r", r);
		V8_OBJECT_GET_INTEGER(color, "g", g);
		V8_OBJECT_GET_INTEGER(color, "b", b);
		
		vehicle->SetNeonColor({ r, g, b, 255 });
	}

	void PearlColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetPearlColor());
	}

	void PearlColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetPearlColor(color);
	}

	void WheelColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetWheelColor());
	}

	void WheelColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetWheelColor(color);
	}

	void InteriorColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetInteriorColor());
	}

	void InteriorColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetInteriorColor(color);
	}

	void DashboardColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetDashboardColor());
	}

	void DashboardColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetDashboardColor(color);
	}

	void CustomTiresGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->GetCustomTires());
	}

	void CustomTiresSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_BOOLEAN(value, state);

		vehicle->SetCustomTires(state);
	}

	void DarknessGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetSpecialDarkness());
	}

	void DarknessSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, darkness);

		vehicle->SetSpecialDarkness(darkness);
	}

	void WindowTintGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetWindowTint());
	}

	void WindowTintSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, tint);

		vehicle->SetWindowTint(tint);
	}

	void NeonActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		bool left, right, front, back;
		vehicle->GetNeonActive(&left, &right, &front, &back);

		V8_NEW_OBJECT(neonActive);
		V8_OBJECT_SET_BOOLEAN(neonActive, "left", left);
		V8_OBJECT_SET_BOOLEAN(neonActive, "right", right);
		V8_OBJECT_SET_BOOLEAN(neonActive, "front", front);
		V8_OBJECT_SET_BOOLEAN(neonActive, "back", back);

		V8_RETURN(neonActive);
	}

	void NeonActiveSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_OBJECT(value, neonActive);
		V8_OBJECT_GET_BOOLEAN(neonActive, "left", left);
		V8_OBJECT_GET_BOOLEAN(neonActive, "right", right);
		V8_OBJECT_GET_BOOLEAN(neonActive, "front", front);
		V8_OBJECT_GET_BOOLEAN(neonActive, "back", back);

		vehicle->SetNeonActive(left, right, front, back);
	}

	void DirtLevelGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetDirtLevel());
	}

	void DirtLevelSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, level);

		vehicle->SetDirtLevel(level);
	}

	void NumberPlateIndexGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetNumberplateIndex());
	}

	void NumberPlateIndexSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, index);

		vehicle->SetNumberplateIndex(index);
	}

	void NumberPlateTextGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_STRING(vehicle->GetNumberplateText().CStr());
	}

	void NumberPlateTextSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_STRING(value, text);

		vehicle->SetNumberplateText(text);
	}

	void LiveryGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetLivery());
	}

	void LiverySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, livery);

		vehicle->SetLivery(livery);
	}

	void RoofLiveryGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetRoofLivery());
	}

	void RoofLiverySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, livery);

		vehicle->SetRoofLivery(livery);
	}

	void WheelTypeGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetWheelType());
	}

	void FrontWheelsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetWheelVariation());
	}

	void RearWheelsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetRearWheelVariation());
	}

	void SetWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INTEGER(1, type);
		V8_ARG_TO_INTEGER(2, variation);

		vehicle->SetWheels(type, variation);
	}

	void SetRearWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INTEGER(1, variation);

		vehicle->SetRearWheels(variation);
	}

	void GetModsCount(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INTEGER(1, category);

		V8_RETURN_INTEGER(vehicle->GetModsCount(category));
	}

	void SetMod(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INTEGER(1, category);
		V8_ARG_TO_INTEGER(2, id);
		
		if (!vehicle->SetMod(category, id))
		{
			if (vehicle->GetModKit() == 0)
				V8Helpers::Throw(isolate, "modkit is not set");
			else
				V8Helpers::Throw(isolate, "mod doesn't exist");
		}
	}

	void GetMod(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INTEGER(1, category);
		
		V8_RETURN_INTEGER(vehicle->GetMod(category));
	}

	void SetExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INTEGER(1, category);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->ToggleExtra(category, state);
	}

	void GetExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INTEGER(1, category);
		
		V8_RETURN_BOOLEAN(vehicle->IsExtraOn(category));
	}

	void SetAppearanceData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_STRING(1, data);
		
		vehicle->LoadAppearanceDataFromBase64(data);
	}

	void GetAppearanceData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_STRING(vehicle->GetAppearanceDataBase64().CStr());
	}
}
