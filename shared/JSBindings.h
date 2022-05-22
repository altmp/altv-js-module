#pragma once
#include <string>
#include <vector>

static std::string utilsBindings =
#include "bindings/Utils.js.gen"
  ;

static std::string vector3Bindings =
#include "bindings/Vector3.js.gen"
  ;

static std::string vector2Bindings =
#include "bindings/Vector2.js.gen"
  ;

static std::string rgbaBindings =
#include "bindings/RGBA.js.gen"
  ;

namespace JSBindings
{
    static const std::string& GetBindingsCode()
    {
        static std::vector<std::string> bindings = { utilsBindings, vector3Bindings, vector2Bindings, rgbaBindings };
        static std::string code;

        // Append all bindings to one big bindings module once
        if(code.empty())
            for(const std::string& binding : bindings) code += binding;

        return code;
    }
}  // namespace JSBindings
