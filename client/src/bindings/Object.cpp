#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/IObject.h"

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);

    std::ostringstream ss;
    ss << "Object{ model: " << object->GetModel() << ", scriptID: " << object->GetScriptGuid() << " }";

    V8_RETURN_STRING(ss.str());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 5);

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

    auto obj = alt::ICore::Instance().CreateObject(modelHash, pos, rot, noOffset, dynamic);
    V8_BIND_BASE_OBJECT(obj, "Failed to create object");
}

static void PosGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_RETURN_VECTOR3(object->GetPosition());
}

static void PosSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_TO_VECTOR3(value, val);
    object->SetPosition(val);
}

static void RotGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_RETURN_VECTOR3(object->GetRotation());
}

static void RotSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(object, alt::IObject);
    V8_TO_VECTOR3(value, val);
    object->SetRotation(val);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto objects = alt::ICore::Instance().GetObjects();
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}

extern V8Class v8Entity;
extern V8Class v8Object("Object",
                      v8Entity,
                      Constructor,
                      [](v8::Local<v8::FunctionTemplate> tpl) {
                            using namespace alt;
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);

                            V8Helpers::SetAccessor(isolate, tpl, "pos", &PosGetter, &PosSetter);
                            V8Helpers::SetAccessor(isolate, tpl, "rot", &RotGetter, &RotSetter);

                            V8Helpers::SetAccessor<IObject, bool, &IObject::IsRemote>(isolate, tpl, "isRemote");
                      });
