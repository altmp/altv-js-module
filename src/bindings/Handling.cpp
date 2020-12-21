
#include "../CV8Resource.h"
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "cpp-sdk/objects/IVehicle.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_OBJECT(1, vehicle);

	info.This()->SetInternalField(0, vehicle);
}

static void IsModified(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_BOOLEAN(vehicle->IsHandlingModified());
}

static void Reset(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	vehicle->ResetHandling();
}

static void HandlingNameHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_INTEGER(vehicle->GetHandling()->GetHandlingNameHash());
}

static void MassGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetMass());
}

static void MassSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetMass(fvalue);
}

static void InitialDragCoeffGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_NUMBER(vehicle->GetHandling()->GetInitialDragCoeff());
}

static void InitialDragCoeffSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetInitialDragCoeff(fvalue);
}

static void DownforceModifierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_NUMBER(vehicle->GetHandling()->GetDownforceModifier());
}

static void DownforceModifierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetDownforceModifier(fvalue);
}

static void unkFloat1Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_NUMBER(vehicle->GetHandling()->GetunkFloat1());
}

static void unkFloat1Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetunkFloat1(fvalue);
}

static void unkFloat2Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_NUMBER(vehicle->GetHandling()->GetunkFloat2());
}

static void unkFloat2Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetunkFloat2(fvalue);
}

static void CentreOfMassOffsetGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN(resource->CreateVector3(vehicle->GetHandling()->GetCentreOfMassOffset()));
}

static void CentreOfMassOffsetSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_OBJECT(val, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	vehicle->GetHandling()->SetCentreOfMassOffset({ x, y, z });
}

static void InertiaMultiplierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN(resource->CreateVector3(vehicle->GetHandling()->GetInertiaMultiplier()));
}

static void InertiaMultiplierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_OBJECT(val, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);
	V8_OBJECT_GET_NUMBER(pos, "z", z);

	vehicle->GetHandling()->SetInertiaMultiplier({ x, y, z });
}

static void PercentSubmergedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetPercentSubmerged());
}

static void PercentSubmergedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetPercentSubmerged(fvalue);
}

static void PercentSubmergedRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetPercentSubmergedRatio());
}

static void PercentSubmergedRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetPercentSubmergedRatio(fvalue);
}

static void DriveBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetDriveBiasFront());
}

static void DriveBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetDriveBiasFront(fvalue);
}

static void AccelerationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetAcceleration());
}

static void AccelerationSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetAcceleration(fvalue);
}

static void InitialDriveGearsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_INTEGER(vehicle->GetHandling()->GetInitialDriveGears());
}

static void InitialDriveGearsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_INTEGER(val, ivalue);
	vehicle->GetHandling()->SetInitialDriveGears(ivalue);
}

static void DriveInertiaGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetDriveInertia());
}

static void DriveInertiaSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetDriveInertia(fvalue);
}

static void ClutchChangeRateScaleUpShiftGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetClutchChangeRateScaleUpShift());
}

static void ClutchChangeRateScaleUpShiftSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetClutchChangeRateScaleUpShift(fvalue);
}

static void ClutchChangeRateScaleDownShiftGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetClutchChangeRateScaleDownShift());
}

static void ClutchChangeRateScaleDownShiftSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetClutchChangeRateScaleDownShift(fvalue);
}

static void InitialDriveForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetInitialDriveForce());
}

static void InitialDriveForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetInitialDriveForce(fvalue);
}

static void DriveMaxFlatVelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetDriveMaxFlatVel());
}

static void DriveMaxFlatVelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetDriveMaxFlatVel(fvalue);
}

static void InitialDriveMaxFlatVelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetInitialDriveMaxFlatVel());
}

static void InitialDriveMaxFlatVelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetInitialDriveMaxFlatVel(fvalue);
}

static void BrakeForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetBrakeForce());
}

static void BrakeForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetBrakeForce(fvalue);
}

static void unkFloat4Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetunkFloat4());
}

static void unkFloat4Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetunkFloat4(fvalue);
}

static void BrakeBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetBrakeBiasFront());
}

static void BrakeBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetBrakeBiasFront(fvalue);
}

static void BrakeBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetBrakeBiasRear());
}

static void BrakeBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetBrakeBiasRear(fvalue);
}

static void HandBrakeForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetHandBrakeForce());
}

static void HandBrakeForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetHandBrakeForce(fvalue);
}

static void SteeringLockGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSteeringLock());
}

static void SteeringLockSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSteeringLock(fvalue);
}

static void SteeringLockRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSteeringLockRatio());
}

static void SteeringLockRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSteeringLockRatio(fvalue);
}

static void TractionCurveMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionCurveMax());
}

static void TractionCurveMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionCurveMax(fvalue);
}

static void TractionCurveMaxRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionCurveMaxRatio());
}

static void TractionCurveMaxRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionCurveMaxRatio(fvalue);
}

static void TractionCurveMinGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionCurveMin());
}

static void TractionCurveMinSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionCurveMin(fvalue);
}

static void TractionCurveMinRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionCurveMinRatio());
}

static void TractionCurveMinRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionCurveMinRatio(fvalue);
}

static void TractionCurveLateralGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionCurveLateral());
}

static void TractionCurveLateralSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionCurveLateral(fvalue);
}

static void TractionCurveLateralRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionCurveLateralRatio());
}

static void TractionCurveLateralRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionCurveLateralRatio(fvalue);
}

static void TractionSpringDeltaMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionSpringDeltaMax());
}

static void TractionSpringDeltaMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionSpringDeltaMax(fvalue);
}

static void TractionSpringDeltaMaxRatioGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionSpringDeltaMaxRatio());
}

static void TractionSpringDeltaMaxRatioSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionSpringDeltaMaxRatio(fvalue);
}

static void LowSpeedTractionLossMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetLowSpeedTractionLossMult());
}

static void LowSpeedTractionLossMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetLowSpeedTractionLossMult(fvalue);
}

static void CamberStiffnessGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetCamberStiffness());
}

static void CamberStiffnessSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetCamberStiffness(fvalue);
}

static void TractionBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionBiasFront());
}

static void TractionBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionBiasFront(fvalue);
}

static void TractionBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionBiasRear());
}

static void TractionBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionBiasRear(fvalue);
}

static void TractionLossMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetTractionLossMult());
}

static void TractionLossMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetTractionLossMult(fvalue);
}

static void SuspensionForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionForce());
}

static void SuspensionForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionForce(fvalue);
}

static void SuspensionCompDampGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionCompDamp());
}

static void SuspensionCompDampSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionCompDamp(fvalue);
}

static void SuspensionReboundDampGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionReboundDamp());
}

static void SuspensionReboundDampSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionReboundDamp(fvalue);
}

static void SuspensionUpperLimitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionUpperLimit());
}

static void SuspensionUpperLimitSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionUpperLimit(fvalue);
}

static void SuspensionLowerLimitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionLowerLimit());
}

static void SuspensionLowerLimitSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionLowerLimit(fvalue);
}

static void SuspensionRaiseGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionRaise());
}

static void SuspensionRaiseSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionRaise(fvalue);
}

static void SuspensionBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionBiasFront());
}

static void SuspensionBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionBiasFront(fvalue);
}

static void SuspensionBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSuspensionBiasRear());
}

static void SuspensionBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSuspensionBiasRear(fvalue);
}

static void AntiRollBarForceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetAntiRollBarForce());
}

static void AntiRollBarForceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetAntiRollBarForce(fvalue);
}

static void AntiRollBarBiasFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetAntiRollBarBiasFront());
}

static void AntiRollBarBiasFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetAntiRollBarBiasFront(fvalue);
}

static void AntiRollBarBiasRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetAntiRollBarBiasRear());
}

static void AntiRollBarBiasRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetAntiRollBarBiasRear(fvalue);
}

static void RollCentreHeightFrontGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetRollCentreHeightFront());
}

static void RollCentreHeightFrontSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetRollCentreHeightFront(fvalue);
}

static void RollCentreHeightRearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetRollCentreHeightRear());
}

static void RollCentreHeightRearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetRollCentreHeightRear(fvalue);
}

static void CollisionDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetCollisionDamageMult());
}

static void CollisionDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetCollisionDamageMult(fvalue);
}

static void WeaponDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetWeaponDamageMult());
}

static void WeaponDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetWeaponDamageMult(fvalue);
}

static void DeformationDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetDeformationDamageMult());
}

static void DeformationDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetDeformationDamageMult(fvalue);
}

static void EngineDamageMultGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetEngineDamageMult());
}

static void EngineDamageMultSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetEngineDamageMult(fvalue);
}

static void PetrolTankVolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetPetrolTankVolume());
}

static void PetrolTankVolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetPetrolTankVolume(fvalue);
}

static void OilVolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetOilVolume());
}

static void OilVolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetOilVolume(fvalue);
}

static void unkFloat5Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetunkFloat5());
}

static void unkFloat5Setter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetunkFloat5(fvalue);
}

static void SeatOffsetDistXGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSeatOffsetDistX());
}

static void SeatOffsetDistXSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSeatOffsetDistX(fvalue);
}

static void SeatOffsetDistYGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSeatOffsetDistY());
}

static void SeatOffsetDistYSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSeatOffsetDistY(fvalue);
}

static void SeatOffsetDistZGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_NUMBER(vehicle->GetHandling()->GetSeatOffsetDistZ());
}

static void SeatOffsetDistZSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_NUMBER(val, fvalue);
	vehicle->GetHandling()->SetSeatOffsetDistZ(fvalue);
}

static void MonetaryValueGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_INTEGER(vehicle->GetHandling()->GetMonetaryValue());
}

static void MonetaryValueSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_INTEGER(val, ivalue);
	vehicle->GetHandling()->SetMonetaryValue(ivalue);
}

static void ModelFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_RETURN_INTEGER(vehicle->GetHandling()->GetModelFlags());
}

static void ModelFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_INTEGER(val, ivalue);
	vehicle->GetHandling()->SetModelFlags(ivalue);
}

static void HandlingFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_INTEGER(vehicle->GetHandling()->GetHandlingFlags());
}

static void HandlingFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_INTEGER(val, ivalue);
	vehicle->GetHandling()->SetHandlingFlags(ivalue);
}

static void DamageFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
	V8_RETURN_INTEGER(vehicle->GetHandling()->GetDamageFlags());
}

static void DamageFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

	V8_TO_INTEGER(val, ivalue);
	vehicle->GetHandling()->SetDamageFlags(ivalue);
}

extern V8Class v8Handling("Handling", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();

		v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		V8::SetMethod(isolate, tpl, "isModified", &IsModified);
		V8::SetMethod(isolate, tpl, "reset", &Reset);

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
		V8::SetAccessor(isolate, tpl, "camberStiffness", &CamberStiffnessGetter, &CamberStiffnessSetter);
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