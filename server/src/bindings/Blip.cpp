#include "stdafx.h"

#include "V8Helpers.h"
#include "V8BindHelpers.h"
#include "V8ResourceImpl.h"

using namespace alt;

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

    alt::Ref<alt::IBlip> blip = ICore::Instance().CreateBlip(nullptr, alt::IBlip::BlipType::AREA, { x, y, z });
    V8_BIND_BASE_OBJECT(blip, "Failed to create AreaBlip");
    blip->SetScaleXY({ width, height });
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

    alt::Ref<alt::IBlip> blip = ICore::Instance().CreateBlip(nullptr, alt::IBlip::BlipType::RADIUS, { x, y, z });
    V8_BIND_BASE_OBJECT(blip, "Failed to create RadiusBlip");
    blip->SetScaleXY({ radius, radius });
}

static void ConstructorPointBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    Ref<IBlip> blip;

    if (info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x);
        V8_ARG_TO_NUMBER(2, y);
        V8_ARG_TO_NUMBER(3, z);

        blip = ICore::Instance().CreateBlip(nullptr, alt::IBlip::BlipType::DESTINATION, { x, y, z });
    }

    if (info.Length() == 1)
    {
        V8_ARG_TO_BASE_OBJECT(1, ent, IEntity, "entity");
        blip = ICore::Instance().CreateBlip(nullptr, alt::IBlip::BlipType::DESTINATION, ent);
    }    

    V8_BIND_BASE_OBJECT(blip, "Failed to create PointBlip");
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
    V8_OBJECT_GET_INT(color, "r", r);
    V8_OBJECT_GET_INT(color, "g", g);
    V8_OBJECT_GET_INT(color, "b", b);
    V8_OBJECT_GET_INT(color, "a", a);

    blip->SetRouteColor({ (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a });
}

static void ScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_RETURN_NUMBER(blip->GetScaleXY()[0]);
}

static void ScaleSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_TO_NUMBER(value, val);
    blip->SetScaleXY({ val, val });
}

static void Fade(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, opacity);
    V8_ARG_TO_INT(2, duration);
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    blip->Fade(opacity, duration);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllBlips()->Clone());
}

extern V8Class v8WorldObject;
extern V8Class v8Blip("Blip", v8WorldObject, Constructor, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8::SetStaticAccessor(isolate, tpl, "routeColor", &RouteColorGetter, &RouteColorSetter);

    V8::SetAccessor<IBlip, int32_t, &IBlip::GetSprite, &IBlip::SetSprite>(isolate, tpl, "sprite");
    V8::SetAccessor<IBlip, Vector2f, &IBlip::GetScaleXY, &IBlip::SetScaleXY>(isolate, tpl, "size");
    V8::SetAccessor(isolate, tpl, "scale", &ScaleGetter, &ScaleSetter);
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetColor, &IBlip::SetColor>(isolate, tpl, "color");
    V8::SetAccessor<IBlip, RGBA, &IBlip::GetSecondaryColor, &IBlip::SetSecondaryColor>(isolate, tpl, "secondaryColor");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetAlpha, &IBlip::SetAlpha>(isolate, tpl, "alpha");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetFlashTimer, &IBlip::SetFlashTimer>(isolate, tpl, "flashTimer");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetFlashInterval, &IBlip::SetFlashInterval>(isolate, tpl, "flashInterval");
    V8::SetAccessor<IBlip, bool, &IBlip::GetRoute, &IBlip::SetRoute>(isolate, tpl, "route");
    V8::SetAccessor<IBlip, bool, &IBlip::GetBright, &IBlip::SetBright>(isolate, tpl, "bright");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetNumber, &IBlip::SetNumber>(isolate, tpl, "number");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetDisplay, &IBlip::SetDisplay>(isolate, tpl, "display");
    V8::SetAccessor<IBlip, bool, &IBlip::GetShowCone, &IBlip::SetShowCone>(isolate, tpl, "showCone");
    V8::SetAccessor<IBlip, bool, &IBlip::GetFlashes, &IBlip::SetFlashes>(isolate, tpl, "flashes");
    V8::SetAccessor<IBlip, bool, &IBlip::GetFlashesAlternate, &IBlip::SetFlashesAlternate>(isolate, tpl, "flashesAlternate");
    V8::SetAccessor<IBlip, bool, &IBlip::GetAsShortRange, &IBlip::SetAsShortRange>(isolate, tpl, "shortRange");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetPriority, &IBlip::SetPriority>(isolate, tpl, "priority");
    V8::SetAccessor<IBlip, float, &IBlip::GetRotation, &IBlip::SetRotation>(isolate, tpl, "heading");
    V8::SetAccessor<IBlip, alt::String, &IBlip::GetGxtName, &IBlip::SetGxtName>(isolate, tpl, "gxtName");
    V8::SetAccessor<IBlip, alt::String, &IBlip::GetName, &IBlip::SetName>(isolate, tpl, "name");
    V8::SetAccessor<IBlip, bool, &IBlip::GetPulse, &IBlip::SetPulse>(isolate, tpl, "pulse");
    V8::SetAccessor<IBlip, bool, &IBlip::GetAsMissionCreator, &IBlip::SetAsMissionCreator>(isolate, tpl, "asMissionCreator");
    V8::SetAccessor<IBlip, bool, &IBlip::GetTickVisible, &IBlip::SetTickVisible>(isolate, tpl, "tickVisible");
    V8::SetAccessor<IBlip, bool, &IBlip::GetHeadingIndicatorVisible, &IBlip::SetHeadingIndicatorVisible>(isolate, tpl, "headingIndicatorVisible");
    V8::SetAccessor<IBlip, bool, &IBlip::GetOutlineIndicatorVisible, &IBlip::SetOutlineIndicatorVisible>(isolate, tpl, "outlineIndicatorVisible");
    V8::SetAccessor<IBlip, bool, &IBlip::GetFriendIndicatorVisible, &IBlip::SetFriendIndicatorVisible>(isolate, tpl, "friendIndicatorVisible");
    V8::SetAccessor<IBlip, bool, &IBlip::GetCrewIndicatorVisible, &IBlip::SetCrewIndicatorVisible>(isolate, tpl, "crewIndicatorVisible");
    V8::SetAccessor<IBlip, int32_t, &IBlip::GetCategory, &IBlip::SetCategory>(isolate, tpl, "category");
    V8::SetAccessor<IBlip, bool, &IBlip::GetAsHighDetail, &IBlip::SetAsHighDetail>(isolate, tpl, "highDetail");
    V8::SetAccessor<IBlip, bool, &IBlip::GetShrinked, &IBlip::SetShrinked>(isolate, tpl, "shrinked");
    V8::SetAccessor<IBlip, Ref<IEntity>, &IBlip::AttachedTo, &IBlip::AttachTo>(isolate, tpl, "attachedTo");

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
