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

class BaseEvent extends BaseUtility {
    // should be set in static blocks by inherited classes
    static _altAddListener = null;
    static _altRemoveListener = null;

    #eventName;
    #handler;

    constructor(eventName, handler) {
        super();

        this.#eventName = eventName;
        this.#handler = handler;
        this.constructor._altAddListener(eventName, handler);
    }

    destroy() {
        alt.Utils.assert(super._tryDestroy(), "Event already destroyed");
        this.constructor._altRemoveListener(this.#eventName, this.#handler);
    }
}

alt.Utils.LocalEvent = class LocalEvent extends BaseEvent {
    static {
        this._altAddListener = alt.on;
        this._altRemoveListener = alt.off;
    }
}

alt.Utils.GenericLocalEvent = class GenericLocalEvent extends BaseEvent {
    static {
        this._altAddListener = (_, handler) => alt.on(handler);
        this._altRemoveListener = (_, handler) => alt.off(handler);
    }

    constructor(handler) {
        super(null, handler);
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

    #clearId() {
        // timer id starts from 1
        this.#id = 0;
    }

    destroy() {
        alt.Utils.assert(super._tryDestroy(), "Timer already destroyed");
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
    alt.Utils.assert(typeof timeout === "number", "Expected a number as first argument");

    return new Promise(resolve => {
        new alt.Utils.Timeout(resolve, timeout);
    });
}

alt.Utils.waitFor = function(callback, timeout = 2000) {
    alt.Utils.assert(typeof callback === "function", "Expected a function as first argument");
    alt.Utils.assert(typeof timeout === "number", "Expected a number as second argument");

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
        alt.Utils.assert(typeof name === "string", "Expected a string as first argument");
        alt.Utils.assert(typeof handler === "function", "Expected a function as second argument");

        super();

        this.#name = name;
        this.#handler = handler;

        ConsoleCommand.#init();
        ConsoleCommand.#addHandler(this);
    }

    destroy() {
        alt.Utils.assert(super._tryDestroy(), `ConsoleCommand '${this.#name}' already destroyed`);

        ConsoleCommand.#removeHandler(this);
    }
}

alt.Utils.AssertionError = class AssertionError extends Error {}
alt.Utils.assert = function(assertion, message) {
    if(!assertion) throw new alt.Utils.AssertionError(message ?? "Assertion failed");
}

// For convenience
function assertRGBA(val, message = "Expected RGBA") {
    return alt.Utils.assert(
        val && typeof val.r === "number" && typeof val.g === "number" && typeof val.b === "number" && typeof val.a === "number", 
        message
    );
}
function assertVector3(val, message = "Expected Vector3") {
    return alt.Utils.assert(
        val && typeof val.x === "number" && typeof val.y === "number" && typeof val.z === "number",
        message
    );
}
function assertVector2(val, message = "Expected Vector2") {
    return alt.Utils.assert(
        val && typeof val.x === "number" && typeof val.y === "number",
        message
    );
}
function assertDrawTextArgs(text, font, scale, color, outline, dropShadow) {
    alt.Utils.assert(typeof text === "string", "Expected a string as first argument");
    alt.Utils.assert(typeof font === "number", "Expected a number as third argument");
    alt.Utils.assert(typeof scale === "number", "Expected a number as fourth argument");
    assertRGBA(color, "Expected RGBA as fifth argument");
    alt.Utils.assert(typeof outline === "boolean", "Expected boolean as sixth argument");
    alt.Utils.assert(typeof dropShadow === "boolean", "Expected boolean as seventh argument");
}
function assertNotNaN(val, message = "Expected number") {
    alt.Utils.assert(!isNaN(val), message)
}

function getClosestEntityFromPool(getEntities, options = {}) {
    const {
        pos,
        range = Infinity,
    } = options;
    assertVector3(pos, "Expected Vector3 as pos option");
    assertNotNaN(range, "Expected a number as range option");

    let closestEntity = null;
    let closestDistance = Infinity;

    for (const entity of getEntities()) {
        const distance = pos.distanceTo(entity.pos);
        if (distance > range || distance > closestDistance) continue;

        closestEntity = entity;
        closestDistance = distance;
    }

    return closestEntity;
}

// Client only
if (alt.isClient && !alt.isWorker) {
    alt.Utils.requestModel = async function(model, timeout = 1000) {
        const _model = model;
        alt.Utils.assert(typeof model === "string" || typeof model === "number", "Expected a string or number as first argument");
        alt.Utils.assert(typeof timeout === "number", "Expected a number as second argument");

        if (typeof model === "string") model = alt.hash(model);
        alt.Utils.assert(native.isModelValid(model), typeof _model === "string"
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
        alt.Utils.assert(typeof animDict === "string", "Expected a string as first argument");
        alt.Utils.assert(typeof timeout === "number", "Expected a number as second argument");
        alt.Utils.assert(native.doesAnimDictExist(animDict), `Anim dict '${animDict}' not valid`);

        try {
            native.requestAnimDict(animDict);
            await alt.Utils.waitFor(() => native.hasAnimDictLoaded(animDict), timeout);
        } catch (e) {
            throw new Error(`Failed to request anim dict '${animDict}'`);
        }
    }

    alt.Utils.requestAnimSet = async function(animSet, timeout = 1000) {
        alt.Utils.assert(typeof animSet === "string", "Expected a string as first argument");
        alt.Utils.assert(typeof timeout === "number", "Expected a number as second argument");

        try {
            native.requestAnimSet(animSet);
            await alt.Utils.waitFor(() => native.hasAnimSetLoaded(animSet), timeout);
        } catch (e) {
            throw new Error(`Failed to request anim (clip) set '${animSet}'`);
        }
    }

    alt.Utils.requestClipSet = alt.Utils.requestAnimSet;

    alt.Utils.requestCutscene = async function(cutsceneName, flags, timeout = 1000) {
        alt.Utils.assert(typeof cutsceneName === "string", "Expected a string as first argument");
        alt.Utils.assert(typeof flags === "number" || typeof flags === "string", "Expected a number or string as second argument");
        alt.Utils.assert(typeof timeout === "number", "Expected a number as third argument");

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
        assertDrawTextArgs(text, font, scale, color, outline, dropShadow);
        assertVector2(pos2d, "Expected Vector2 as second argument");

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
        assertDrawTextArgs(text, font, scale, color, outline, dropShadow);
        assertVector3(pos3d, "Expected Vector3 as second argument");

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
        assertVector3(pos, "Expected Vector3 as first argument");
        alt.Utils.assert(typeof radius === "number", "Expected a number as second argument");
        alt.Utils.assert(typeof timeout === "number", "Expected a number as third argument");

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
            alt.Utils.assert(typeof keyCode === "number" || Array.isArray(keyCode), "Expected a number or array as first argument");
            alt.Utils.assert(typeof handler === "function", "Expected a function as second argument");
            alt.Utils.assert(typeof eventType === "string", "Expected a string as third argument");

            super();

            this.#keyCodes = Array.isArray(keyCode) ? keyCode : [keyCode];
            this.#handler = handler;
            this.#eventType = eventType;

            Keybind.#init(this);
            Keybind.#addHandler(this);
        }

        destroy() {
            alt.Utils.assert(super._tryDestroy(), `Keybind keyCodes: [${this.#keyCodes.join(", ")}] already destroyed`);

            Keybind.#removeHandler(this);
        }
    }

    alt.Utils.Marker = class Marker extends BaseUtility {
        #tick;
        #type;
        #pos;
        #dir;
        #rot;
        #scale;
        #color;
        #bobUpAndDown;
        #faceCamera;
        #p19;
        #rotate;
        #textureDict;
        #textureName;
        #drawOnEnts;

        constructor(
            pos,
            {
                type = 0,
                dir = alt.Vector3.zero,
                rot = alt.Vector3.zero,
                scale = alt.Vector3.one,
                color = new alt.RGBA(255, 255, 255),
                bobUpAndDown = false,
                faceCamera = false,
                p19 = 2,
                rotate = false,
                textureDict = undefined,
                textureName = undefined,
                drawOnEnts = false,
            } = {},
        ) {
            super();

            assertVector3(pos, "Expected Vector3 as first argument");
            alt.Utils.assert(typeof type === "number", "Expected a number for type option");
            assertVector3(dir, "Expected Vector3 for dir option");
            assertVector3(rot, "Expected Vector3 for rot option");
            assertVector3(scale, "Expected Vector3 for scale option");
            assertRGBA(color, "Expected RGBA for color option");
            alt.Utils.assert(typeof bobUpAndDown === "boolean", "Expected a boolean for bobUpAndDown option");
            alt.Utils.assert(typeof faceCamera === "boolean", "Expected a boolean for faceCamera option");
            alt.Utils.assert(typeof p19 === "number", "Expected a number for p19 option");
            alt.Utils.assert(typeof rotate === "boolean", "Expected a boolean for rotate option");
            alt.Utils.assert(typeof textureDict === "undefined" || typeof textureDict === "string", "Expected a string or undefined for textureDict option");
            alt.Utils.assert(typeof textureName === "undefined" || typeof textureName === "string", "Expected a string or undefined for textureName option");
            alt.Utils.assert(typeof drawOnEnts === "boolean", "Expected a boolean for drawOnEnts option");

            this.#pos = pos;
            this.#type = type;
            this.#dir = dir;
            this.#rot = rot;
            this.#scale = scale;
            this.#color = color;
            this.#bobUpAndDown = bobUpAndDown;
            this.#faceCamera = faceCamera;
            this.#p19 = p19;
            this.#rotate = rotate;
            this.#textureDict = textureDict;
            this.#textureName = textureName;
            this.#drawOnEnts = drawOnEnts;

            this.#tick = new alt.Utils.EveryTick(() => {
                native.drawMarker(
                    this.#type,
                    this.#pos.x,
                    this.#pos.y,
                    this.#pos.z,
                    this.#dir.x,
                    this.#dir.y,
                    this.#dir.z,
                    this.#rot.x,
                    this.#rot.y,
                    this.#rot.z,
                    this.#scale.x,
                    this.#scale.y,
                    this.#scale.z,
                    this.#color.r,
                    this.#color.g,
                    this.#color.b,
                    this.#color.a,
                    this.#bobUpAndDown,
                    this.#faceCamera,
                    this.#p19,
                    this.#rotate,
                    this.#textureDict,
                    this.#textureName,
                    this.#drawOnEnts,
                );
            });
        }

        get pos() {
            return this.#pos;
        }

        set pos(value) {
            assertVector3(value);

            this.#pos = value;
        }

        get type() {
            return this.#type;
        }

        set type(value) {
            alt.Utils.assert(typeof value === "number", "Expected a number");

            this.#type = value;
        }

        get dir() {
            return this.#dir;
        }

        set dir(value) {
            assertVector3(value);

            this.#dir = value;
        }

        get rot() {
            return this.#rot;
        }

        set rot(value) {
            assertVector3(value);

            this.#rot = value;
        }

        get scale() {
            return this.#scale;
        }

        set scale(value) {
            assertVector3(value);

            this.#scale = value;
        }

        get p19() {
            return this.#p19;
        }

        set p19(value) {
            alt.Utils.assert(typeof value === "number", "Expected a number");

            this.#p19 = value;
        }

        get color() {
            return this.#color;
        }

        set color(value) {
            assertRGBA(value);

            this.#color = value;
        }

        get bobUpAndDown() {
            return this.#bobUpAndDown;
        }

        set bobUpAndDown(value) {
            alt.Utils.assert(typeof value === "boolean", "Expected a boolean");

            this.#bobUpAndDown = value;
        }

        get faceCamera() {
            return this.#faceCamera;
        }

        set faceCamera(value) {
            alt.Utils.assert(typeof value === "boolean", "Expected a boolean");

            this.#faceCamera = value;
        }

        get rotate() {
            return this.#rotate;
        }

        set rotate(value) {
            alt.Utils.assert(typeof value === "boolean", "Expected a boolean");

            this.#rotate = value;
        }

        get drawOnEnts() {
            return this.#drawOnEnts;
        }

        set drawOnEnts(value) {
            alt.Utils.assert(typeof value === "boolean", "Expected a boolean");

            this.#drawOnEnts = value;
        }

        get textureDict() {
            return this.#textureDict;
        }

        set textureDict(value) {
            alt.Utils.assert(typeof value === "undefined" || typeof value === "string", "Expected a string or undefined");

            this.#textureDict = value;
        }

        get textureName() {
            return this.#textureName;
        }

        set textureName(value) {
            alt.Utils.assert(typeof value === "undefined" || typeof value === "string", "Expected a string or undefined");

            this.#textureName = value;
        }

        destroy() {
            alt.Utils.assert(super._tryDestroy(), "Marker already destroyed");

            this.#tick.destroy();
        }
    }

    async function registerPedheadshotBase64(headshotNative, ped) {
        let error = null;
        let headshot = null;
    
        try {
            headshot = headshotNative(ped);
            alt.Utils.assert(
                native.isPedheadshotValid(headshot),
                `Ped or player is invalid: ${ped} (headshot: ${headshot})`,
            );
    
            await alt.Utils.waitFor(() => native.isPedheadshotReady(headshot));
            return alt.getHeadshotBase64(headshot);
        } catch (cause) {
            error = new Error(`Failed to get ped headshot base64: ${headshot}`, { cause });
        } finally {
            native.unregisterPedheadshot(headshot);
        }
    
        throw error;
    }

    alt.Utils.registerPedheadshotBase64 = registerPedheadshotBase64.bind(null, native.registerPedheadshot);
    alt.Utils.registerPedheadshot3Base64 = registerPedheadshotBase64.bind(null, native.registerPedheadshot3);
    alt.Utils.registerPedheadshotTransparentBase64 = registerPedheadshotBase64.bind(null, native.registerPedheadshotTransparent);

    const getClosestEntity = (getEntities) => (options = {}) => {
        return getClosestEntityFromPool(
            getEntities, 
            {
                pos: options.pos ?? alt.Player.local.pos,
                ...options
            }
        );
    };
    
    alt.Utils.getClosestVehicle = getClosestEntity(() => alt.Vehicle.streamedIn);
    alt.Utils.getClosestPlayer = getClosestEntity(() => alt.Player.streamedIn);
    alt.Utils.getClosestWorldObject = getClosestEntity(() => alt.Object.allWorld);

    // TODO: change it to .streamedIn when serverside api will be added
    alt.Utils.getClosestObject = getClosestEntity(() => alt.Object.all);

    alt.Utils.ServerEvent = class ServerEvent extends BaseEvent {
        static {
            this._altAddListener = alt.onServer;
            this._altRemoveListener = alt.offServer;
        }
    }

    alt.Utils.GenericServerEvent = class GenericServerEvent extends BaseEvent {
        static {
            this._altAddListener = (_, handler) => alt.onServer(handler);
            this._altRemoveListener = (_, handler) => alt.offServer(handler);
        }
    
        constructor(handler) {
            super(null, handler);
        }
    }
}
// Server only
else {
    const getClosestEntity = (getEntities) =>
        getClosestEntityFromPool.bind(null, getEntities);

    alt.Utils.getClosestVehicle = getClosestEntity(() => alt.Vehicle.all);
    alt.Utils.getClosestPlayer = getClosestEntity(() => alt.Player.all);

    alt.Utils.ClientEvent = class ClientEvent extends BaseEvent {
        static {
            this._altAddListener = alt.onClient;
            this._altRemoveListener = alt.offClient;
        }
    }

    alt.Utils.GenericClientEvent = class GenericClientEvent extends BaseEvent {
        static {
            this._altAddListener = (_, handler) => alt.onClient(handler);
            this._altRemoveListener = (_, handler) => alt.offClient(handler);
        }

        constructor(handler) {
            super(null, handler);
        }
    }
}
