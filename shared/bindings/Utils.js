/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Utils JS bindings

// Shared
alt.Utils.wait = function(timeout) {
    if (typeof timeout !== "number") throw new Error("Expected a number as first argument");

    return new Promise(resolve => {
        alt.setTimeout(resolve, timeout);
    });
}

alt.Utils.waitFor = function(callback, timeout = 2000) {
    if (typeof callback !== "function") throw new Error("Expected a function as first argument");
    if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

    const checkUntil = Date.now() + timeout;

    return new Promise((resolve, reject) => {
        const tick = alt.everyTick(() => {
            let result;
            try {
                result = callback();
            } catch (e) {
                const promiseError = new Error(`Failed to wait for callback, error: ${e.message}`);

                reject(promiseError);
                alt.logError(promiseError.message);
                alt.logError(e.stack);
                alt.clearInterval(tick);
                return;
            }

            if (result) {
                alt.clearInterval(tick);
                resolve();
            } else if (Date.now() > checkUntil) {
                alt.clearInterval(tick);
                reject(new Error(`Failed to wait for callback: ${callback}`));
            }
        });
    });
}

// Client only
if (alt.isClient) {
    alt.Utils.requestModel = async function(model, timeout = 1000) {
        const _model = model;
        if (typeof model !== "string" && typeof model !== "number") throw new Error("Expected a string or number as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        if (typeof model === "string") model = alt.hash(model);
        if (!native.isModelValid(model)) throw new Error(typeof _model === "string"
            ? `Model '${_model}', with hash ${alt.hash(_model)} is invalid`
            : `Model ${_model} is invalid`);

        try {
            native.requestModel(model);
            await alt.Utils.waitFor(() => native.hasModelLoaded(model), timeout);
        } catch (e) {
            throw new Error(`Failed to request model '${_model}'`);
        }
    }

    alt.Utils.requestAnimDict = async function(animDict, timeout = 1000) {
        if (typeof animDict !== "string") throw new Error("Expected a string as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        if (!native.doesAnimDictExist(animDict)) throw new Error(`Anim dict '${animDict}' not valid`);

        try {
            native.requestAnimDict(animDict);
            await alt.Utils.waitFor(() => native.hasAnimDictLoaded(animDict), timeout);
        } catch (e) {
            throw new Error(`Failed to request anim dict '${animDict}'`);
        }
    }

    alt.Utils.requestAnimSet = async function(animSet, timeout = 1000) {
        if (typeof animSet !== "string") throw new Error("Expected a string as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        try {
            native.requestAnimSet(animSet);
            await alt.Utils.waitFor(() => native.hasAnimSetLoaded(animSet), timeout);
        } catch (e) {
            throw new Error(`Failed to request anim (clip) set '${animSet}'`);
        }
    }

    // clip set is basically the same as anim set
    alt.Utils.requestClipSet = alt.Utils.requestAnimSet;

    alt.Utils.requestCutscene = async function(cutsceneName, flags, timeout = 1000) {
        if (typeof cutsceneName !== "string") throw new Error("Expected a string as first argument");
        if (typeof flags !== "number" && typeof flags !== "string") 
            throw new Error("Expected a number or string as second argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as third argument");

        try {
            native.requestCutscene(cutsceneName, typeof flags === "string" ? parseInt(flags) : flags);
            await alt.Utils.waitFor(() => native.hasThisCutsceneLoaded(cutsceneName), timeout);
        } catch (e) {
            throw new Error(`Failed to request cutscene '${cutsceneName}'`);
        }
    }
}
// Server only
else {

}
