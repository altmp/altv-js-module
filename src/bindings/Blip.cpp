
// #include "../CV8Resource.h"
// #include "../helpers/V8Class.h"
// #include "cpp-sdk/script-objects/IBlip.h"

// static void Constructor(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(info.IsConstructCall(), "Blip constructor is not a function");

//     // xyz as obj
//     /*
//     V8_CHECK(info.Length() == 2, "new Blip(...) expects 2 args");

//     V8_CHECK(info[0]->IsObject(), "pos must be an object of {x, y, z}");
//     auto pos = info[1]->ToObject(isolate);
//     double x = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "x")).ToLocalChecked()
//         ->NumberValue(ctx).ToChecked();
//     double y = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "y")).ToLocalChecked()
//         ->NumberValue(ctx).ToChecked();
//     double z = pos->Get(ctx, v8::String::NewFromUtf8(isolate, "z")).ToLocalChecked()
//         ->NumberValue(ctx).ToChecked();

//     V8_CHECK(info[1]->IsNumber(), "type must be an unsigned integer");
//     */

//     /*
// 	let blip = new alt.Blip(0, 0, 72, 11, 50, 50);
// 	blip.rotation = 0;
// 	*/

//     // xyz as args

//     V8_CHECK(false, "You can't use constructor of abstract class");
// }

// static void ConstructorAreaBlip(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(info.IsConstructCall(), "AreaBlip constructor is not a function");

//     V8_CHECK(info.Length() == 5, "new AreaBlip(...) expects 5 args");
//     V8_CHECK(info[0]->IsNumber(), "x must be a number");
//     V8_CHECK(info[1]->IsNumber(), "y must be a number");
//     V8_CHECK(info[2]->IsNumber(), "z must be a number");
//     V8_CHECK(info[3]->IsNumber(), "width must be a number");
//     V8_CHECK(info[4]->IsNumber(), "height must be a number");

//     v8::Local<v8::Number> x = info[0]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> y = info[1]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> z = info[2]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> width = info[3]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> height = info[4]->ToNumber(ctx).ToLocalChecked();

//     alt::Ref<alt::IBlip> blip = alt::IBlip::CreateBlipForArea({x->Value(), y->Value(), z->Value()}, width->Value(), height->Value());

//     V8_CHECK(blip, "Blip creation failed");

//     static_cast<CV8ResourceImpl *>(resource)->AddOwned(blip);
//     resource->BindEntity(info.This(), blip.Get());
// }

// static void ConstructorRadiusBlip(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(info.IsConstructCall(), "RadiusBlip constructor is not a function");

//     V8_CHECK(info.Length() == 4, "new RadiusBlip(...) expects 4 args");
//     V8_CHECK(info[0]->IsNumber(), "x must be a number");
//     V8_CHECK(info[1]->IsNumber(), "y must be a number");
//     V8_CHECK(info[2]->IsNumber(), "z must be a number");
//     V8_CHECK(info[3]->IsNumber(), "radius must be a number");

//     v8::Local<v8::Number> x = info[0]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> y = info[1]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> z = info[2]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> radius = info[3]->ToNumber(ctx).ToLocalChecked();
//     alt::Ref<alt::IBlip> blip = alt::IBlip::CreateBlipForRadius({x->Value(), y->Value(), z->Value()}, radius->Value());

//     V8_CHECK(blip, "Blip creation failed");

//     static_cast<CV8ResourceImpl *>(resource)->AddOwned(blip);
//     resource->BindEntity(info.This(), blip.Get());
// }

// static void ConstructorPointBlip(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(info.IsConstructCall(), "PointBlip constructor is not a function");

//     V8_CHECK(info.Length() == 3, "new PointBlip(...) expects 3 args");
//     V8_CHECK(info[0]->IsNumber(), "x must be a number");
//     V8_CHECK(info[1]->IsNumber(), "y must be a number");
//     V8_CHECK(info[2]->IsNumber(), "z must be a number");

//     v8::Local<v8::Number> x = info[0]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> y = info[1]->ToNumber(ctx).ToLocalChecked();
//     v8::Local<v8::Number> z = info[2]->ToNumber(ctx).ToLocalChecked();
//     alt::Ref<alt::IBlip> blip = alt::IBlip::Create(alt::IBlip::BlipType::DESTINATION, {x->Value(), y->Value(), z->Value()});

//     V8_CHECK(blip, "Blip creation failed");

//     static_cast<CV8ResourceImpl *>(resource)->AddOwned(blip);
//     resource->BindEntity(info.This(), blip.Get());
// }

// static void ConstructorPedBlip(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(info.IsConstructCall(), "PedBlip constructor is not a function");

//     V8_CHECK(info.Length() == 1, "new PedBlip(...) expects 1 arg");
//     V8_CHECK(info[0]->IsNumber(), "pedId must be a number");

//     uint32_t pedId = info[0]->IntegerValue(ctx).ToChecked();
//     alt::Ref<alt::IBlip> blip = alt::IBlip::CreateBlipForEntity(alt::IBlip::BlipType::PED, pedId);

//     V8_CHECK(blip, "Blip creation failed");

//     static_cast<CV8ResourceImpl *>(resource)->AddOwned(blip);
//     resource->BindEntity(info.This(), blip.Get());
// }

// static void ConstructorVehicleBlip(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(info.IsConstructCall(), "VehicleBlip constructor is not a function");

//     V8_CHECK(info.Length() == 1, "new VehicleBlip(...) expects 1 arg");
//     V8_CHECK(info[0]->IsNumber(), "vehicleId must be a number");

//     uint32_t vehicleId = info[0]->IntegerValue(ctx).ToChecked();
//     alt::Ref<alt::IBlip> blip = alt::IBlip::CreateBlipForEntity(alt::IBlip::BlipType::VEHICLE, vehicleId);

//     V8_CHECK(blip, "Blip creation failed");

//     static_cast<CV8ResourceImpl *>(resource)->AddOwned(blip);
//     resource->BindEntity(info.This(), blip.Get());
// }

// //static void ScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetScale()));
// //}

// static void ScaleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "scale must be a number");
//     double val = value->NumberValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();

//     blip->SetScale(val);
// }

// /**
//  * Scale XY
//  * */
// //static void ScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //    auto scale = blip->GetScaleXY();
// //
// //
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, ));
// //}

// /**
//  * Scale XY
//  * */
// //static void ScaleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //    V8_CHECK(value->IsNumber(), "scale must be a number");
// //    double val = value->NumberValue(ctx).ToChecked();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //    blip->SetScale(val);
// //}

// //static void SpriteGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void SpriteSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "sprite must be an integer");
//     int sprite = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();

//     blip->SetSprite(sprite);
// }

// //static void ColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void ColorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "color must be an integer");
//     int val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetColor(val);
// }

// //static void SecondaryColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void SecondaryColorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "secondaryColor must be an unsigned integer");
//     int val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetSecondaryColor(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void AlphaSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "alpha must be an integer");
//     int val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetAlpha(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void FlashTimerSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "flashTimer must be an integer");
//     int val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetFlashTimer(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void FlashIntervalSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "flashInterval must be an integer");
//     int val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetFlashInterval(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void FriendlySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetAsFriendly(val);
// }

// //static void RouteGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void RouteSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetRoute(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void BrightSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetBright(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void NumberSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "number must be an integer");
//     auto val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetNumber(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void ShowConeSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetShowCone(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void FlashesSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetFlashes(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void FlashesAlternateSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetFlashesAlternate(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void ShortRangeSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetAsShortRange(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void PrioritySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "priority must be an integer");
//     auto val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetPriority(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->GetSprite()));
// //}

// static void RotationSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "rotation must be a number");
//     auto val = value->NumberValue(ctx).ToChecked();

//     Log::Debug << "nextRotation = " << val << Log::Endl;

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetRotation(val);
// }

// //static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// //{
// //    v8::Isolate* isolate = info.GetIsolate();
// //    auto ctx = isolate->GetEnteredContext();
// //
// //	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();
// //
// //	info.GetReturnValue().Set(v8::Number::New(isolate, blip->()));
// //}

// static void GxtNameSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsString(), "gxtName must be a string");
//     auto val = *v8::String::Utf8Value(info.GetIsolate(), value->ToString(ctx).ToLocalChecked());

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetGxtName(val);
// }

// static void NameSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsString(), "name must be a string");
//     auto val = *v8::String::Utf8Value(info.GetIsolate(), value->ToString(ctx).ToLocalChecked());

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetName(val);
// }

// /*
// static void Getter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
// {
//     v8::Isolate* isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

// 	alt::Ref<alt::IBlip> blip = (alt::Ref<alt::IBlip>)info.This()->GetInternalField(0).As<v8::External>()->Value();

//     auto ret = v8::Number::New(isolate, blip->GETTER()));
// 	info.GetReturnValue().Set(ret);
// }
// */

// static void RouteColorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "routeColor must be unsigned integer");
//     auto val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetRouteColor(val);
// }

// static void PulseSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetPulse(val);
// }

// static void AsMissionCreatorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetAsMissionCreator(val);
// }

// static void tickVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetTickVisible(val);
// }

// static void headingIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetHeadingIndicatorVisible(val);
// }

// static void outlineIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetOutlineIndicatorVisible(val);
// }

// static void friendIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetFriendIndicatorVisible(val);
// }

// static void crewIndicatorVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetCrewIndicatorVisible(val);
// }

// static void categorySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(value->IsNumber(), "category must be integer");
//     auto val = value->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetCategory(val);
// }

// static void highDetailSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetAsHighDetail(val);
// }

// static void shrinkedSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     auto val = value->ToBoolean(isolate)->Value();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();
//     blip->SetShrinked(val);
// }

// static void Fade(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8_CHECK(info.Length() == 2, "2 args expected");

//     V8_CHECK(info[0]->IsNumber(), "opacity must be an unsigned integer");
//     uint32_t opacity = info[0]->IntegerValue(ctx).ToChecked();

//     V8_CHECK(info[1]->IsNumber(), "duration must be an unsigned integer");
//     uint32_t duration = info[1]->IntegerValue(ctx).ToChecked();

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IBlip> blip = _this->GetHandle().As<alt::IBlip>();

//     blip->Fade(opacity, duration);
// }

// static V8Class v8blip("Blip", "WorldObject", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
//     v8::Isolate *isolate = v8::Isolate::GetCurrent();

//     /*
// 	let ped = game.playerPedId();
// 	let coords = game.getEntityCoords(ped, true);

// 	let area = new alt.AreaBlip(coords.x, coords.y, coords.z, 30, 30);
// 	area.color = 5;
// 	area.rotation = 0;
// 	*/

//     V8::SetAccessor(isolate, tpl, "sprite", nullptr, &SpriteSetter);
//     V8::SetAccessor(isolate, tpl, "scale", nullptr, &ScaleSetter);
//     V8::SetAccessor(isolate, tpl, "color", nullptr, &ColorSetter);
//     V8::SetAccessor(isolate, tpl, "secondaryColor", nullptr, &SecondaryColorSetter);
//     V8::SetAccessor(isolate, tpl, "alpha", nullptr, &AlphaSetter);
//     V8::SetAccessor(isolate, tpl, "flashTimer", nullptr, &FlashTimerSetter);
//     V8::SetAccessor(isolate, tpl, "flashInterval", nullptr, &FlashIntervalSetter);
//     V8::SetAccessor(isolate, tpl, "route", nullptr, &RouteSetter);
//     V8::SetAccessor(isolate, tpl, "bright", nullptr, &BrightSetter);
//     V8::SetAccessor(isolate, tpl, "number", nullptr, &NumberSetter);
//     V8::SetAccessor(isolate, tpl, "showCone", nullptr, &ShowConeSetter);
//     V8::SetAccessor(isolate, tpl, "flashes", nullptr, &FlashesSetter);
//     V8::SetAccessor(isolate, tpl, "flashesAlternate", nullptr, &FlashesAlternateSetter);
//     V8::SetAccessor(isolate, tpl, "shortRange", nullptr, &ShortRangeSetter);
//     V8::SetAccessor(isolate, tpl, "priority", nullptr, &PrioritySetter);
//     V8::SetAccessor(isolate, tpl, "heading", nullptr, &RotationSetter);
//     V8::SetAccessor(isolate, tpl, "gxtName", nullptr, &GxtNameSetter);
//     V8::SetAccessor(isolate, tpl, "name", nullptr, &NameSetter);
//     V8::SetAccessor(isolate, tpl, "routeColor", nullptr, &RouteColorSetter);
//     V8::SetAccessor(isolate, tpl, "pulse", nullptr, &PulseSetter);
//     V8::SetAccessor(isolate, tpl, "asMissionCreator", nullptr, &AsMissionCreatorSetter);
//     V8::SetAccessor(isolate, tpl, "tickVisible", nullptr, &tickVisibleSetter);
//     V8::SetAccessor(isolate, tpl, "headingIndicatorVisible", nullptr, &headingIndicatorVisibleSetter);
//     V8::SetAccessor(isolate, tpl, "outlineIndicatorVisible", nullptr, &outlineIndicatorVisibleSetter);
//     V8::SetAccessor(isolate, tpl, "friendIndicatorVisible", nullptr, &friendIndicatorVisibleSetter);
//     V8::SetAccessor(isolate, tpl, "crewIndicatorVisible", nullptr, &crewIndicatorVisibleSetter);
//     V8::SetAccessor(isolate, tpl, "category", nullptr, &categorySetter);
//     V8::SetAccessor(isolate, tpl, "highDetail", nullptr, &highDetailSetter);
//     V8::SetAccessor(isolate, tpl, "shrinked", nullptr, &shrinkedSetter);

//     V8::SetMethod(isolate, tpl, "fade", &Fade);
// });

// static V8Class v8AreaBlip("AreaBlip", "Blip", ConstructorAreaBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
//     v8::Isolate *isolate = v8::Isolate::GetCurrent();

//     v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
// });

// static V8Class v8RadiusBlip("RadiusBlip", "Blip", ConstructorRadiusBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
//     v8::Isolate *isolate = v8::Isolate::GetCurrent();

//     v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
// });

// static V8Class v8PointBlip("PointBlip", "Blip", ConstructorPointBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
//     v8::Isolate *isolate = v8::Isolate::GetCurrent();

//     v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
// });

// static V8Class v8PedBlip("PedBlip", "Blip", ConstructorPedBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
//     v8::Isolate *isolate = v8::Isolate::GetCurrent();

//     v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
// });

// static V8Class v8VehicleBlip("VehicleBlip", "Blip", ConstructorVehicleBlip, [](v8::Local<v8::FunctionTemplate> tpl) {
//     v8::Isolate *isolate = v8::Isolate::GetCurrent();

//     v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
// });
