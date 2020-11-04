// #include "../CV8Resource.h"
// #include "../helpers/V8Class.h"

// static void Constructor(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();
// 	auto ctx = isolate->GetEnteredContext();

// 	V8_CHECK(info.IsConstructCall(), "HandlingData constructor is not a function");
// 	V8_CHECK(info.Length() == 1, "new HandlingData(...) expects 1 arg");

// 	V8_CHECK(info[0]->IsNumber() || info[0]->IsString(), "zoomDataId must be a number or string");

// 	if (info[0]->IsNumber())
// 	{
// 		uint8_t zoomDataId = info[0]->Uint32Value(ctx).ToChecked();
// 		ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 		V8_CHECK(data, "zoomData with this id not found");
// 		info.This()->SetInternalField(0, info[0]);
// 	}
// 	else
// 	{
// 		std::string zoomDataAlias = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());
// 		ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataAlias);
// 		V8_CHECK(data, "zoomData with this id not found");
// 		uint8_t id = CMapZoomData::Instance().GetIdFromAlias(zoomDataAlias);
// 		info.This()->SetInternalField(0, v8::Integer::NewFromUnsigned(isolate, id));
// 	}
// }

// static void Get(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	V8_CHECK(info.Length() == 1, "MapZoomData.get expects 1 arg");
// 	V8_CHECK(info[0]->IsNumber() || info[0]->IsString(), "zoomDataId must be a number or string");

// 	static V8Class *mapZoomDataClass = &v8MapZoomData;

// 	std::vector<v8::Local<v8::Value>> args{
// 		info[0]};

// 	info.GetReturnValue().Set(mapZoomDataClass->New(isolate->GetEnteredContext(), args));
// }

// static void ResetAll(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	CMapZoomData::Instance().ResetAll();
// }

// static void fZoomScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	info.GetReturnValue().Set(v8::Number::New(isolate, data->fZoomScale));
// }

// static void fZoomScaleSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();
// 	V8_CHECK(val->IsNumber(), "fZoomScale must be a number");

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	data->fZoomScale = (float)val->NumberValue(isolate->GetEnteredContext()).ToChecked();
// }

// static void fZoomSpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	info.GetReturnValue().Set(v8::Number::New(isolate, data->fZoomSpeed));
// }

// static void fZoomSpeedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();
// 	V8_CHECK(val->IsNumber(), "fZoomSpeed must be a number");

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	data->fZoomSpeed = (float)val->NumberValue(isolate->GetEnteredContext()).ToChecked();
// }

// static void fScrollSpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	info.GetReturnValue().Set(v8::Number::New(isolate, data->fScrollSpeed));
// }

// static void fScrollSpeedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();
// 	V8_CHECK(val->IsNumber(), "fScrollSpeed must be a number");

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	data->fScrollSpeed = (float)val->NumberValue(isolate->GetEnteredContext()).ToChecked();
// }

// static void vTilesXGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	info.GetReturnValue().Set(v8::Number::New(isolate, data->vTilesX));
// }

// static void vTilesXSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();
// 	V8_CHECK(val->IsNumber(), "vTilesX must be a number");

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	data->vTilesX = (float)val->NumberValue(isolate->GetEnteredContext()).ToChecked();
// }

// static void vTilesYGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	info.GetReturnValue().Set(v8::Number::New(isolate, data->vTilesY));
// }

// static void vTilesYSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
// 	v8::Isolate *isolate = info.GetIsolate();
// 	V8_CHECK(val->IsNumber(), "vTilesY must be a number");

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	ZoomData *data = CMapZoomData::Instance().GetZoomData(zoomDataId);
// 	V8_CHECK(data, "zoom data not found");

// 	data->vTilesY = (float)val->NumberValue(isolate->GetEnteredContext()).ToChecked();
// }

// static void Reset(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
// 	v8::Isolate *isolate = v8::Isolate::GetCurrent();

// 	alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
// 	V8_CHECK(resource, "Invalid resource");

// 	uint8_t zoomDataId = info.This()->GetInternalField(0)->Uint32Value(isolate->GetEnteredContext()).ToChecked();

// 	CMapZoomData::Instance().Reset(zoomDataId);
// }

// static V8Class v8mapZoomData(
// 	"MapZoomData", "", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
// 		v8::Isolate *isolate = v8::Isolate::GetCurrent();

// 		v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

// 		tpl->InstanceTemplate()->SetInternalFieldCount(1);

// 		tpl->Set(isolate, "get", v8::FunctionTemplate::New(isolate, &Get));
// 		tpl->Set(isolate, "resetAll", v8::FunctionTemplate::New(isolate, &ResetAll));

// 		V8::SetAccessor(isolate, tpl, "fZoomScale", &fZoomScaleGetter, &fZoomScaleSetter);
// 		V8::SetAccessor(isolate, tpl, "fZoomSpeed", &fZoomSpeedGetter, &fZoomSpeedSetter);
// 		V8::SetAccessor(isolate, tpl, "fScrollSpeed", &fScrollSpeedGetter, &fScrollSpeedSetter);
// 		V8::SetAccessor(isolate, tpl, "vTilesX", &vTilesXGetter, &vTilesXSetter);
// 		V8::SetAccessor(isolate, tpl, "vTilesY", &vTilesYGetter, &vTilesYSetter);
// 		V8::SetMethod(isolate, tpl, "reset", &Reset);
// 	},
// 	false);
