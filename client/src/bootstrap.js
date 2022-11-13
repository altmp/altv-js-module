// clang-format off
import * as alt from "alt";
import * as native from "natives";

// Load the global bindings code
const bindingsGlobal = {};
new Function("alt", "native", "__global", __internal_bindings_code)(alt, native, bindingsGlobal);
__setLogFunction(bindingsGlobal.genericLog);

let mainPath = __internal_main_path;
if (mainPath[0] !== "/") mainPath = "/" + mainPath;

// Load the main file
if(!alt.isWorker) __internal_start_file(mainPath);
else await import(mainPath)
