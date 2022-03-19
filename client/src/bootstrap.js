// clang-format off
import * as alt from "alt-client";

// Load the global bindings code
new Function("alt", __internal_bindings_code)(alt);

let mainPath = __internal_main_path;
if(mainPath[0] !== "/") mainPath = "/" + mainPath;
await import(mainPath);
