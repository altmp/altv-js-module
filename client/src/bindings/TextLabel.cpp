#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/ILocalObject.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(9, 11);

    V8_ARG_TO_STRING(1, text);
    V8_ARG_TO_STRING(2, fontName);
    V8_ARG_TO_NUMBER(3, fontSize);
    V8_ARG_TO_NUMBER(4, scale);
    V8_ARG_TO_VECTOR3(5, pos);
    V8_ARG_TO_VECTOR3(6, rot);
    V8_ARG_TO_RGBA(7, color);
    V8_ARG_TO_NUMBER(8, outlineWidth);
    V8_ARG_TO_RGBA(9, outlineColor);
    V8_ARG_TO_BOOLEAN_OPT(10, useStreaming, false);
    V8_ARG_TO_UINT_OPT(11, streamingDistance, 0);

    auto textLabel =
    alt::ICore::Instance().CreateTextLabel(text, fontName, fontSize, scale, pos, rot, color, outlineWidth, outlineColor, useStreaming, streamingDistance, resource->GetResource());
    V8_BIND_BASE_OBJECT(textLabel, "Failed to create textlabel");
}

// static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
// {
//     V8_GET_ISOLATE_CONTEXT_RESOURCE();
//     V8_RETURN(resource->GetAllTextLabel());
// }

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::TEXT_LABEL, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::TEXT_LABEL)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void AlignGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_GET_THIS_BASE_OBJECT(label, alt::ITextLabel);

    V8_RETURN_NUMBER(label->GetAlign());
}

static void AlignSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(label, alt::ITextLabel);

    V8_TO_NUMBER(val, align);
    label->SetAlign((alt::ITextLabel::Alignment) align);
}

extern V8Class v8WorldObject;
extern V8Class v8TextLabel("TextLabel",
                           v8WorldObject,
                           Constructor,
                           [](v8::Local<v8::FunctionTemplate> tpl)
                           {
                               using namespace alt;
                               v8::Isolate* isolate = v8::Isolate::GetCurrent();

                               V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                               V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

                               V8Helpers::SetAccessor<ITextLabel, bool, &ITextLabel::IsStreamedIn>(isolate, tpl, "isStreamedIn");

                               V8Helpers::SetAccessor<ITextLabel, bool, &ITextLabel::IsGlobal>(isolate, tpl, "isGlobal");
                               V8Helpers::SetAccessor<ITextLabel, IPlayer*, &ITextLabel::GetTarget>(isolate, tpl, "target");
                               V8Helpers::SetAccessor<ITextLabel, bool, &ITextLabel::IsVisible, &ITextLabel::SetVisible>(isolate, tpl, "visible");
                               V8Helpers::SetAccessor<ITextLabel, RGBA, &ITextLabel::GetColor, &ITextLabel::SetColor>(isolate, tpl, "color");
                               V8Helpers::SetAccessor<ITextLabel, RGBA, &ITextLabel::GetOutlineColor, &ITextLabel::SetOutlineColor>(isolate, tpl, "outlineColor");
                               V8Helpers::SetAccessor<ITextLabel, float, &ITextLabel::GetOutlineWidth, &ITextLabel::SetOutlineWidth>(isolate, tpl, "outlineWidth");
                               V8Helpers::SetAccessor<ITextLabel, float, &ITextLabel::GetFontSize, &ITextLabel::SetFontSize>(isolate, tpl, "fontSize");
                               V8Helpers::SetAccessor(isolate, tpl, "align", &AlignGetter, &AlignSetter);
                               V8Helpers::SetAccessor<ITextLabel, std::string, &ITextLabel::GetText, const std::string&, &ITextLabel::SetText>(isolate, tpl, "text");
                               V8Helpers::SetAccessor<ITextLabel, std::string, &ITextLabel::GetFont, const std::string&, &ITextLabel::SetFont>(isolate, tpl, "font");
                               V8Helpers::SetAccessor<ITextLabel, float, &ITextLabel::GetScale, &ITextLabel::SetScale>(isolate, tpl, "scale");
                               V8Helpers::SetAccessor<ITextLabel, Rotation, &ITextLabel::GetRotation, &ITextLabel::SetRotation>(isolate, tpl, "rot");
                               V8Helpers::SetAccessor<ITextLabel, bool, &ITextLabel::IsFacingCamera, &ITextLabel::SetFaceCamera>(isolate, tpl, "faceCamera");

                               V8Helpers::SetAccessor<ITextLabel, uint32_t, &ITextLabel::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                           });
