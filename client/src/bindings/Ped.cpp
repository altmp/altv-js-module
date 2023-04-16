#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllPeds());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetPeds().size());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IEntity* entity = alt::ICore::Instance().GetEntityByID(id);

    if(entity && entity->GetType() == alt::IEntity::Type::PED)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

// clang-format off
extern V8Class v8Entity;
extern V8Class v8Ped("Ped", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);
    V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

    V8Helpers::SetAccessor<IPed, uint32_t, &IPed::GetCurrentWeapon>(isolate, tpl, "currentWeapon");
    V8Helpers::SetAccessor<IPed, uint16_t, &IPed::GetHealth>(isolate, tpl, "health");
    V8Helpers::SetAccessor<IPed, uint16_t, &IPed::GetMaxHealth>(isolate, tpl, "maxHealth");
    V8Helpers::SetAccessor<IPed, uint16_t, &IPed::GetArmour>(isolate, tpl, "armour");
});
