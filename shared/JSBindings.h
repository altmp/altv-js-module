#pragma once
#include <string>
#include <array>

// clang-format off
// Shared
static constexpr const char* utilsBindings =
#include "bindings/Utils.js.gen"
;

static constexpr const char* vector3Bindings =
#include "bindings/Vector3.js.gen"
;

static constexpr const char* vector2Bindings =
#include "bindings/Vector2.js.gen"
;

static constexpr const char* rgbaBindings =
#include "bindings/RGBA.js.gen"
;
// Client
#ifdef ALT_CLIENT_API
static constexpr const char* objectBindings =
#include "bindings/Object.js.gen"
;
#endif
// Server
#ifdef ALT_SERVER_API

#endif
// clang-format on

namespace JSBindings
{
    static std::string GetBindingsCode()
    {
        static constexpr std::array<const char*, 4> sharedBindings = { utilsBindings, vector3Bindings, vector2Bindings, rgbaBindings };

        std::string code;
        for(const char* binding : sharedBindings) code += binding;
#ifdef ALT_CLIENT_API
        static constexpr std::array<const char*, 1> clientBindings = { objectBindings };
        for(const char* binding : clientBindings) code += binding;
#endif
#ifdef ALT_SERVER_API
        static constexpr std::array<const char*, 0> serverBindings = {};
        for(const char* binding : serverBindings) code += binding;
#endif

        return code;
    }
}  // namespace JSBindings
