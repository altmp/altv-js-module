// clang-format off
import * as alt from "alt-client";
import * as native from "natives";

// Load the global bindings code
new Function("alt", "native", __internal_bindings_code)(alt, native);

let mainPath = alt.Resource.current.main;
if(mainPath[0] !== "/") mainPath = "/" + mainPath;
await import(mainPath);
