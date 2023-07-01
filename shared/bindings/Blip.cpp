#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "cpp-sdk/script-objects/IBlip.h"

using namespace alt;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);

    std::ostringstream ss;
    ss << "Blip{ name: " << blip->GetName() << ", category: " << blip->GetCategory() << " }";

    V8_RETURN_STRING(ss.str());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK(false, "You can't use constructor of abstract class");
}

#ifdef ALT_SERVER_API
static void ConstructorAreaBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(6);
    V8_ARG_TO_NUMBER(1, x);
    V8_ARG_TO_NUMBER(2, y);
    V8_ARG_TO_NUMBER(3, z);
    V8_ARG_TO_NUMBER(4, width);
    V8_ARG_TO_NUMBER(5, height);
    V8_ARG_TO_BOOLEAN(6, global);

    alt::IBlip* blip = ICore::Instance().CreateBlip(global, alt::IBlip::BlipType::AREA, { x, y, z });
    V8_BIND_BASE_OBJECT(blip, "Failed to create AreaBlip");
    blip->SetScaleXY({ width, height });
}
#endif

#ifdef ALT_CLIENT_API
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

    alt::IBlip* blip = alt::ICore::Instance().CreateBlip({ x, y, z }, width, height, resource->GetResource());
    V8_BIND_BASE_OBJECT(blip, "Failed to create AreaBlip");
}
#endif

#ifdef ALT_SERVER_API
static void ConstructorRadiusBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(5);
    V8_ARG_TO_NUMBER(1, x);
    V8_ARG_TO_NUMBER(2, y);
    V8_ARG_TO_NUMBER(3, z);
    V8_ARG_TO_NUMBER(4, radius);
    V8_ARG_TO_BOOLEAN(5, global);

    alt::IBlip* blip = ICore::Instance().CreateBlip(global, alt::IBlip::BlipType::RADIUS, { x, y, z });
    V8_BIND_BASE_OBJECT(blip, "Failed to create RadiusBlip");
    blip->SetScaleXY({ radius, radius });
}
#endif

#ifdef ALT_CLIENT_API
static void ConstructorRadiusBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(4);
    V8_ARG_TO_NUMBER(1, x);
    V8_ARG_TO_NUMBER(2, y);
    V8_ARG_TO_NUMBER(3, z);
    V8_ARG_TO_NUMBER(4, radius);

    alt::IBlip* blip = alt::ICore::Instance().CreateBlip({ x, y, z }, radius, resource->GetResource());
    V8_BIND_BASE_OBJECT(blip, "Failed to create RadiusBlip");
}
#endif

#ifdef ALT_SERVER_API
static void ConstructorPointBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(2, 4);

    IBlip* blip;

    if(info.Length() == 4)
    {
        V8_ARG_TO_NUMBER(1, x);
        V8_ARG_TO_NUMBER(2, y);
        V8_ARG_TO_NUMBER(3, z);
        V8_ARG_TO_BOOLEAN(4, global);

        blip = ICore::Instance().CreateBlip(global, alt::IBlip::BlipType::DESTINATION, { x, y, z });
    }

    else if(info.Length() == 2)
    {
        if(resource->IsVector3(info[0]))
        {
            V8_ARG_TO_VECTOR3(1, pos);
            V8_ARG_TO_BOOLEAN(2, global);
            blip = ICore::Instance().CreateBlip(global, alt::IBlip::BlipType::DESTINATION, pos);
        }
        else if(resource->IsBaseObject(info[0]))
        {
            V8_ARG_TO_BASE_OBJECT(1, ent, IEntity, "entity");
            V8_ARG_TO_BOOLEAN(2, global);
            blip = ICore::Instance().CreateBlip(global, alt::IBlip::BlipType::DESTINATION, ent);
        }
        else
        {
            V8_CHECK(false, "The PointBlip argument must be a Vector3 or an Entity");
        }
    }

    V8_BIND_BASE_OBJECT(blip, "Failed to create PointBlip");
}
#endif

#ifdef ALT_CLIENT_API
static void ConstructorPointBlip(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(3);
    V8_ARG_TO_NUMBER(1, x);
    V8_ARG_TO_NUMBER(2, y);
    V8_ARG_TO_NUMBER(3, z);

    alt::IBlip* blip = alt::ICore::Instance().CreateBlip(alt::IBlip::BlipType::DESTINATION, { x, y, z }, resource->GetResource());
    V8_BIND_BASE_OBJECT(blip, "Failed to create PointBlip");
}
#endif

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

static void AttachedToGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_RETURN_BASE_OBJECT(blip->AttachedTo());
}

static void AttachedToSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_TO_ENTITY(value, val);
    blip->AttachTo(val);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllBlips()->Clone());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBlips().size());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::BLIP, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::BLIP)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

#ifdef ALT_CLIENT_API
static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);

    alt::IBlip* blip = alt::ICore::Instance().GetBlipByGameID(scriptGuid);

    if(blip)
    {
        V8_RETURN_BASE_OBJECT(blip);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByRemoteId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT32(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByRemoteID(alt::IBaseObject::Type::BLIP, id);

    if(entity)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}
#endif

static void TypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_RETURN_NUMBER(blip->GetBlipType());
}

static void TypeSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_TO_NUMBER(value, val);
    blip->SetBlipType((IBlip::BlipType)val);
}

#ifdef ALT_SERVER_API
static void AddTargetPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, player, alt::IPlayer, "Player");

    blip->AddTargetPlayer(player);
}
static void RemoveTargetPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, player, alt::IPlayer, "Player");

    blip->RemoveTargetPlayer(player);
}

static void GetTargets(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(blip, alt::IBlip);

    std::vector<IPlayer*> targets = blip->GetTargets();
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, targets.size());
    for(uint32_t i = 0; i < targets.size(); ++i) jsAll->Set(resource->GetContext(), i, resource->GetBaseObjectOrNull(targets[i]));

    V8_RETURN(jsAll);
}
#endif

extern V8Class v8WorldObject;
extern V8Class v8Blip("Blip",
                      v8WorldObject,
                      Constructor,
                      [](v8::Local<v8::FunctionTemplate> tpl)
                      {
                          v8::Isolate* isolate = v8::Isolate::GetCurrent();

                          V8Helpers::SetMethod(isolate, tpl, "toString", ToString);
                          V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                          V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

                          V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
#ifdef ALT_CLIENT_API
                          V8Helpers::SetStaticMethod(isolate, tpl, "getByRemoteID", StaticGetByRemoteId);
#endif

                          V8Helpers::SetAccessor<IBlip, RGBA, &IBlip::GetRouteColor, &IBlip::SetRouteColor>(isolate, tpl, "routeColor");
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetSprite, &IBlip::SetSprite>(isolate, tpl, "sprite");
                          V8Helpers::SetAccessor<IBlip, Vector2f, &IBlip::GetScaleXY, &IBlip::SetScaleXY>(isolate, tpl, "size");
                          V8Helpers::SetAccessor(isolate, tpl, "scale", &ScaleGetter, &ScaleSetter);
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetColor, &IBlip::SetColor>(isolate, tpl, "color");
                          V8Helpers::SetAccessor<IBlip, RGBA, &IBlip::GetSecondaryColor, &IBlip::SetSecondaryColor>(isolate, tpl, "secondaryColor");
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetAlpha, &IBlip::SetAlpha>(isolate, tpl, "alpha");
                          V8Helpers::SetAccessor<IBlip, int32_t, &IBlip::GetFlashTimer, &IBlip::SetFlashTimer>(isolate, tpl, "flashTimer");
                          V8Helpers::SetAccessor<IBlip, int32_t, &IBlip::GetFlashInterval, &IBlip::SetFlashInterval>(isolate, tpl, "flashInterval");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetRoute, &IBlip::SetRoute>(isolate, tpl, "route");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetBright, &IBlip::SetBright>(isolate, tpl, "bright");
                          V8Helpers::SetAccessor<IBlip, int32_t, &IBlip::GetNumber, &IBlip::SetNumber>(isolate, tpl, "number");
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetDisplay, &IBlip::SetDisplay>(isolate, tpl, "display");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetShowCone, &IBlip::SetShowCone>(isolate, tpl, "showCone");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetFlashes, &IBlip::SetFlashes>(isolate, tpl, "flashes");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetFlashesAlternate, &IBlip::SetFlashesAlternate>(isolate, tpl, "flashesAlternate");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetAsShortRange, &IBlip::SetAsShortRange>(isolate, tpl, "shortRange");
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetPriority, &IBlip::SetPriority>(isolate, tpl, "priority");
                          V8Helpers::SetAccessor<IBlip, float, &IBlip::GetRotation, &IBlip::SetRotation>(isolate, tpl, "heading");
                          V8Helpers::SetAccessor<IBlip, std::string, &IBlip::GetGxtName, const std::string&, &IBlip::SetGxtName>(isolate, tpl, "gxtName");
                          V8Helpers::SetAccessor<IBlip, std::string, &IBlip::GetName, const std::string&, &IBlip::SetName>(isolate, tpl, "name");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetPulse, &IBlip::SetPulse>(isolate, tpl, "pulse");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetAsMissionCreator, &IBlip::SetAsMissionCreator>(isolate, tpl, "asMissionCreator");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetTickVisible, &IBlip::SetTickVisible>(isolate, tpl, "tickVisible");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetHeadingIndicatorVisible, &IBlip::SetHeadingIndicatorVisible>(isolate, tpl, "headingIndicatorVisible");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetOutlineIndicatorVisible, &IBlip::SetOutlineIndicatorVisible>(isolate, tpl, "outlineIndicatorVisible");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetFriendIndicatorVisible, &IBlip::SetFriendIndicatorVisible>(isolate, tpl, "friendIndicatorVisible");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetCrewIndicatorVisible, &IBlip::SetCrewIndicatorVisible>(isolate, tpl, "crewIndicatorVisible");
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetCategory, &IBlip::SetCategory>(isolate, tpl, "category");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetAsHighDetail, &IBlip::SetAsHighDetail>(isolate, tpl, "highDetail");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetShrinked, &IBlip::SetShrinked>(isolate, tpl, "shrinked");
                          V8Helpers::SetAccessor(isolate, tpl, "blipType", &TypeGetter, &TypeSetter);
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::IsAttached>(isolate, tpl, "isAttached");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::GetAsFriendly, &IBlip::SetAsFriendly>(isolate, tpl, "isFriendly");

#ifdef ALT_CLIENT_API
                          V8Helpers::SetAccessor<IBlip, uint32_t, &IBlip::GetGameID>(isolate, tpl, "scriptID");
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::IsStreamedIn>(isolate, tpl, "isStreamedIn");
                          V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::IsGlobal>(isolate, tpl, "isGlobal");
#endif

#ifdef ALT_SERVER_API
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::IsGlobal, &IBlip::SetGlobal>(isolate, tpl, "isGlobal");
                          V8Helpers::SetStaticAccessor(isolate, tpl, "targets", &GetTargets);
                          V8Helpers::SetMethod(isolate, tpl, "addTarget", &AddTargetPlayer);
                          V8Helpers::SetMethod(isolate, tpl, "removeTarget", &RemoveTargetPlayer);
#endif

                          V8Helpers::SetAccessor(isolate, tpl, "attachedTo", &AttachedToGetter, &AttachedToSetter);

                          V8Helpers::SetMethod(isolate, tpl, "fade", &Fade);
                          V8Helpers::SetAccessor<IBlip, bool, &IBlip::IsVisible, &IBlip::SetVisible>(isolate, tpl, "visible");
                      });

extern V8Class v8AreaBlip("AreaBlip",
                          v8Blip,
                          ConstructorAreaBlip,
                          [](v8::Local<v8::FunctionTemplate> tpl)
                          {
                              v8::Isolate* isolate = v8::Isolate::GetCurrent();
                              v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
                          });

extern V8Class v8RadiusBlip("RadiusBlip",
                            v8Blip,
                            ConstructorRadiusBlip,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();
                                v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
                            });

extern V8Class v8PointBlip("PointBlip",
                           v8Blip,
                           ConstructorPointBlip,
                           [](v8::Local<v8::FunctionTemplate> tpl)
                           {
                               v8::Isolate* isolate = v8::Isolate::GetCurrent();
                               v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
                           });
