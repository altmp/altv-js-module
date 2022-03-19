#pragma once
#include <string>
#include <vector>

static std::string vector3Bindings =
#include "bindings/Vector3.js.gen"
  ;

namespace JSBindings
{
    const std::string& GetBindingsCode()
    {
        static std::vector<std::string> bindings = { vector3Bindings };
        static std::string code;

        // Append all bindings to one big bindings module once
        if(code.empty())
            for(const std::string& binding : bindings) code += binding;

        return code;
    }
}  // namespace JSBindings
