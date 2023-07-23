#pragma once

#include <v8.h>

#include "cpp-sdk/objects/IEntity.h"

#include "V8Class.h"

class V8Entity
{
    V8Class* _class;
    alt::IBaseObject* handle;
    v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> jsVal;

public:
    V8Entity(v8::Local<v8::Context> ctx, V8Class* __class, v8::Local<v8::Object> obj, alt::IBaseObject* _handle) : _class(__class), handle(_handle)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        obj->SetInternalField(0, v8::External::New(isolate, this));
        jsVal.Reset(isolate, obj);
    }

    V8Class* GetClass()
    {
        return _class;
    }

    alt::IBaseObject* GetHandle()
    {
        return handle;
    }

    v8::Local<v8::Object> GetJSVal(v8::Isolate* isolate)
    {
        return jsVal.Get(isolate);
    }

    static V8Entity* Get(v8::Local<v8::Value> val)
    {
        if(!val->IsObject()) return nullptr;

        v8::Local<v8::Object> obj = val.As<v8::Object>();
        if(obj->InternalFieldCount() != 1) return nullptr;

        v8::Local<v8::Value> i = obj->GetInternalField(0);
        if(!i->IsExternal()) return nullptr;

        return static_cast<V8Entity*>(i.As<v8::External>()->Value());
    }

    static V8Class* GetClass(alt::IBaseObject* handle)
    {
        extern V8Class v8Player, v8Vehicle, v8Blip, v8Checkpoint, v8Ped, v8Object, v8VirtualEntity, v8VirtualEntityGroup, v8Marker, v8Colshape;
#ifdef ALT_SERVER_API
        extern V8Class v8VoiceChannel, v8ConnectionInfo;
#else
        extern V8Class v8WebView, v8LocalPlayer, v8RmlDocument, v8RmlElement, v8LocalObject, v8WebSocketClient, v8Audio, v8HttpClient, v8AudioFilter, v8TextLabel, v8LocalPed, v8LocalVehicle,
          v8Font, v8AudioOutput, v8AudioOutputFrontend, v8AudioOutputWorld, v8AudioOutputAttached;
#endif

        if(!handle) return nullptr;

        switch(handle->GetType())
        {
            case alt::IBaseObject::Type::PLAYER: return &v8Player;
            case alt::IBaseObject::Type::VEHICLE: return &v8Vehicle;
            case alt::IBaseObject::Type::BLIP: return &v8Blip;
            case alt::IBaseObject::Type::CHECKPOINT: return &v8Checkpoint;
            case alt::IBaseObject::Type::PED: return &v8Ped;
            case alt::IBaseObject::Type::OBJECT: return &v8Object;
            case alt::IBaseObject::Type::VIRTUAL_ENTITY: return &v8VirtualEntity;
            case alt::IBaseObject::Type::VIRTUAL_ENTITY_GROUP: return &v8VirtualEntityGroup;
            case alt::IBaseObject::Type::MARKER: return &v8Marker;
            case alt::IBaseObject::Type::COLSHAPE: return &v8Colshape;
#ifdef ALT_SERVER_API
            case alt::IBaseObject::Type::VOICE_CHANNEL: return &v8VoiceChannel;
            case alt::IBaseObject::Type::CONNECTION_INFO: return &v8ConnectionInfo;
#else
            case alt::IBaseObject::Type::WEBVIEW: return &v8WebView;
            case alt::IBaseObject::Type::LOCAL_PLAYER: return &v8LocalPlayer;
            case alt::IBaseObject::Type::RML_DOCUMENT: return &v8RmlDocument;
            case alt::IBaseObject::Type::RML_ELEMENT: return &v8RmlElement;
            case alt::IBaseObject::Type::LOCAL_OBJECT: return &v8LocalObject;
            case alt::IBaseObject::Type::WEBSOCKET_CLIENT: return &v8WebSocketClient;
            case alt::IBaseObject::Type::AUDIO: return &v8Audio;
            case alt::IBaseObject::Type::AUDIO_OUTPUT: return &v8AudioOutput;
            case alt::IBaseObject::Type::AUDIO_OUTPUT_FRONTEND: return &v8AudioOutputFrontend;
            case alt::IBaseObject::Type::AUDIO_OUTPUT_WORLD: return &v8AudioOutputWorld;
            case alt::IBaseObject::Type::AUDIO_OUTPUT_ATTACHED: return &v8AudioOutputAttached;
            case alt::IBaseObject::Type::HTTP_CLIENT: return &v8HttpClient;
            case alt::IBaseObject::Type::AUDIO_FILTER: return &v8AudioFilter;
            case alt::IBaseObject::Type::TEXT_LABEL: return &v8TextLabel;
            case alt::IBaseObject::Type::LOCAL_PED: return &v8LocalPed;
            case alt::IBaseObject::Type::LOCAL_VEHICLE: return &v8LocalVehicle;
            case alt::IBaseObject::Type::FONT: return &v8Font;
#endif
        }

        return nullptr;
    }
};
