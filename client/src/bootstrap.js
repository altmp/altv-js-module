// clang-format off
import * as alt from "alt";
import * as native from "natives";

// Load the global bindings code
new Function("alt", "native", __internal_bindings_code)(alt, native);
__setLogFunction(genericLog);

let mainPath = __internal_main_path;
if (mainPath[0] !== "/") mainPath = "/" + mainPath;
// Load the main file
__internal_start_file(mainPath);
