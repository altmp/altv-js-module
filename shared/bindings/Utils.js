/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Utils JS bindings

// Client only
if(alt.isClient) {
    alt.Utils.requestModel = async function(model, timeout = 1000) {
        if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

        if (typeof model === "string") model = alt.hash(model);
        if (!native.isModelValid(model)) throw new Error("Model not valid");

        const checkUntil = Date.now() + timeout;
        native.requestModel(model);

        while (!native.hasModelLoaded(model)) {
            if (Date.now() > checkUntil) throw new Error("Failed to request model");
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestAnimDict = async function(animDict) {
        if (typeof animDict !== "string") throw new Error("Expected a string as first argument");
        if (!native.doesAnimDictExist(animDict)) throw new Error("Anim dict not valid");

        const checkUntil = Date.now() + timeout;
        native.requestAnimDict(animDict);

        while (!native.hasAnimDictLoaded(animDict)) {
            if (Date.now() > checkUntil) throw new Error("Failed to request anim dict");
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestAnimSet = async function(animSet) {
        if (typeof animSet !== "string") throw new Error("Expected a string as first argument");

        const checkUntil = Date.now() + timeout;
        native.requestAnimSet(animSet);

        while (!native.hasAnimSetLoaded(animSet)) {
            if (Date.now() > checkUntil) throw new Error("Failed to request anim set");
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestClipSet = async function(clipSet) {
        if (typeof clipSet !== "string") throw new Error("Expected a string as first argument");

        const checkUntil = Date.now() + timeout;
        native.requestClipSet(clipSet);

        while (!native.hasClipSetLoaded(clipSet)) {
            if (Date.now() > checkUntil) throw new Error("Failed to request clip set");
            await alt.Utils.wait(5);
        }
    }

    alt.Utils.requestCutscene = async function(cutsceneName, flags) {
        if (typeof cutsceneName !== "string") throw new Error("Expected a string as first argument");
        if (typeof flags !== "number") throw new Error("Expected a number as second argument");

        const checkUntil = Date.now() + timeout;
        native.requestCutscene(cutsceneName, flags);

        while (!native.hasCutsceneLoaded(cutsceneName)) {
            if (Date.now() > checkUntil) throw new Error("Failed to request cutscene");
            await alt.Utils.wait(5);
        }
    }
}
// Server only
else {

}

// Shared
alt.Utils.wait = function(timeout) {
    if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

    return new Promise(resolve => {
        alt.setTimeout(resolve, timeout);
    });
}

alt.Utils.timeoutPromise = async function(promise, timeout) {
    if (!(promise instanceof Promise)) throw new Error("Expected a promise as first argument");
    if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

    let timeout;
    return Promise.race([
        promise,
        new Promise((_res, rej) => {
            timeout = alt.setTimeout(() => rej('Operation timed out'), ms);
        }),
    ]).finally(() => alt.clearTimeout(timeout));
}