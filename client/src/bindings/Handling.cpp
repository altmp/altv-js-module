
#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "cpp-sdk/objects/IVehicle.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_OBJECT(1, vehicle);

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::HANDLING);
    info.This()->SetInternalField(1, vehicle);
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
    V8_RETURN_UINT(vehicle->GetHandling()->GetHandlingNameHash());
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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

    vehicle->ReplaceHandling();
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

    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
    vehicle->GetHandling()->SetAcceleration(fvalue);
}

static void InitialDriveGearsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetHandling()->GetInitialDriveGears());
}

static void InitialDriveGearsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, ivalue);
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
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
    vehicle->ReplaceHandling();
    vehicle->GetHandling()->SetSeatOffsetDistZ(fvalue);
}

static void MonetaryValueGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetHandling()->GetMonetaryValue());
}

static void MonetaryValueSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, ivalue);
    vehicle->ReplaceHandling();
    vehicle->GetHandling()->SetMonetaryValue(ivalue);
}

static void ModelFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_RETURN_UINT(vehicle->GetHandling()->GetModelFlags());
}

static void ModelFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, ivalue);
    vehicle->ReplaceHandling();
    vehicle->GetHandling()->SetModelFlags(ivalue);
}

static void HandlingFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
    V8_RETURN_UINT(vehicle->GetHandling()->GetHandlingFlags());
}

static void HandlingFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, ivalue);
    vehicle->ReplaceHandling();
    vehicle->GetHandling()->SetHandlingFlags(ivalue);
}

static void DamageFlagsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);
    V8_RETURN_UINT(vehicle->GetHandling()->GetDamageFlags());
}

static void DamageFlagsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_ENTITY(1, vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, ivalue);
    vehicle->ReplaceHandling();
    vehicle->GetHandling()->SetDamageFlags(ivalue);
}

extern V8Class v8Handling("Handling", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    tpl->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(V8Class::InternalFields::COUNT));

    V8Helpers::SetMethod(isolate, tpl, "isModified", &IsModified);
    V8Helpers::SetMethod(isolate, tpl, "reset", &Reset);

    V8Helpers::SetAccessor(isolate, tpl, "handlingNameHash", &HandlingNameHashGetter);
    V8Helpers::SetAccessor(isolate, tpl, "mass", &MassGetter, &MassSetter);
    V8Helpers::SetAccessor(isolate, tpl, "initialDragCoeff", &InitialDragCoeffGetter, &InitialDragCoeffSetter);
    V8Helpers::SetAccessor(isolate, tpl, "downforceModifier", &DownforceModifierGetter, &DownforceModifierSetter);
    V8Helpers::SetAccessor(isolate, tpl, "unkFloat1", &unkFloat1Getter, &unkFloat1Setter);
    V8Helpers::SetAccessor(isolate, tpl, "unkFloat2", &unkFloat2Getter, &unkFloat2Setter);
    V8Helpers::SetAccessor(isolate, tpl, "centreOfMassOffset", &CentreOfMassOffsetGetter, &CentreOfMassOffsetSetter);
    V8Helpers::SetAccessor(isolate, tpl, "inertiaMultiplier", &InertiaMultiplierGetter, &InertiaMultiplierSetter);
    V8Helpers::SetAccessor(isolate, tpl, "percentSubmerged", &PercentSubmergedGetter, &PercentSubmergedSetter);
    V8Helpers::SetAccessor(isolate, tpl, "percentSubmergedRatio", &PercentSubmergedRatioGetter, &PercentSubmergedRatioSetter);
    V8Helpers::SetAccessor(isolate, tpl, "driveBiasFront", &DriveBiasFrontGetter, &DriveBiasFrontSetter);
    V8Helpers::SetAccessor(isolate, tpl, "acceleration", &AccelerationGetter, &AccelerationSetter);
    V8Helpers::SetAccessor(isolate, tpl, "initialDriveGears", &InitialDriveGearsGetter, &InitialDriveGearsSetter);
    V8Helpers::SetAccessor(isolate, tpl, "driveInertia", &DriveInertiaGetter, &DriveInertiaSetter);
    V8Helpers::SetAccessor(isolate, tpl, "clutchChangeRateScaleUpShift", &ClutchChangeRateScaleUpShiftGetter, &ClutchChangeRateScaleUpShiftSetter);
    V8Helpers::SetAccessor(isolate, tpl, "clutchChangeRateScaleDownShift", &ClutchChangeRateScaleDownShiftGetter, &ClutchChangeRateScaleDownShiftSetter);
    V8Helpers::SetAccessor(isolate, tpl, "initialDriveForce", &InitialDriveForceGetter, &InitialDriveForceSetter);
    V8Helpers::SetAccessor(isolate, tpl, "driveMaxFlatVel", &DriveMaxFlatVelGetter, &DriveMaxFlatVelSetter);
    V8Helpers::SetAccessor(isolate, tpl, "initialDriveMaxFlatVel", &InitialDriveMaxFlatVelGetter, &InitialDriveMaxFlatVelSetter);
    V8Helpers::SetAccessor(isolate, tpl, "brakeForce", &BrakeForceGetter, &BrakeForceSetter);
    V8Helpers::SetAccessor(isolate, tpl, "unkFloat4", &unkFloat4Getter, &unkFloat4Setter);
    V8Helpers::SetAccessor(isolate, tpl, "brakeBiasFront", &BrakeBiasFrontGetter, &BrakeBiasFrontSetter);
    V8Helpers::SetAccessor(isolate, tpl, "brakeBiasRear", &BrakeBiasRearGetter, &BrakeBiasRearSetter);
    V8Helpers::SetAccessor(isolate, tpl, "handBrakeForce", &HandBrakeForceGetter, &HandBrakeForceSetter);
    V8Helpers::SetAccessor(isolate, tpl, "steeringLock", &SteeringLockGetter, &SteeringLockSetter);
    V8Helpers::SetAccessor(isolate, tpl, "steeringLockRatio", &SteeringLockRatioGetter, &SteeringLockRatioSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionCurveMax", &TractionCurveMaxGetter, &TractionCurveMaxSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionCurveMaxRatio", &TractionCurveMaxRatioGetter, &TractionCurveMaxRatioSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionCurveMin", &TractionCurveMinGetter, &TractionCurveMinSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionCurveMinRatio", &TractionCurveMinRatioGetter, &TractionCurveMinRatioSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionCurveLateral", &TractionCurveLateralGetter, &TractionCurveLateralSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionCurveLateralRatio", &TractionCurveLateralRatioGetter, &TractionCurveLateralRatioSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionSpringDeltaMax", &TractionSpringDeltaMaxGetter, &TractionSpringDeltaMaxSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionSpringDeltaMaxRatio", &TractionSpringDeltaMaxRatioGetter, &TractionSpringDeltaMaxRatioSetter);
    V8Helpers::SetAccessor(isolate, tpl, "lowSpeedTractionLossMult", &LowSpeedTractionLossMultGetter, &LowSpeedTractionLossMultSetter);
    V8Helpers::SetAccessor(isolate, tpl, "camberStiffness", &CamberStiffnessGetter, &CamberStiffnessSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionBiasFront", &TractionBiasFrontGetter, &TractionBiasFrontSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionBiasRear", &TractionBiasRearGetter, &TractionBiasRearSetter);
    V8Helpers::SetAccessor(isolate, tpl, "tractionLossMult", &TractionLossMultGetter, &TractionLossMultSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionForce", &SuspensionForceGetter, &SuspensionForceSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionCompDamp", &SuspensionCompDampGetter, &SuspensionCompDampSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionReboundDamp", &SuspensionReboundDampGetter, &SuspensionReboundDampSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionUpperLimit", &SuspensionUpperLimitGetter, &SuspensionUpperLimitSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionLowerLimit", &SuspensionLowerLimitGetter, &SuspensionLowerLimitSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionRaise", &SuspensionRaiseGetter, &SuspensionRaiseSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionBiasFront", &SuspensionBiasFrontGetter, &SuspensionBiasFrontSetter);
    V8Helpers::SetAccessor(isolate, tpl, "suspensionBiasRear", &SuspensionBiasRearGetter, &SuspensionBiasRearSetter);
    V8Helpers::SetAccessor(isolate, tpl, "antiRollBarForce", &AntiRollBarForceGetter, &AntiRollBarForceSetter);
    V8Helpers::SetAccessor(isolate, tpl, "antiRollBarBiasFront", &AntiRollBarBiasFrontGetter, &AntiRollBarBiasFrontSetter);
    V8Helpers::SetAccessor(isolate, tpl, "antiRollBarBiasRear", &AntiRollBarBiasRearGetter, &AntiRollBarBiasRearSetter);
    V8Helpers::SetAccessor(isolate, tpl, "rollCentreHeightFront", &RollCentreHeightFrontGetter, &RollCentreHeightFrontSetter);
    V8Helpers::SetAccessor(isolate, tpl, "rollCentreHeightRear", &RollCentreHeightRearGetter, &RollCentreHeightRearSetter);
    V8Helpers::SetAccessor(isolate, tpl, "collisionDamageMult", &CollisionDamageMultGetter, &CollisionDamageMultSetter);
    V8Helpers::SetAccessor(isolate, tpl, "weaponDamageMult", &WeaponDamageMultGetter, &WeaponDamageMultSetter);
    V8Helpers::SetAccessor(isolate, tpl, "deformationDamageMult", &DeformationDamageMultGetter, &DeformationDamageMultSetter);
    V8Helpers::SetAccessor(isolate, tpl, "engineDamageMult", &EngineDamageMultGetter, &EngineDamageMultSetter);
    V8Helpers::SetAccessor(isolate, tpl, "petrolTankVolume", &PetrolTankVolumeGetter, &PetrolTankVolumeSetter);
    V8Helpers::SetAccessor(isolate, tpl, "oilVolume", &OilVolumeGetter, &OilVolumeSetter);
    V8Helpers::SetAccessor(isolate, tpl, "unkFloat5", &unkFloat5Getter, &unkFloat5Setter);
    V8Helpers::SetAccessor(isolate, tpl, "seatOffsetDistX", &SeatOffsetDistXGetter, &SeatOffsetDistXSetter);
    V8Helpers::SetAccessor(isolate, tpl, "seatOffsetDistY", &SeatOffsetDistYGetter, &SeatOffsetDistYSetter);
    V8Helpers::SetAccessor(isolate, tpl, "seatOffsetDistZ", &SeatOffsetDistZGetter, &SeatOffsetDistZSetter);
    V8Helpers::SetAccessor(isolate, tpl, "monetaryValue", &MonetaryValueGetter, &MonetaryValueSetter);
    V8Helpers::SetAccessor(isolate, tpl, "modelFlags", &ModelFlagsGetter, &ModelFlagsSetter);
    V8Helpers::SetAccessor(isolate, tpl, "handlingFlags", &HandlingFlagsGetter, &HandlingFlagsSetter);
    V8Helpers::SetAccessor(isolate, tpl, "damageFlags", &DamageFlagsGetter, &DamageFlagsSetter);
});
