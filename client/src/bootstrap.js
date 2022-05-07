// clang-format off
import * as alt from "alt";
import * as native from "natives";

// Load the global bindings code
new Function("alt", "native", __internal_bindings_code)(alt, native);

let mainPath = __internal_main_path;
if(mainPath[0] !== "/") mainPath = "/" + mainPath;
try {
    // Load the main file
    const module = await import(mainPath);
    __internal_set_exports(module);
}
catch(e) {
    let location = `${alt.Resource.current.name}:${mainPath}:${e.lineNumber ?? 0}`;
    alt.logError(`[V8] Exception at ${location} (${e.message ?? "Unknown error"}) ${e.stack ?? ""}`);
}
