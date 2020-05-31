#include "stdafx.h"

#include "../V8ResourceImpl.h"
#include "../V8Helpers.h"

using EventType = alt::CEvent::Type;

V8::LocalEventHandler consoleCommand(
	EventType::CONSOLE_COMMAND_EVENT,
	"consoleCommand",
	[](V8ResourceImpl* resource, const alt::CEvent* e, std::vector<v8::Local<v8::Value>>& args) {
		auto ev = static_cast<const alt::CConsoleCommandEvent*>(e);
		v8::Isolate* isolate = resource->GetIsolate();

		args.push_back(v8::String::NewFromUtf8(isolate, ev->GetName().GetData(), v8::NewStringType::kNormal, ev->GetName().GetSize()).ToLocalChecked());
		for (auto& arg : ev->GetArgs())
			args.push_back(v8::String::NewFromUtf8(isolate, arg.GetData(), v8::NewStringType::kNormal, arg.GetSize()).ToLocalChecked());
	}
);
