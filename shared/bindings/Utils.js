/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Utils JS bindings

// Client only
if(alt.isClient) {
    alt.Utils.requestModel = async function(model, timeout = 1000) {
        const _model = model;
        if (typeof model !== "string" && typeof model !== "number") throw new Error("Expected a string or number as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        if (typeof model === "string") model = alt.hash(model);
        if (!native.isModelValid(model)) throw new Error(typeof _model === "string"
            ? `Model '${_model}', with hash ${alt.hash(_model)} is invalid`
            : `Model ${_model} is invalid`);

        const checkUntil = Date.now() + timeout;
        native.requestModel(model);

        while (!native.hasModelLoaded(model)) {
            if (Date.now() > checkUntil) throw new Error(`Failed to request model '${_model}'`);
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestAnimDict = async function(animDict, timeout = 1000) {
        if (typeof animDict !== "string") throw new Error("Expected a string as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        if (!native.doesAnimDictExist(animDict)) throw new Error(`Anim dict '${animDict}' not valid`);

        const checkUntil = Date.now() + timeout;
        native.requestAnimDict(animDict);

        while (!native.hasAnimDictLoaded(animDict)) {
            if (Date.now() > checkUntil) throw new Error(`Failed to request anim dict '${animDict}'`);
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestAnimSet = async function(animSet, timeout = 1000) {
        if (typeof animSet !== "string") throw new Error("Expected a string as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        const checkUntil = Date.now() + timeout;
        native.requestAnimSet(animSet);

        while (!native.hasAnimSetLoaded(animSet)) {
            if (Date.now() > checkUntil) throw new Error(`Failed to request anim set '${animSet}'`);
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestClipSet = async function(clipSet, timeout = 1000) {
        if (typeof clipSet !== "string") throw new Error("Expected a string as first argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        const checkUntil = Date.now() + timeout;
        native.requestClipSet(clipSet);

        while (!native.hasClipSetLoaded(clipSet)) {
            if (Date.now() > checkUntil) throw new Error(`Failed to request clip set '${clipSet}'`);
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestCutscene = async function(cutsceneName, flags, timeout = 1000) {
        if (typeof cutsceneName !== "string") throw new Error("Expected a string as first argument");
        if (typeof flags !== "number" && typeof flags !== "string") 
            throw new Error("Expected a number or string as second argument");
        if (typeof timeout !== "number") throw new Error("Expected a number as third argument");

        const checkUntil = Date.now() + timeout;
        native.requestCutscene(cutsceneName, typeof flags === "string" ? parseInt(flags) : flags);

        while (!native.hasCutsceneLoaded(cutsceneName)) {
            if (Date.now() > checkUntil) throw new Error(`Failed to request cutscene '${cutsceneName}'`);
            await alt.Utils.wait(5);
        }
    }
}
// Server only
else {

}

// Shared
alt.Utils.wait = function(timeout) {
    if (typeof timeout !== "number") throw new Error("Expected a number as first argument");

    return new Promise(resolve => {
        alt.setTimeout(resolve, timeout);
    });
}
