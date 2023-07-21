#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/ILocalObject.h"

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);

    std::ostringstream ss;
    ss << "Object{ model: " << object->GetModel() << ", scriptID: " << object->GetScriptID() << " }";

    V8_RETURN_STRING(ss.str());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 7);

    uint32_t modelHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, modelName);
        modelHash = alt::ICore::Instance().Hash(modelName);
    }
    else
    {
        V8_ARG_TO_UINT(1, _modelHash);
        modelHash = _modelHash;
    }
    V8_ARG_TO_VECTOR3(2, pos);
    V8_ARG_TO_VECTOR3(3, rot);
    V8_ARG_TO_BOOLEAN_OPT(4, noOffset, false);
    V8_ARG_TO_BOOLEAN_OPT(5, dynamic, false);
    V8_ARG_TO_BOOLEAN_OPT(6, useStreaming, false);
    V8_ARG_TO_UINT_OPT(7, streamingDistance, 0);

    auto obj = alt::ICore::Instance().CreateLocalObject(modelHash, pos, rot, noOffset, dynamic, useStreaming, streamingDistance, resource->GetResource());
    V8_BIND_BASE_OBJECT(obj, "Failed to create object");
}

static void PosGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_RETURN_VECTOR3(object->GetPosition());
}

static void PosSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_TO_VECTOR3(value, val);
    object->SetPosition(val);
}

static void RotGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_RETURN_VECTOR3(object->GetRotation());
}

static void RotSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_TO_VECTOR3(value, val);
    object->SetRotation(val);
}

static void AttachToEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_CHECK_ARGS_LEN_MIN(4);

    V8_ARG_TO_INT(2, bone);
    V8_ARG_TO_VECTOR3(3, pos);
    V8_ARG_TO_VECTOR3(4, rot);
    V8_ARG_TO_BOOLEAN_OPT(5, useSoftPinning, false);
    V8_ARG_TO_BOOLEAN_OPT(6, collision, false);
    V8_ARG_TO_BOOLEAN_OPT(7, fixedRot, false);

    if(info[0]->IsUint32())
    {
        V8_ARG_TO_INT(1, scriptId);
        object->AttachToEntity(scriptId, bone, pos, rot, useSoftPinning, collision, fixedRot);
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, entity, alt::IEntity, "Entity");
        object->AttachToEntity(entity, bone, pos, rot, useSoftPinning, collision, fixedRot);
    }
}

static void Detach(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_ARG_TO_BOOLEAN_OPT(1, dynamic, false);

    object->Detach(dynamic);
}

static void ToggleCollision(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_BOOLEAN(1, toggle);
    V8_ARG_TO_BOOLEAN(2, keepPhysics);

    object->ToggleCollision(toggle, keepPhysics);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_RETURN(resource->GetAllLocalObjects());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::LOCAL_OBJECT).size());
}

static void AllWorldGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto objects = alt::ICore::Instance().GetWorldObjects();
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}

static void ModelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);

    if(val->IsNumber())
    {
        V8_TO_INTEGER(val, model);
        object->SetModel(model);
    }
    else
    {
        V8_TO_STRING(val, model);
        object->SetModel(alt::ICore::Instance().Hash(model));
    }
}

static void ModelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::ILocalObject);
    V8_RETURN_UINT(object->GetModel());
}

extern V8Class v8Object;
extern V8Class v8LocalObject("LocalObject",
                             v8Object,
                             Constructor,
                             [](v8::Local<v8::FunctionTemplate> tpl)
                             {
                                 using namespace alt;
                                 v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                 V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

                                 V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                                 V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

                                 V8Helpers::SetStaticAccessor(isolate, tpl, "allWorld", &AllWorldGetter);

                                 V8Helpers::SetAccessor(isolate, tpl, "model", &ModelGetter, &ModelSetter);

                                 V8Helpers::SetMethod<ILocalObject, &ILocalObject::ResetAlpha>(isolate, tpl, "resetAlpha");

                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsDynamic>(isolate, tpl, "dynamic");
                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::HasGravity, &ILocalObject::ToggleGravity>(isolate, tpl, "hasGravity");

                                 V8Helpers::SetMethod(isolate, tpl, "attachToEntity", &AttachToEntity);
                                 V8Helpers::SetMethod(isolate, tpl, "detach", &Detach);

                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsCollisionEnabled>(isolate, tpl, "isCollisionEnabled");
                                 V8Helpers::SetMethod(isolate, tpl, "toggleCollision", &ToggleCollision);

                                 V8Helpers::SetMethod<ILocalObject, &ILocalObject::PlaceOnGroundProperly>(isolate, tpl, "placeOnGroundProperly");

                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsPositionFrozen, &ILocalObject::SetPositionFrozen>(isolate, tpl, "positionFrozen");

                                 V8Helpers::SetMethod<ILocalObject, &ILocalObject::ActivatePhysics>(isolate, tpl, "activatePhysics");

                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsWorldObject>(isolate, tpl, "isWorldObject");
                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsWeaponObject>(isolate, tpl, "isWeaponObject");

                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsStreamedIn>(isolate, tpl, "isStreamedIn");
                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::UsesStreaming>(isolate, tpl, "useStreaming");

                                 V8Helpers::SetAccessor<ILocalObject, uint32_t, &ILocalObject::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                                 V8Helpers::SetAccessor<ILocalObject, bool, &ILocalObject::IsVisible, &ILocalObject::SetVisible>(isolate, tpl, "visible");
                             });
