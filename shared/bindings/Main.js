/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Main JS bindings

// Client only
if(alt.isClient) {
    alt.Utils.requestModel = async function(model, timeout = 1000) {
        if (typeof model === "string") model = alt.hash(model);
        if (!native.isModelValid(model)) throw new Error("Model not valid");
        const checkUntil = Date.now() + timeout;
        native.requestModel(model);

        while (!native.hasModelLoaded(model)) {
            if (Date.now() > checkUntil) throw new Error("Failed to request model");
            await alt.Utils.wait(5);
        }
    }
}
// Server only
else {

}

// Shared
alt.Utils.wait = function(timeout) {
    return new Promise(resolve => {
        alt.setTimeout(resolve, timeout);
    });
}
