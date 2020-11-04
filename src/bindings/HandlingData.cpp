
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

		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handlingNameHash").ToLocalChecked(), &HandlingNameHashGetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "mass").ToLocalChecked(), &MassGetter, &MassSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "initialDragCoeff").ToLocalChecked(), &InitialDragCoeffGetter, &InitialDragCoeffSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "downforceModifier").ToLocalChecked(), &DownforceModifierGetter, &DownforceModifierSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "unkFloat1").ToLocalChecked(), &unkFloat1Getter, &unkFloat1Setter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "unkFloat2").ToLocalChecked(), &unkFloat2Getter, &unkFloat2Setter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "centreOfMassOffset").ToLocalChecked(), &CentreOfMassOffsetGetter, &CentreOfMassOffsetSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "inertiaMultiplier").ToLocalChecked(), &InertiaMultiplierGetter, &InertiaMultiplierSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "percentSubmerged").ToLocalChecked(), &PercentSubmergedGetter, &PercentSubmergedSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "percentSubmergedRatio").ToLocalChecked(), &PercentSubmergedRatioGetter, &PercentSubmergedRatioSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "driveBiasFront").ToLocalChecked(), &DriveBiasFrontGetter, &DriveBiasFrontSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "acceleration").ToLocalChecked(), &AccelerationGetter, &AccelerationSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "initialDriveGears").ToLocalChecked(), &InitialDriveGearsGetter, &InitialDriveGearsSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "driveInertia").ToLocalChecked(), &DriveInertiaGetter, &DriveInertiaSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "clutchChangeRateScaleUpShift").ToLocalChecked(), &ClutchChangeRateScaleUpShiftGetter, &ClutchChangeRateScaleUpShiftSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "clutchChangeRateScaleDownShift").ToLocalChecked(), &ClutchChangeRateScaleDownShiftGetter, &ClutchChangeRateScaleDownShiftSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "initialDriveForce").ToLocalChecked(), &InitialDriveForceGetter, &InitialDriveForceSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "driveMaxFlatVel").ToLocalChecked(), &DriveMaxFlatVelGetter, &DriveMaxFlatVelSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "initialDriveMaxFlatVel").ToLocalChecked(), &InitialDriveMaxFlatVelGetter, &InitialDriveMaxFlatVelSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "brakeForce").ToLocalChecked(), &BrakeForceGetter, &BrakeForceSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "unkFloat4").ToLocalChecked(), &unkFloat4Getter, &unkFloat4Setter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "brakeBiasFront").ToLocalChecked(), &BrakeBiasFrontGetter, &BrakeBiasFrontSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "brakeBiasRear").ToLocalChecked(), &BrakeBiasRearGetter, &BrakeBiasRearSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handBrakeForce").ToLocalChecked(), &HandBrakeForceGetter, &HandBrakeForceSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "steeringLock").ToLocalChecked(), &SteeringLockGetter, &SteeringLockSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "steeringLockRatio").ToLocalChecked(), &SteeringLockRatioGetter, &SteeringLockRatioSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionCurveMax").ToLocalChecked(), &TractionCurveMaxGetter, &TractionCurveMaxSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionCurveMaxRatio").ToLocalChecked(), &TractionCurveMaxRatioGetter, &TractionCurveMaxRatioSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionCurveMin").ToLocalChecked(), &TractionCurveMinGetter, &TractionCurveMinSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionCurveMinRatio").ToLocalChecked(), &TractionCurveMinRatioGetter, &TractionCurveMinRatioSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionCurveLateral").ToLocalChecked(), &TractionCurveLateralGetter, &TractionCurveLateralSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionCurveLateralRatio").ToLocalChecked(), &TractionCurveLateralRatioGetter, &TractionCurveLateralRatioSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionSpringDeltaMax").ToLocalChecked(), &TractionSpringDeltaMaxGetter, &TractionSpringDeltaMaxSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionSpringDeltaMaxRatio").ToLocalChecked(), &TractionSpringDeltaMaxRatioGetter, &TractionSpringDeltaMaxRatioSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lowSpeedTractionLossMult").ToLocalChecked(), &LowSpeedTractionLossMultGetter, &LowSpeedTractionLossMultSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "camberStiffnesss").ToLocalChecked(), &CamberStiffnesssGetter, &CamberStiffnesssSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionBiasFront").ToLocalChecked(), &TractionBiasFrontGetter, &TractionBiasFrontSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionBiasRear").ToLocalChecked(), &TractionBiasRearGetter, &TractionBiasRearSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tractionLossMult").ToLocalChecked(), &TractionLossMultGetter, &TractionLossMultSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionForce").ToLocalChecked(), &SuspensionForceGetter, &SuspensionForceSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionCompDamp").ToLocalChecked(), &SuspensionCompDampGetter, &SuspensionCompDampSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionReboundDamp").ToLocalChecked(), &SuspensionReboundDampGetter, &SuspensionReboundDampSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionUpperLimit").ToLocalChecked(), &SuspensionUpperLimitGetter, &SuspensionUpperLimitSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionLowerLimit").ToLocalChecked(), &SuspensionLowerLimitGetter, &SuspensionLowerLimitSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionRaise").ToLocalChecked(), &SuspensionRaiseGetter, &SuspensionRaiseSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionBiasFront").ToLocalChecked(), &SuspensionBiasFrontGetter, &SuspensionBiasFrontSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "suspensionBiasRear").ToLocalChecked(), &SuspensionBiasRearGetter, &SuspensionBiasRearSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "antiRollBarForce").ToLocalChecked(), &AntiRollBarForceGetter, &AntiRollBarForceSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "antiRollBarBiasFront").ToLocalChecked(), &AntiRollBarBiasFrontGetter, &AntiRollBarBiasFrontSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "antiRollBarBiasRear").ToLocalChecked(), &AntiRollBarBiasRearGetter, &AntiRollBarBiasRearSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "rollCentreHeightFront").ToLocalChecked(), &RollCentreHeightFrontGetter, &RollCentreHeightFrontSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "rollCentreHeightRear").ToLocalChecked(), &RollCentreHeightRearGetter, &RollCentreHeightRearSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "collisionDamageMult").ToLocalChecked(), &CollisionDamageMultGetter, &CollisionDamageMultSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "weaponDamageMult").ToLocalChecked(), &WeaponDamageMultGetter, &WeaponDamageMultSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "deformationDamageMult").ToLocalChecked(), &DeformationDamageMultGetter, &DeformationDamageMultSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "engineDamageMult").ToLocalChecked(), &EngineDamageMultGetter, &EngineDamageMultSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "petrolTankVolume").ToLocalChecked(), &PetrolTankVolumeGetter, &PetrolTankVolumeSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "oilVolume").ToLocalChecked(), &OilVolumeGetter, &OilVolumeSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "unkFloat5").ToLocalChecked(), &unkFloat5Getter, &unkFloat5Setter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "seatOffsetDistX").ToLocalChecked(), &SeatOffsetDistXGetter, &SeatOffsetDistXSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "seatOffsetDistY").ToLocalChecked(), &SeatOffsetDistYGetter, &SeatOffsetDistYSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "seatOffsetDistZ").ToLocalChecked(), &SeatOffsetDistZGetter, &SeatOffsetDistZSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "monetaryValue").ToLocalChecked(), &MonetaryValueGetter, &MonetaryValueSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "modelFlags").ToLocalChecked(), &ModelFlagsGetter, &ModelFlagsSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handlingFlags").ToLocalChecked(), &HandlingFlagsGetter, &HandlingFlagsSetter);
		proto->SetAccessor(v8::String::NewFromUtf8(isolate, "damageFlags").ToLocalChecked(), &DamageFlagsGetter, &DamageFlagsSetter);
	});
