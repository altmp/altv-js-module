
#include "../CV8Resource.h"
#include "../helpers/V8Class.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	V8_CHECK(info.IsConstructCall(), "HandlingData constructor is not a function");
	V8_CHECK(info.Length() == 1, "new HandlingData(...) expects 1 arg");

	V8_CHECK(info[0]->IsNumber(), "modelHash must be a number");
	uint32_t modelHash = info[0]->Uint32Value(ctx).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "model doesn't exist");

	info.This()->SetInternalField(0, info[0]);
}

static void GetForHandlingName(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "HandlingData.getForHandlingName expects 1 arg");
	V8_CHECK(info[0]->IsNumber(), "modelHash must be a number");
	uint32_t modelHash = info[0]->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	extern V8Class v8HandlingData;

	std::vector<v8::Local<v8::Value>> args{
		v8::Number::New(isolate, modelHash)};

	info.GetReturnValue().Set(v8HandlingData.New(isolate->GetEnteredContext(), args));
}

static void HandlingNameHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetHandlingNameHash()));
}

static void MassGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetMass()));
}

static void MassSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "mass must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetMass((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void InitialDragCoeffGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetInitialDragCoeff()));
}

static void InitialDragCoeffSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "initialDragCoeff must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetInitialDragCoeff((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void DownforceModifierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetDownforceModifier()));
}

static void DownforceModifierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "downforceModifier must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDownforceModifier((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void unkFloat1Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetunkFloat1()));
}

static void unkFloat1Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "unkFloat1 must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetunkFloat1((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void unkFloat2Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetunkFloat2()));
}

static void unkFloat2Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "unkFloat2 must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetunkFloat2((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void CentreOfMassOffsetGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(resource->CreateVector3(handling->GetCentreOfMassOffset()));
}

static void CentreOfMassOffsetSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();
	V8_CHECK(val->IsObject(), "centreOfMassOffset must be a Vector3");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	v8::Local<v8::Object> pos = val.As<v8::Object>();

	v8::Local<v8::Number> x = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "x").ToLocalChecked()).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> y = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "y").ToLocalChecked()).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> z = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "z").ToLocalChecked()).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();

	handling->SetCentreOfMassOffset({(float)x->Value(), (float)y->Value(), (float)z->Value()});
}

static void InertiaMultiplierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(resource->CreateVector3(handling->GetInertiaMultiplier()));
}

static void InertiaMultiplierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();
	V8_CHECK(val->IsObject(), "inertiaMultiplier must be a Vector3");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	v8::Local<v8::Object> pos = val.As<v8::Object>();

	v8::Local<v8::Number> x = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "x").ToLocalChecked()).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> y = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "y").ToLocalChecked()).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> z = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "z").ToLocalChecked()).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();

	handling->SetInertiaMultiplier({(float)x->Value(), (float)y->Value(), (float)z->Value()});
}

static void PercentSubmergedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetPercentSubmerged()));
}

static void PercentSubmergedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "percentSubmerged must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetPercentSubmerged((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void PercentSubmergedRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetPercentSubmergedRatio()));
}

static void PercentSubmergedRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "percentSubmergedRatio must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetPercentSubmergedRatio((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void DriveBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetDriveBiasFront()));
}

static void DriveBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "driveBiasFront must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDriveBiasFront((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void AccelerationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetAcceleration()));
}

static void AccelerationSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "acceleration must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetAcceleration((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void InitialDriveGearsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetInitialDriveGears()));
}

static void InitialDriveGearsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "initialDriveGears must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetInitialDriveGears(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void DriveInertiaGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetDriveInertia()));
}

static void DriveInertiaSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "driveInertia must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDriveInertia((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void ClutchChangeRateScaleUpShiftGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetClutchChangeRateScaleUpShift()));
}

static void ClutchChangeRateScaleUpShiftSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "clutchChangeRateScaleUpShift must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetClutchChangeRateScaleUpShift((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void ClutchChangeRateScaleDownShiftGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetClutchChangeRateScaleDownShift()));
}

static void ClutchChangeRateScaleDownShiftSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "clutchChangeRateScaleDownShift must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetClutchChangeRateScaleDownShift((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void InitialDriveForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetInitialDriveForce()));
}

static void InitialDriveForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "initialDriveForce must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetInitialDriveForce((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void DriveMaxFlatVelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetDriveMaxFlatVel()));
}

static void DriveMaxFlatVelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "driveMaxFlatVel must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDriveMaxFlatVel((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void InitialDriveMaxFlatVelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetInitialDriveMaxFlatVel()));
}

static void InitialDriveMaxFlatVelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "initialDriveMaxFlatVel must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetInitialDriveMaxFlatVel((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void BrakeForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetBrakeForce()));
}

static void BrakeForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "brakeForce must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetBrakeForce((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void unkFloat4Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetunkFloat4()));
}

static void unkFloat4Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "unkFloat4 must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetunkFloat4((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void BrakeBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetBrakeBiasFront()));
}

static void BrakeBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "brakeBiasFront must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetBrakeBiasFront((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void BrakeBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetBrakeBiasRear()));
}

static void BrakeBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "brakeBiasRear must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetBrakeBiasRear((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void HandBrakeForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetHandBrakeForce()));
}

static void HandBrakeForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "handBrakeForce must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetHandBrakeForce((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SteeringLockGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSteeringLock()));
}

static void SteeringLockSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "steeringLock must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSteeringLock((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SteeringLockRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSteeringLockRatio()));
}

static void SteeringLockRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "steeringLockRatio must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSteeringLockRatio((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionCurveMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionCurveMax()));
}

static void TractionCurveMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionCurveMax must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionCurveMax((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionCurveMaxRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionCurveMaxRatio()));
}

static void TractionCurveMaxRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionCurveMaxRatio must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionCurveMaxRatio((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionCurveMinGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionCurveMin()));
}

static void TractionCurveMinSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionCurveMin must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionCurveMin((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionCurveMinRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionCurveMinRatio()));
}

static void TractionCurveMinRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionCurveMinRatio must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionCurveMinRatio((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionCurveLateralGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionCurveLateral()));
}

static void TractionCurveLateralSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionCurveLateral must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionCurveLateral((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionCurveLateralRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionCurveLateralRatio()));
}

static void TractionCurveLateralRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionCurveLateralRatio must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionCurveLateralRatio((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionSpringDeltaMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionSpringDeltaMax()));
}

static void TractionSpringDeltaMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionSpringDeltaMax must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionSpringDeltaMax((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionSpringDeltaMaxRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionSpringDeltaMaxRatio()));
}

static void TractionSpringDeltaMaxRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionSpringDeltaMaxRatio must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionSpringDeltaMaxRatio((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void LowSpeedTractionLossMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetLowSpeedTractionLossMult()));
}

static void LowSpeedTractionLossMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "lowSpeedTractionLossMult must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetLowSpeedTractionLossMult((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void CamberStiffnesssGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetCamberStiffnesss()));
}

static void CamberStiffnesssSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "camberStiffnesss must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetCamberStiffnesss((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionBiasFront()));
}

static void TractionBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionBiasFront must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionBiasFront((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionBiasRear()));
}

static void TractionBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionBiasRear must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionBiasRear((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void TractionLossMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetTractionLossMult()));
}

static void TractionLossMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "tractionLossMult must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetTractionLossMult((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionForce()));
}

static void SuspensionForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionForce must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionForce((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionCompDampGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionCompDamp()));
}

static void SuspensionCompDampSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionCompDamp must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionCompDamp((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionReboundDampGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionReboundDamp()));
}

static void SuspensionReboundDampSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionReboundDamp must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionReboundDamp((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionUpperLimitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionUpperLimit()));
}

static void SuspensionUpperLimitSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionUpperLimit must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionUpperLimit((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionLowerLimitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionLowerLimit()));
}

static void SuspensionLowerLimitSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionLowerLimit must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionLowerLimit((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionRaiseGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionRaise()));
}

static void SuspensionRaiseSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionRaise must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionRaise((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionBiasFront()));
}

static void SuspensionBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionBiasFront must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionBiasFront((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SuspensionBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSuspensionBiasRear()));
}

static void SuspensionBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "suspensionBiasRear must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSuspensionBiasRear((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void AntiRollBarForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetAntiRollBarForce()));
}

static void AntiRollBarForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "antiRollBarForce must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetAntiRollBarForce((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void AntiRollBarBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetAntiRollBarBiasFront()));
}

static void AntiRollBarBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "antiRollBarBiasFront must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetAntiRollBarBiasFront((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void AntiRollBarBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetAntiRollBarBiasRear()));
}

static void AntiRollBarBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "antiRollBarBiasRear must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetAntiRollBarBiasRear((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void RollCentreHeightFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetRollCentreHeightFront()));
}

static void RollCentreHeightFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "rollCentreHeightFront must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetRollCentreHeightFront((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void RollCentreHeightRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetRollCentreHeightRear()));
}

static void RollCentreHeightRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "rollCentreHeightRear must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetRollCentreHeightRear((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void CollisionDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetCollisionDamageMult()));
}

static void CollisionDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "collisionDamageMult must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetCollisionDamageMult((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void WeaponDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetWeaponDamageMult()));
}

static void WeaponDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "weaponDamageMult must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetWeaponDamageMult((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void DeformationDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetDeformationDamageMult()));
}

static void DeformationDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "deformationDamageMult must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDeformationDamageMult((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void EngineDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetEngineDamageMult()));
}

static void EngineDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "engineDamageMult must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetEngineDamageMult((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void PetrolTankVolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetPetrolTankVolume()));
}

static void PetrolTankVolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "petrolTankVolume must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetPetrolTankVolume((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void OilVolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetOilVolume()));
}

static void OilVolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "oilVolume must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetOilVolume((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void unkFloat5Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	auto ctx = isolate->GetEnteredContext();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetunkFloat5()));
}

static void unkFloat5Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "unkFloat5 must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetunkFloat5((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SeatOffsetDistXGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSeatOffsetDistX()));
}

static void SeatOffsetDistXSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "seatOffsetDistX must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSeatOffsetDistX((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SeatOffsetDistYGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSeatOffsetDistY()));
}

static void SeatOffsetDistYSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "seatOffsetDistY must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSeatOffsetDistY((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void SeatOffsetDistZGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetSeatOffsetDistZ()));
}

static void SeatOffsetDistZSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "seatOffsetDistZ must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetSeatOffsetDistZ((float)val->NumberValue(isolate->GetEnteredContext()).ToChecked());
}

static void MonetaryValueGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetMonetaryValue()));
}

static void MonetaryValueSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "monetaryValue must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetMonetaryValue(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void ModelFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetModelFlags()));
}

static void ModelFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "modelFlags must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetModelFlags(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void HandlingFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetHandlingFlags()));
}

static void HandlingFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "handlingFlags must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetHandlingFlags(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void DamageFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	info.GetReturnValue().Set(v8::Number::New(isolate, handling->GetDamageFlags()));
}

static void DamageFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	V8_CHECK(val->IsNumber(), "damageFlags must be a number");

	uint32_t modelHash = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDamageFlags(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

extern V8Class v8HandlingData(
	"HandlingData", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();

		v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		tpl->Set(isolate, "getForHandlingName", v8::FunctionTemplate::New(isolate, &GetForHandlingName));

		V8::SetAccessor(isolate, tpl, "handlingNameHash", &HandlingNameHashGetter);
		V8::SetAccessor(isolate, tpl, "mass", &MassGetter, &MassSetter);
		V8::SetAccessor(isolate, tpl, "initialDragCoeff", &InitialDragCoeffGetter, &InitialDragCoeffSetter);
		V8::SetAccessor(isolate, tpl, "downforceModifier", &DownforceModifierGetter, &DownforceModifierSetter);
		V8::SetAccessor(isolate, tpl, "unkFloat1", &unkFloat1Getter, &unkFloat1Setter);
		V8::SetAccessor(isolate, tpl, "unkFloat2", &unkFloat2Getter, &unkFloat2Setter);
		V8::SetAccessor(isolate, tpl, "centreOfMassOffset", &CentreOfMassOffsetGetter, &CentreOfMassOffsetSetter);
		V8::SetAccessor(isolate, tpl, "inertiaMultiplier", &InertiaMultiplierGetter, &InertiaMultiplierSetter);
		V8::SetAccessor(isolate, tpl, "percentSubmerged", &PercentSubmergedGetter, &PercentSubmergedSetter);
		V8::SetAccessor(isolate, tpl, "percentSubmergedRatio", &PercentSubmergedRatioGetter, &PercentSubmergedRatioSetter);
		V8::SetAccessor(isolate, tpl, "driveBiasFront", &DriveBiasFrontGetter, &DriveBiasFrontSetter);
		V8::SetAccessor(isolate, tpl, "acceleration", &AccelerationGetter, &AccelerationSetter);
		V8::SetAccessor(isolate, tpl, "initialDriveGears", &InitialDriveGearsGetter, &InitialDriveGearsSetter);
		V8::SetAccessor(isolate, tpl, "driveInertia", &DriveInertiaGetter, &DriveInertiaSetter);
		V8::SetAccessor(isolate, tpl, "clutchChangeRateScaleUpShift", &ClutchChangeRateScaleUpShiftGetter, &ClutchChangeRateScaleUpShiftSetter);
		V8::SetAccessor(isolate, tpl, "clutchChangeRateScaleDownShift", &ClutchChangeRateScaleDownShiftGetter, &ClutchChangeRateScaleDownShiftSetter);
		V8::SetAccessor(isolate, tpl, "initialDriveForce", &InitialDriveForceGetter, &InitialDriveForceSetter);
		V8::SetAccessor(isolate, tpl, "driveMaxFlatVel", &DriveMaxFlatVelGetter, &DriveMaxFlatVelSetter);
		V8::SetAccessor(isolate, tpl, "initialDriveMaxFlatVel", &InitialDriveMaxFlatVelGetter, &InitialDriveMaxFlatVelSetter);
		V8::SetAccessor(isolate, tpl, "brakeForce", &BrakeForceGetter, &BrakeForceSetter);
		V8::SetAccessor(isolate, tpl, "unkFloat4", &unkFloat4Getter, &unkFloat4Setter);
		V8::SetAccessor(isolate, tpl, "brakeBiasFront", &BrakeBiasFrontGetter, &BrakeBiasFrontSetter);
		V8::SetAccessor(isolate, tpl, "brakeBiasRear", &BrakeBiasRearGetter, &BrakeBiasRearSetter);
		V8::SetAccessor(isolate, tpl, "handBrakeForce", &HandBrakeForceGetter, &HandBrakeForceSetter);
		V8::SetAccessor(isolate, tpl, "steeringLock", &SteeringLockGetter, &SteeringLockSetter);
		V8::SetAccessor(isolate, tpl, "steeringLockRatio", &SteeringLockRatioGetter, &SteeringLockRatioSetter);
		V8::SetAccessor(isolate, tpl, "tractionCurveMax", &TractionCurveMaxGetter, &TractionCurveMaxSetter);
		V8::SetAccessor(isolate, tpl, "tractionCurveMaxRatio", &TractionCurveMaxRatioGetter, &TractionCurveMaxRatioSetter);
		V8::SetAccessor(isolate, tpl, "tractionCurveMin", &TractionCurveMinGetter, &TractionCurveMinSetter);
		V8::SetAccessor(isolate, tpl, "tractionCurveMinRatio", &TractionCurveMinRatioGetter, &TractionCurveMinRatioSetter);
		V8::SetAccessor(isolate, tpl, "tractionCurveLateral", &TractionCurveLateralGetter, &TractionCurveLateralSetter);
		V8::SetAccessor(isolate, tpl, "tractionCurveLateralRatio", &TractionCurveLateralRatioGetter, &TractionCurveLateralRatioSetter);
		V8::SetAccessor(isolate, tpl, "tractionSpringDeltaMax", &TractionSpringDeltaMaxGetter, &TractionSpringDeltaMaxSetter);
		V8::SetAccessor(isolate, tpl, "tractionSpringDeltaMaxRatio", &TractionSpringDeltaMaxRatioGetter, &TractionSpringDeltaMaxRatioSetter);
		V8::SetAccessor(isolate, tpl, "lowSpeedTractionLossMult", &LowSpeedTractionLossMultGetter, &LowSpeedTractionLossMultSetter);
		V8::SetAccessor(isolate, tpl, "camberStiffnesss", &CamberStiffnesssGetter, &CamberStiffnesssSetter);
		V8::SetAccessor(isolate, tpl, "tractionBiasFront", &TractionBiasFrontGetter, &TractionBiasFrontSetter);
		V8::SetAccessor(isolate, tpl, "tractionBiasRear", &TractionBiasRearGetter, &TractionBiasRearSetter);
		V8::SetAccessor(isolate, tpl, "tractionLossMult", &TractionLossMultGetter, &TractionLossMultSetter);
		V8::SetAccessor(isolate, tpl, "suspensionForce", &SuspensionForceGetter, &SuspensionForceSetter);
		V8::SetAccessor(isolate, tpl, "suspensionCompDamp", &SuspensionCompDampGetter, &SuspensionCompDampSetter);
		V8::SetAccessor(isolate, tpl, "suspensionReboundDamp", &SuspensionReboundDampGetter, &SuspensionReboundDampSetter);
		V8::SetAccessor(isolate, tpl, "suspensionUpperLimit", &SuspensionUpperLimitGetter, &SuspensionUpperLimitSetter);
		V8::SetAccessor(isolate, tpl, "suspensionLowerLimit", &SuspensionLowerLimitGetter, &SuspensionLowerLimitSetter);
		V8::SetAccessor(isolate, tpl, "suspensionRaise", &SuspensionRaiseGetter, &SuspensionRaiseSetter);
		V8::SetAccessor(isolate, tpl, "suspensionBiasFront", &SuspensionBiasFrontGetter, &SuspensionBiasFrontSetter);
		V8::SetAccessor(isolate, tpl, "suspensionBiasRear", &SuspensionBiasRearGetter, &SuspensionBiasRearSetter);
		V8::SetAccessor(isolate, tpl, "antiRollBarForce", &AntiRollBarForceGetter, &AntiRollBarForceSetter);
		V8::SetAccessor(isolate, tpl, "antiRollBarBiasFront", &AntiRollBarBiasFrontGetter, &AntiRollBarBiasFrontSetter);
		V8::SetAccessor(isolate, tpl, "antiRollBarBiasRear", &AntiRollBarBiasRearGetter, &AntiRollBarBiasRearSetter);
		V8::SetAccessor(isolate, tpl, "rollCentreHeightFront", &RollCentreHeightFrontGetter, &RollCentreHeightFrontSetter);
		V8::SetAccessor(isolate, tpl, "rollCentreHeightRear", &RollCentreHeightRearGetter, &RollCentreHeightRearSetter);
		V8::SetAccessor(isolate, tpl, "collisionDamageMult", &CollisionDamageMultGetter, &CollisionDamageMultSetter);
		V8::SetAccessor(isolate, tpl, "weaponDamageMult", &WeaponDamageMultGetter, &WeaponDamageMultSetter);
		V8::SetAccessor(isolate, tpl, "deformationDamageMult", &DeformationDamageMultGetter, &DeformationDamageMultSetter);
		V8::SetAccessor(isolate, tpl, "engineDamageMult", &EngineDamageMultGetter, &EngineDamageMultSetter);
		V8::SetAccessor(isolate, tpl, "petrolTankVolume", &PetrolTankVolumeGetter, &PetrolTankVolumeSetter);
		V8::SetAccessor(isolate, tpl, "oilVolume", &OilVolumeGetter, &OilVolumeSetter);
		V8::SetAccessor(isolate, tpl, "unkFloat5", &unkFloat5Getter, &unkFloat5Setter);
		V8::SetAccessor(isolate, tpl, "seatOffsetDistX", &SeatOffsetDistXGetter, &SeatOffsetDistXSetter);
		V8::SetAccessor(isolate, tpl, "seatOffsetDistY", &SeatOffsetDistYGetter, &SeatOffsetDistYSetter);
		V8::SetAccessor(isolate, tpl, "seatOffsetDistZ", &SeatOffsetDistZGetter, &SeatOffsetDistZSetter);
		V8::SetAccessor(isolate, tpl, "monetaryValue", &MonetaryValueGetter, &MonetaryValueSetter);
		V8::SetAccessor(isolate, tpl, "modelFlags", &ModelFlagsGetter, &ModelFlagsSetter);
		V8::SetAccessor(isolate, tpl, "handlingFlags", &HandlingFlagsGetter, &HandlingFlagsSetter);
		V8::SetAccessor(isolate, tpl, "damageFlags", &DamageFlagsGetter, &DamageFlagsSetter);
	});
