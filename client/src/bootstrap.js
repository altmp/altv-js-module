// clang-format off
import * as alt from "alt";
import * as native from "natives";

let mainPath = __internal_main_path;
let isEval = __internal_eval;
if(!isEval && mainPath[0] !== "/") mainPath = "/" + mainPath;
try {
    // Load the main file or the script if eval
    let module;
    if(isEval) {
        eval(mainPath);
    }else{
        module = await import(mainPath);
        __internal_set_exports(module);
    }
}
catch(e) {
    let location = `${alt.Resource.current.name}:${mainPath}:${e.lineNumber ?? 0}`;
    alt.logError(`[V8] Exception at ${location} (${e.message ?? "Unknown error"}) ${e.stack ?? ""}`);
}
