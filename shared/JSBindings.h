#pragma once
#include <string>
#include <array>

// Shared
#include "bindings/Utils.out.h"
#include "bindings/RGBA.out.h"
#include "bindings/Vector3.out.h"
#include "bindings/Vector2.out.h"

// Client
#ifdef ALT_CLIENT_API
#endif

// Server
#ifdef ALT_SERVER_API
#endif

namespace JSBindings
{
    static std::string GetBindingsCode()
    {
        static constexpr std::array<const char*, 4> bindings = { UtilsBindings::source, RGBABindings::source, Vector3Bindings::source, Vector2Bindings::source };

        std::string code;
        for(const char* binding : bindings) code += binding;

        return code;
    }
}  // namespace JSBindings
