// clang-format off
import * as alt from "alt";
import * as native from "natives";

// Load the global bindings code
const bindingsGlobal = {};
new Function("alt", "__global", __internal_bindings_code)(alt, bindingsGlobal);
__setLogFunction(bindingsGlobal.genericLog);

let mainPath = __internal_main_path;
if (mainPath[0] !== "/") mainPath = "/" + mainPath;
// Load the main file
__internal_start_file(mainPath);
