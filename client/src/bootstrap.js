// clang-format off
import * as alt from "alt-client";
import * as native from "natives";

// Load the global bindings code
new Function("alt", "native", __internal_bindings_code)(alt, native);

let mainPath = alt.Resource.current.main;
if(mainPath[0] !== "/") mainPath = "/" + mainPath;
try {
    await import(mainPath);
}
catch(e) {
    let location = `${alt.Resource.current.name}:${mainPath}:${e.lineNumber ?? 0}`;
    alt.logError(`[V8] Exception at ${location} (${e.message ?? "Unknown error"}) ${e.stack ?? ""}`);
}
