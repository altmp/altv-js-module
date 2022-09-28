/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Utils JS bindings

// Shared

class BaseUtility {
    #destroyed = false;

    _tryDestroy() {
        if (this.#destroyed) return false;
        this.#destroyed = true;
        return true;
    }
}

alt.Utils.Timer = class Timer extends BaseUtility {
    #id = 0;

    constructor(callback, ms, once) {
        super();

        const handler = () => {
            if (once) {
                super._tryDestroy();
                this.#clearId();
            }

            callback();
        };

        if (once)
            this.#id = alt.setTimeout(handler, ms);
        else
            this.#id = alt.setInterval(handler, ms);
    }

    get id() {
        return this.#id;
    }

    set id(_) {
        throw new Error("Timer id property is read-only");
    }

    #clearId() {
        // timer id starts from 1
        this.#id = 0;
    }

    destroy() {
        if (!super._tryDestroy())
            throw new Error("Timer already destroyed");

        alt.clearTimer(this.#id);
        this.#clearId();
    }
}

alt.Utils.Timeout = class Timeout extends alt.Utils.Timer {
    constructor(callback, ms) {
        super(callback, ms, true);
    }
}

alt.Utils.NextTick = class NextTick extends alt.Utils.Timer {
    constructor(callback) {
        super(callback, 0, true);
    }
}

alt.Utils.Interval = class Interval extends alt.Utils.Timer {
    constructor(callback, ms) {
        super(callback, ms, false);
    }
}

alt.Utils.EveryTick = class EveryTick extends alt.Utils.Timer {
    constructor(callback) {
        super(callback, 0, false);
    }
}

alt.Utils.wait = function(timeout) {
    if (typeof timeout !== "number") throw new Error("Expected a number as first argument");

    return new Promise(resolve => {
        new alt.Utils.Timeout(resolve, timeout);
    });
}

alt.Utils.waitFor = function(callback, timeout = 2000) {
    if (typeof callback !== "function") throw new Error("Expected a function as first argument");
    if (typeof timeout !== "number") throw new Error("Expected a number as second argument");

    const checkUntil = Date.now() + timeout;

    return new Promise((resolve, reject) => {
        const tick = new alt.Utils.EveryTick(() => {
            let result;
            try {
                result = callback();
            } catch (e) {
                const promiseError = new Error(`Failed to wait for callback, error: ${e.message}`);
                promiseError.stack = e.stack;
                reject(promiseError);
                tick.destroy();
                return;
            }

            if (result) {
                tick.destroy();
                resolve();
            } else if (Date.now() > checkUntil) {
                tick.destroy();
                reject(new Error(`Failed to wait for callback: ${callback}`));
            }
        });
    });
}

alt.Utils.ConsoleCommand = class ConsoleCommand extends BaseUtility {
    /**
     * @type {Map<string, Set<(...args: string[]) => void>>}
     */
    static #handlers = null;

    static #init() {
        if (ConsoleCommand.#handlers) return;
        ConsoleCommand.#handlers = new Map();

        alt.on("consoleCommand", (name, ...args) => {
            ConsoleCommand.#handlers
                .get(name)
                ?.forEach(h => h(...args));
        });
    }

    static #addHandler(instance) {
        const handlers = ConsoleCommand.#handlers.get(instance.#name) ?? new Set();
        handlers.add(instance.#handler);
        ConsoleCommand.#handlers.set(instance.#name, handlers);
    }

    static #removeHandler(instance) {
        ConsoleCommand.#handlers
            .get(instance.#name)
            ?.delete(instance.#handler);
    }

    #name = "";
    #handler = () => {};

    constructor(name, handler) {
        if (typeof name !== "string")
            throw new Error("Expected a string as first argument");
        if (typeof handler !== "function")
            throw new Error("Expected a function as second argument");

        super();

        this.#name = name;
        this.#handler = handler;

        ConsoleCommand.#init();
        ConsoleCommand.#addHandler(this);
    }

    destroy() {
        if (!super._tryDestroy())
            throw new Error(`ConsoleCommand '${this.#name}' already destroyed`);

        ConsoleCommand.#removeHandler(this);
    }
}

// Client only
if (alt.isClient && !alt.isWorker) {
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

    // Shortcut for alt.Object
    // TODO: Make client/server only bindings work
    alt.Object.prototype.waitForSpawn = function() {
        return alt.Utils.waitFor(() => this.scriptID !== 0);
    }

    alt.Utils.drawText2dThisFrame = function(
        text,
        pos2d = new alt.Vector2(0.5),
        font = 0,
        scale = 0.5,
        color = new alt.RGBA(255, 255, 255),
        outline = true,
        dropShadow = true,
    ) {
        if (typeof text !== "string")
            throw new Error("Expected a string as first argument");
        if (typeof pos2d.x !== "number" || typeof pos2d.y !== "number")
            throw new Error("Expected Vector2 as second argument");
        if (typeof font !== "number")
            throw new Error("Expected a number as third argument");
        if (typeof scale !== "number")
            throw new Error("Expected a number as fourth argument");
        if (
            typeof color.r !== "number" || typeof color.g !== "number" ||
            typeof color.b !== "number" || typeof color.a !== "number"
        ) throw new Error("Expected RGBA as fifth argument");
        if (typeof outline !== "boolean")
            throw new Error("Expected boolean as sixth argument");
        if (typeof dropShadow !== "boolean")
            throw new Error("Expected boolean as seventh argument");
    
        native.setTextFont(font);
        native.setTextProportional(false);
        native.setTextScale(scale, scale);
        native.setTextColour(...color.toArray());
        native.setTextEdge(2, 0, 0, 0, 150);
    
        if (outline) native.setTextOutline();
        if (dropShadow) {
            native.setTextDropshadow(0, 0, 0, 0, 255);
            native.setTextDropShadow();
        }
    
        native.setTextCentre(true);
        native.beginTextCommandDisplayText("CELL_EMAIL_BCON");
        // Split text into pieces of max 99 chars blocks
        (text.match(/.{1,99}/g))?.forEach((textBlock) => {
            native.addTextComponentSubstringPlayerName(textBlock);
        });
    
        native.endTextCommandDisplayText(pos2d.x, pos2d.y, 0);
    }
    
    alt.Utils.drawText2d = function(
        text,
        pos2d,
        font,
        scale,
        color,
        outline,
        dropShadow,
    ) {
        return new alt.Utils.EveryTick(() => {
            alt.Utils.drawText2dThisFrame(text, pos2d, font, scale, color, outline, dropShadow);
        });
    }
    
    alt.Utils.drawText3dThisFrame = function(
        text,
        pos3d,
        font = 0,
        scale = 0.5,
        color = new alt.RGBA(255, 255, 255),
        outline = true,
        dropShadow = true,
    ) {
        if (typeof text !== "string")
            throw new Error("Expected a string as first argument");
        if (typeof pos3d.x !== "number" || typeof pos3d.y !== "number" || typeof pos3d.z !== "number")
            throw new Error("Expected Vector3 as second argument");
        if (typeof font !== "number")
            throw new Error("Expected a number as third argument");
        if (typeof scale !== "number")
            throw new Error("Expected a number as fourth argument");
        if (
            typeof color.r !== "number" || typeof color.g !== "number" || 
            typeof color.b !== "number" || typeof color.a !== "number"
        ) throw new Error("Expected RGBA as fifth argument");
        if (typeof outline !== "boolean")
            throw new Error("Expected boolean as sixth argument");
        if (typeof dropShadow !== "boolean")
            throw new Error("Expected boolean as seventh argument");
    
        native.setDrawOrigin(pos3d.x, pos3d.y, pos3d.z, 0);
        native.beginTextCommandDisplayText("STRING");
        native.addTextComponentSubstringPlayerName(text);
        native.setTextFont(font);
        native.setTextScale(1, scale);
        native.setTextWrap(0.0, 1.0);
        native.setTextCentre(true);
        native.setTextColour(...color.toArray());
    
        if (outline) native.setTextOutline();
        if (dropShadow) {
            native.setTextDropshadow(0, 0, 0, 0, 255);
            native.setTextDropShadow();
        }
    
        native.endTextCommandDisplayText(0, 0, 0);
        native.clearDrawOrigin();
    }
    
    alt.Utils.drawText3d = function(
        text,
        pos3d,
        font,
        scale,
        color,
        outline,
        dropShadow,
    ) {
        return new alt.Utils.EveryTick(() => {
            alt.Utils.drawText3dThisFrame(text, pos3d, font, scale, color, outline, dropShadow);
        });
    }

    alt.Utils.loadMapArea = async function(pos, radius = 50.0, timeout = 5000) {
        if (pos != null && typeof pos.x !== "number" || typeof pos.y !== "number" || typeof pos.z !== "number")
            throw new Error("Expected Vector3 as first argument");
        if (typeof radius !== "number")
            throw new Error("Expected a number as second argument");
        if (typeof timeout !== "number")
            throw new Error("Expected a number as third argument");

        alt.FocusData.overrideFocus(pos);
        native.newLoadSceneStop();
        native.newLoadSceneStartSphere(pos.x, pos.y, pos.z, radius, 0);
    
        try {
            await alt.Utils.waitFor(native.isNewLoadSceneLoaded, timeout);
        } catch (e) {
            throw new Error(`Failed to load map area pos: { x: ${pos.x.toFixed(2)}, y: ${pos.y.toFixed(2)}, z: ${pos.z.toFixed(2)} }`);
        } finally {
            alt.FocusData.clearFocus();
        }
    }

    alt.Utils.Keybind = class Keybind extends BaseUtility {
        /**
         * @type {Map<number, Set<() => void>>}
         */
        static #keyupHandlers = null;
        static #keydownHandlers = null;

        static #init(instance) {
            switch (instance.#eventType) {
                case "keyup":
                    if (this.#keyupHandlers) return;
                    this.#keyupHandlers = new Map();

                    alt.on("keyup", (keyCode) => {
                        Keybind.#keyupHandlers
                            .get(keyCode)
                            ?.forEach(h => h());
                    });

                    break;

                case "keydown":
                    if (this.#keydownHandlers) return;
                    this.#keydownHandlers = new Map();

                    alt.on("keydown", (keyCode) => {
                        Keybind.#keydownHandlers
                            .get(keyCode)
                            ?.forEach(h => h());
                    });

                    break;

                default:
                    throw new Error(`Unknown eventType: ${instance.#eventType}`);
            }
        }

        static #getHandlers(eventType, keyCode) {
            let allHandlers;

            switch (eventType) {
                case "keyup":
                    allHandlers = Keybind.#keyupHandlers;
                    break;

                case "keydown":
                    allHandlers = Keybind.#keydownHandlers;
                    break;

                default:
                    throw new Error(`Unknown eventType: ${eventType}`);
            }

            const handlers = allHandlers.get(keyCode) ?? new Set();
            allHandlers.set(keyCode, handlers);

            return handlers;
        }

        static #addHandler(instance) {
            for (const keyCode of instance.#keyCodes) {
                Keybind
                    .#getHandlers(instance.#eventType, keyCode)
                    .add(instance.#handler);
            }
        }

        static #removeHandler(instance) {
            for (const keyCode of instance.#keyCodes) {
                Keybind
                    .#getHandlers(instance.#eventType, keyCode)
                    .delete(instance.#handler);
            }
        }

        #eventType = "";
        #keyCodes = [];
        #handler = () => { };

        constructor(keyCode, handler, eventType = "keyup") {
            if (!(typeof keyCode === "number" || Array.isArray(keyCode)))
                throw new Error("Expected a number or array as first argument");
            if (typeof handler !== "function")
                throw new Error("Expected a function as second argument");
            if (typeof eventType !== "string")
                throw new Error("Expected a string as third argument");

            super();

            this.#keyCodes = Array.isArray(keyCode) ? keyCode : [keyCode];
            this.#handler = handler;
            this.#eventType = eventType;

            Keybind.#init(this);
            Keybind.#addHandler(this);
        }

        destroy() {
            if (!super._tryDestroy())
                throw new Error(`Keybind keyCodes: [${this.#keyCodes.join(", ")}] already destroyed`);

            Keybind.#removeHandler(this);
        }
    }

    alt.Utils.setVehicleTurboPtfx = function(scriptID, toggle) {
        if (typeof scriptID !== "number")
            throw new Error("Expected a number as first argument");
        if (typeof toggle !== "boolean")
            throw new Error("Expected a boolean as second argument");

        native.requestNamedPtfxAsset('veh_xs_vehicle_mods');
        native.setVehicleNitroEnabled(scriptID, toggle, 0.0, 0.0, 0, true);
    }
}
// Server only
else {

}
