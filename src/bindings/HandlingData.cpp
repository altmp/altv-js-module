
#include "../CV8Resource.h"
#include "../helpers/V8Class.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_CONSTRUCTOR();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "model doesn't exist");

	info.This()->SetInternalField(0, info[0]);
}

static void GetForHandlingName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, modelHash);

	std::vector<v8::Local<v8::Value>> args{
		v8::Number::New(isolate, modelHash) 
	};

	extern V8Class v8HandlingData;
	V8_RETURN(v8HandlingData.New(isolate->GetEnteredContext(), args));
}

static void HandlingNameHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	uint32_t modelHash2 = info.This()->GetInternalField(0)->IntegerValue(ctx).ToChecked();

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetHandlingNameHash());
}

static void MassGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetMass());
}

static void MassSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetMass(fvalue);
}

static void InitialDragCoeffGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetInitialDragCoeff());
}

static void InitialDragCoeffSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetInitialDragCoeff(fvalue);
}

static void DownforceModifierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetDownforceModifier());
}

static void DownforceModifierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetDownforceModifier(fvalue);
}

static void unkFloat1Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetunkFloat1());
}

static void unkFloat1Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetunkFloat1(fvalue);
}

static void unkFloat2Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetunkFloat2());
}

static void unkFloat2Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetunkFloat2(fvalue);
}

static void CentreOfMassOffsetGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN(resource->CreateVector3(handling->GetCentreOfMassOffset()));
}

static void CentreOfMassOffsetSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_OBJECT(val, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	handling->SetCentreOfMassOffset({ x, y, z });
}

static void InertiaMultiplierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN(resource->CreateVector3(handling->GetInertiaMultiplier()));
}

static void InertiaMultiplierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_OBJECT(val, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	handling->SetInertiaMultiplier({ x, y, z });
}

static void PercentSubmergedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetPercentSubmerged());
}

static void PercentSubmergedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetPercentSubmerged(fvalue);
}

static void PercentSubmergedRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetPercentSubmergedRatio());
}

static void PercentSubmergedRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetPercentSubmergedRatio(fvalue);
}

static void DriveBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetDriveBiasFront());
}

static void DriveBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetDriveBiasFront(fvalue);
}

static void AccelerationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetAcceleration());
}

static void AccelerationSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetAcceleration(fvalue);
}

static void InitialDriveGearsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetInitialDriveGears());
}

static void InitialDriveGearsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetInitialDriveGears(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void DriveInertiaGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetDriveInertia());
}

static void DriveInertiaSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetDriveInertia(fvalue);
}

static void ClutchChangeRateScaleUpShiftGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetClutchChangeRateScaleUpShift());
}

static void ClutchChangeRateScaleUpShiftSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetClutchChangeRateScaleUpShift(fvalue);
}

static void ClutchChangeRateScaleDownShiftGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetClutchChangeRateScaleDownShift());
}

static void ClutchChangeRateScaleDownShiftSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetClutchChangeRateScaleDownShift(fvalue);
}

static void InitialDriveForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetInitialDriveForce());
}

static void InitialDriveForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetInitialDriveForce(fvalue);
}

static void DriveMaxFlatVelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetDriveMaxFlatVel());
}

static void DriveMaxFlatVelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetDriveMaxFlatVel(fvalue);
}

static void InitialDriveMaxFlatVelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetInitialDriveMaxFlatVel());
}

static void InitialDriveMaxFlatVelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetInitialDriveMaxFlatVel(fvalue);
}

static void BrakeForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetBrakeForce());
}

static void BrakeForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetBrakeForce(fvalue);
}

static void unkFloat4Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetunkFloat4());
}

static void unkFloat4Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetunkFloat4(fvalue);
}

static void BrakeBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetBrakeBiasFront());
}

static void BrakeBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetBrakeBiasFront(fvalue);
}

static void BrakeBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetBrakeBiasRear());
}

static void BrakeBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetBrakeBiasRear(fvalue);
}

static void HandBrakeForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetHandBrakeForce());
}

static void HandBrakeForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetHandBrakeForce(fvalue);
}

static void SteeringLockGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSteeringLock());
}

static void SteeringLockSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSteeringLock(fvalue);
}

static void SteeringLockRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSteeringLockRatio());
}

static void SteeringLockRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSteeringLockRatio(fvalue);
}

static void TractionCurveMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionCurveMax());
}

static void TractionCurveMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionCurveMax(fvalue);
}

static void TractionCurveMaxRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionCurveMaxRatio());
}

static void TractionCurveMaxRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionCurveMaxRatio(fvalue);
}

static void TractionCurveMinGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionCurveMin());
}

static void TractionCurveMinSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionCurveMin(fvalue);
}

static void TractionCurveMinRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionCurveMinRatio());
}

static void TractionCurveMinRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionCurveMinRatio(fvalue);
}

static void TractionCurveLateralGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionCurveLateral());
}

static void TractionCurveLateralSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionCurveLateral(fvalue);
}

static void TractionCurveLateralRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionCurveLateralRatio());
}

static void TractionCurveLateralRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionCurveLateralRatio(fvalue);
}

static void TractionSpringDeltaMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionSpringDeltaMax());
}

static void TractionSpringDeltaMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionSpringDeltaMax(fvalue);
}

static void TractionSpringDeltaMaxRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionSpringDeltaMaxRatio());
}

static void TractionSpringDeltaMaxRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionSpringDeltaMaxRatio(fvalue);
}

static void LowSpeedTractionLossMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetLowSpeedTractionLossMult());
}

static void LowSpeedTractionLossMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetLowSpeedTractionLossMult(fvalue);
}

static void CamberStiffnesssGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetCamberStiffnesss());
}

static void CamberStiffnesssSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetCamberStiffnesss(fvalue);
}

static void TractionBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionBiasFront());
}

static void TractionBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionBiasFront(fvalue);
}

static void TractionBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionBiasRear());
}

static void TractionBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionBiasRear(fvalue);
}

static void TractionLossMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetTractionLossMult());
}

static void TractionLossMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetTractionLossMult(fvalue);
}

static void SuspensionForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionForce());
}

static void SuspensionForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionForce(fvalue);
}

static void SuspensionCompDampGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionCompDamp());
}

static void SuspensionCompDampSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionCompDamp(fvalue);
}

static void SuspensionReboundDampGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionReboundDamp());
}

static void SuspensionReboundDampSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionReboundDamp(fvalue);
}

static void SuspensionUpperLimitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionUpperLimit());
}

static void SuspensionUpperLimitSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionUpperLimit(fvalue);
}

static void SuspensionLowerLimitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionLowerLimit());
}

static void SuspensionLowerLimitSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionLowerLimit(fvalue);
}

static void SuspensionRaiseGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionRaise());
}

static void SuspensionRaiseSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionRaise(fvalue);
}

static void SuspensionBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionBiasFront());
}

static void SuspensionBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionBiasFront(fvalue);
}

static void SuspensionBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSuspensionBiasRear());
}

static void SuspensionBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSuspensionBiasRear(fvalue);
}

static void AntiRollBarForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetAntiRollBarForce());
}

static void AntiRollBarForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetAntiRollBarForce(fvalue);
}

static void AntiRollBarBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetAntiRollBarBiasFront());
}

static void AntiRollBarBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetAntiRollBarBiasFront(fvalue);
}

static void AntiRollBarBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetAntiRollBarBiasRear());
}

static void AntiRollBarBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetAntiRollBarBiasRear(fvalue);
}

static void RollCentreHeightFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetRollCentreHeightFront());
}

static void RollCentreHeightFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetRollCentreHeightFront(fvalue);
}

static void RollCentreHeightRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetRollCentreHeightRear());
}

static void RollCentreHeightRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetRollCentreHeightRear(fvalue);
}

static void CollisionDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetCollisionDamageMult());
}

static void CollisionDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetCollisionDamageMult(fvalue);
}

static void WeaponDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetWeaponDamageMult());
}

static void WeaponDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetWeaponDamageMult(fvalue);
}

static void DeformationDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetDeformationDamageMult());
}

static void DeformationDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetDeformationDamageMult(fvalue);
}

static void EngineDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetEngineDamageMult());
}

static void EngineDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetEngineDamageMult(fvalue);
}

static void PetrolTankVolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetPetrolTankVolume());
}

static void PetrolTankVolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetPetrolTankVolume(fvalue);
}

static void OilVolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetOilVolume());
}

static void OilVolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetOilVolume(fvalue);
}

static void unkFloat5Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetunkFloat5());
}

static void unkFloat5Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetunkFloat5(fvalue);
}

static void SeatOffsetDistXGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSeatOffsetDistX());
}

static void SeatOffsetDistXSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSeatOffsetDistX(fvalue);
}

static void SeatOffsetDistYGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSeatOffsetDistY());
}

static void SeatOffsetDistYSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSeatOffsetDistY(fvalue);
}

static void SeatOffsetDistZGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetSeatOffsetDistZ());
}

static void SeatOffsetDistZSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_TO_NUMBER(val, fvalue);

	handling->SetSeatOffsetDistZ(fvalue);
}

static void MonetaryValueGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetMonetaryValue());
}

static void MonetaryValueSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetMonetaryValue(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void ModelFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetModelFlags());
}

static void ModelFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetModelFlags(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void HandlingFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetHandlingFlags());
}

static void HandlingFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetHandlingFlags(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void DamageFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	V8_RETURN_NUMBER(handling->GetDamageFlags());
}

static void DamageFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_INTERNAL_FIELD_INTEGER(1, modelHash);

	auto handling = alt::ICore::Instance().GetHandlingData(modelHash);
	V8_CHECK(handling, "handling data for vehicle not found");

	handling->SetDamageFlags(val->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

extern V8Class v8HandlingData("HandlingData", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();

		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		V8::SetMethod(isolate, tpl, "getForHandlingName", &GetForHandlingName);

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
