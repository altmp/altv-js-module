#include "../CV8Resource.h"
#include "../helpers/V8Helpers.h"
#include "cpp-sdk/script-objects/IBlip.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK(false, "You can't use constructor of abstract class");
}

static void ConstructorAreaBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(5);
	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, width);
	V8_ARG_TO_NUMBER(5, height);

	alt::IResource* res = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
    alt::Ref<alt::IBlip> blip = res->CreateBlip({ x, y, z }, width, height);
	V8_BIND_BASE_OBJECT(blip);
}

static void ConstructorRadiusBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(4);
	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);
	V8_ARG_TO_NUMBER(4, radius);

	alt::IResource* res = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
    alt::Ref<alt::IBlip> blip = res->CreateBlip({ x, y, z }, radius);
	V8_BIND_BASE_OBJECT(blip);
}

static void ConstructorPointBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(3);
	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);

	alt::IResource* res = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
    alt::Ref<alt::IBlip> blip = res->CreateBlip(alt::IBlip::BlipType::DESTINATION, { x, y, z });
	V8_BIND_BASE_OBJECT(blip);
}

static void ConstructorPedBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, pedId);

	alt::IResource* res = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
    alt::Ref<alt::IBlip> blip = res->CreateBlip(alt::IBlip::BlipType::PED, pedId);
	V8_BIND_BASE_OBJECT(blip);
}

static void ConstructorVehicleBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_INTEGER(1, vehicleId);

	alt::IResource* res = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
    alt::Ref<alt::IBlip> blip = res->CreateBlip(alt::IBlip::BlipType::VEHICLE, vehicleId);


	V8_BIND_BASE_OBJECT(blip);
}

static void SizeSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);

	V8_TO_OBJECT(value, pos);
	V8_OBJECT_GET_NUMBER(pos, "x", x);
	V8_OBJECT_GET_NUMBER(pos, "y", y);

	blip->SetScaleXY(x, y);
}

static void SizeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);

	alt::Vector2f blipPos = blip->GetScaleXY();

	V8_NEW_OBJECT(pos);
	V8_OBJECT_SET_NUMBER(pos, "x", blipPos[0]);
	V8_OBJECT_SET_NUMBER(pos, "y", blipPos[1]);

	V8_RETURN(pos);
}

static void SpriteGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetSprite());
}

static void SpriteSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetSprite(val);
}

static void ColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetColor());
}

static void ColorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetColor(val);
}

static void SecondaryColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN(resource->CreateRGBA(blip->GetSecondaryColor()));
}

static void SecondaryColorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);

	V8_TO_OBJECT(value, color);
	V8_OBJECT_GET_INTEGER(color, "r", r);
	V8_OBJECT_GET_INTEGER(color, "g", g);
	V8_OBJECT_GET_INTEGER(color, "b", b);

	blip->SetSecondaryColor({ (uint8_t)r, (uint8_t)g, (uint8_t)b, 255 });
}

static void AlphaGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetAlpha());
}

static void AlphaSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetAlpha(val);
}

static void FlashTimerGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetFlashTimer());
}

static void FlashTimerSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetFlashTimer(val);
}

static void FlashIntervalGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetFlashInterval());
}

static void FlashIntervalSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetFlashInterval(val);
}

static void FriendlyGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetAsFriendly());
}

static void FriendlySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetAsFriendly(val);
}

static void RouteGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetRoute());
}

static void RouteSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetRoute(val);
}

static void BrightGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetBright());
}

static void BrightSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetBright(val);
}

static void NumberGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetNumber());
}

static void NumberSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
	blip->SetNumber(val);
}

static void DisplayGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetDisplay());
}

static void DisplaySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
	blip->SetDisplay(val);
}

static void ShowConeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetShowCone());
}

static void ShowConeSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetShowCone(val);
}

static void FlashesGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetFlashes());
}

static void FlashesSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetFlashes(val);
}

static void FlashesAlternateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetFlashesAlternate());
}

static void FlashesAlternateSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetFlashesAlternate(val);
}

static void ShortRangeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetAsShortRange());
}

static void ShortRangeSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetAsShortRange(val);
}

static void PriorityGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetPriority());
}

static void PrioritySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetPriority(val);
}

static void RotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_NUMBER(blip->GetRotation());
}

static void RotationSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_NUMBER(value, val);
    blip->SetRotation(val);
}

static void GxtNameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_STRING(blip->GetGxtName());
}

static void GxtNameSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_STRING(value, val);
    blip->SetGxtName(val.CStr());
}

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_STRING(blip->GetName());
}

static void NameSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_STRING(value, val);
	blip->SetName(val.CStr());
}

static void RouteColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN(resource->CreateRGBA(blip->GetRouteColor()));
}

static void RouteColorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);

	V8_TO_OBJECT(value, color);
	V8_OBJECT_GET_INTEGER(color, "r", r);
	V8_OBJECT_GET_INTEGER(color, "g", g);
	V8_OBJECT_GET_INTEGER(color, "b", b);
	V8_OBJECT_GET_INTEGER(color, "a", a);

	blip->SetRouteColor({ (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a });
}

static void PulseGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetPulse());
}

static void PulseSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetPulse(val);
}

static void AsMissionCreatorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetAsMissionCreator());
}

static void AsMissionCreatorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetAsMissionCreator(val);
}

static void TickVisibleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetTickVisible());
}

static void TickVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetTickVisible(val);
}

static void HeadingIndicatorVisibleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetHeadingIndicatorVisible());
}

static void HeadingIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetHeadingIndicatorVisible(val);
}

static void OutlineIndicatorVisibleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetOutlineIndicatorVisible());
}

static void OutlineIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetOutlineIndicatorVisible(val);
}

static void FriendIndicatorVisibleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetFriendIndicatorVisible());
}

static void FriendIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetFriendIndicatorVisible(val);
}

static void CrewIndicatorVisibleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetCrewIndicatorVisible());
}

static void CrewIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetCrewIndicatorVisible(val);
}

static void CategoryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_INTEGER(blip->GetCategory());
}

static void CategorySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_INTEGER(value, val);
    blip->SetCategory(val);
}

static void HighDetailGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetAsHighDetail());
}

static void HighDetailSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetAsHighDetail(val);
}

static void ShrinkedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_RETURN_BOOLEAN(blip->GetShrinked());
}

static void ShrinkedSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
	V8_TO_BOOLEAN(value, val);
    blip->SetShrinked(val);
}

static void Fade(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_INTEGER(1, opacity);
	V8_ARG_TO_INTEGER(2, duration);
	V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    blip->Fade(opacity, duration);
}

extern V8Class v8WorldObject;
extern V8Class v8Blip("Blip", v8WorldObject, Constructor, [](v8::Local<v8::FunctionTemplate> tpl){
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetStaticAccessor(isolate, tpl, "routeColor", &RouteColorGetter, &RouteColorSetter);

	V8::SetAccessor(isolate, tpl, "sprite", &SpriteGetter, &SpriteSetter);
	V8::SetAccessor(isolate, tpl, "size", &SizeGetter, &SizeSetter);
	V8::SetAccessor(isolate, tpl, "color", &ColorGetter, &ColorSetter);
	V8::SetAccessor(isolate, tpl, "secondaryColor", &SecondaryColorGetter, &SecondaryColorSetter);
	V8::SetAccessor(isolate, tpl, "alpha", &AlphaGetter, &AlphaSetter);
	V8::SetAccessor(isolate, tpl, "flashTimer", &FlashTimerGetter, &FlashTimerSetter);
	V8::SetAccessor(isolate, tpl, "flashInterval", &FlashIntervalGetter, &FlashIntervalSetter);
	V8::SetAccessor(isolate, tpl, "route", &RouteGetter, &RouteSetter);
	V8::SetAccessor(isolate, tpl, "bright", &BrightGetter, &BrightSetter);
	V8::SetAccessor(isolate, tpl, "number", &NumberGetter, &NumberSetter);
	V8::SetAccessor(isolate, tpl, "display", &DisplayGetter, &DisplaySetter);
	V8::SetAccessor(isolate, tpl, "showCone", &ShowConeGetter, &ShowConeSetter);
	V8::SetAccessor(isolate, tpl, "flashes", &FlashesGetter, &FlashesSetter);
	V8::SetAccessor(isolate, tpl, "flashesAlternate", &FlashesAlternateGetter, &FlashesAlternateSetter);
	V8::SetAccessor(isolate, tpl, "shortRange", &ShortRangeGetter, &ShortRangeSetter);
	V8::SetAccessor(isolate, tpl, "priority", &PriorityGetter, &PrioritySetter);
	V8::SetAccessor(isolate, tpl, "heading", &RotationGetter, &RotationSetter);
	V8::SetAccessor(isolate, tpl, "gxtName", &GxtNameGetter, &GxtNameSetter);
	V8::SetAccessor(isolate, tpl, "name", &NameGetter, &NameSetter);
	V8::SetAccessor(isolate, tpl, "pulse", &PulseGetter, &PulseSetter);
	V8::SetAccessor(isolate, tpl, "asMissionCreator", &AsMissionCreatorGetter, &AsMissionCreatorSetter);
	V8::SetAccessor(isolate, tpl, "tickVisible", &TickVisibleGetter, &TickVisibleSetter);
	V8::SetAccessor(isolate, tpl, "headingIndicatorVisible", &HeadingIndicatorVisibleGetter, &HeadingIndicatorVisibleSetter);
	V8::SetAccessor(isolate, tpl, "outlineIndicatorVisible", &OutlineIndicatorVisibleGetter, &OutlineIndicatorVisibleSetter);
	V8::SetAccessor(isolate, tpl, "friendIndicatorVisible", &FriendIndicatorVisibleGetter, &FriendIndicatorVisibleSetter);
	V8::SetAccessor(isolate, tpl, "crewIndicatorVisible", &CrewIndicatorVisibleGetter, &CrewIndicatorVisibleSetter);
	V8::SetAccessor(isolate, tpl, "category", &CategoryGetter, &CategorySetter);
	V8::SetAccessor(isolate, tpl, "highDetail", &HighDetailGetter, &HighDetailSetter);
	V8::SetAccessor(isolate, tpl, "shrinked", &ShrinkedGetter, &ShrinkedSetter);

	V8::SetMethod(isolate, tpl, "fade", &Fade);
});

extern V8Class v8AreaBlip("AreaBlip", v8Blip, ConstructorAreaBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
});

extern V8Class v8RadiusBlip("RadiusBlip", v8Blip, ConstructorRadiusBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
});

extern V8Class v8PointBlip("PointBlip", v8Blip, ConstructorPointBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
});

extern V8Class v8PedBlip("PedBlip", v8Blip, ConstructorPedBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
});

extern V8Class v8VehicleBlip("VehicleBlip", v8Blip, ConstructorVehicleBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
});
