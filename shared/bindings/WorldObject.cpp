
#include "../V8Helpers.h"
#include "../helpers/BindHelpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"

using namespace alt;

extern V8Class v8BaseObject;
extern V8Class v8WorldObject("WorldObject",
                             v8BaseObject,
                             [](v8::Local<v8::FunctionTemplate> tpl)
                             {
                                 v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                 V8Helpers::SetAccessor<IWorldObject, Position, &IWorldObject::GetPosition, &IWorldObject::SetPosition>(isolate, tpl, "pos");
                                 V8Helpers::SetAccessor<IWorldObject, int32_t, &IWorldObject::GetDimension, &IWorldObject::SetDimension>(isolate, tpl, "dimension");
                             });
