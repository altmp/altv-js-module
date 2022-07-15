#pragma once
#include <string>
#include <array>

// clang-format off
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
// clang-format on

namespace JSBindings
{
    static std::string GetBindingsCode()
    {
        static constexpr std::array<const char*, 4> bindings = { utilsBindings, vector3Bindings, vector2Bindings, rgbaBindings };

        std::string code;
        for(const char* binding : bindings) code += binding;

        return code;
    }
}  // namespace JSBindings
