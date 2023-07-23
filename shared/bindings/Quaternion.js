/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Quaternion JS bindings

/*
 * Can be used if someone wants to add .add, .sub, .div etc.
function getXYZWFromArgs(args) {
    alt.Utils.assert(args.length === 1 || args.length === 4, "1 or 4 arguments expected");

    const firstArg = args[0];
    let x = 0, y = 0, z = 0, w = 0;

    if(args.length === 4) {
        x = parseFloat(firstArg);
        y = parseFloat(args[1]);
        z = parseFloat(args[2]);
        w = parseFloat(args[3]);

        assertNotNaN(x, "Expected a number as first argument");
        assertNotNaN(y, "Expected a number as second argument");
        assertNotNaN(z, "Expected a number as third argument");
        assertNotNaN(w, "Expected a number as fourth argument");
    }
    else {
        if(typeof firstArg === "number" || typeof firstArg === "string") {
            const number = parseFloat(firstArg);
            assertNotNaN(number, "Expected a number or string as first argument");

            x = number;
            y = number;
            z = number;
            w = number;
        }
        else if(Array.isArray(firstArg)) {
            if(typeof firstArg[0] === "number" || typeof firstArg[0] === "string") {
                x = parseFloat(firstArg[0]);
                assertNotNaN(x, "Expected an array of 4 numbers as first argument");
            }
            if(typeof firstArg[1] === "number" || typeof firstArg[1] === "string") {
                y = parseFloat(firstArg[1]);
                assertNotNaN(y, "Expected an array of 4 numbers as first argument");
            }
            if(typeof firstArg[2] === "number" || typeof firstArg[2] === "string") {
                z = parseFloat(firstArg[2]);
                assertNotNaN(z, "Expected an array of 4 numbers as first argument");
            }
            if(typeof firstArg[3] === "number" || typeof firstArg[3] === "string") {
                w = parseFloat(firstArg[3]);
                assertNotNaN(w, "Expected an array of 4 numbers as first argument");
            }
        }
        else if(firstArg && typeof firstArg === "object") {
            if(firstArg.x !== undefined) {
                x = parseFloat(firstArg.x);
                assertNotNaN(x, "Expected Quaternion as first argument");
            }
            if(firstArg.y !== undefined) {
                y = parseFloat(firstArg.y);
                assertNotNaN(y, "Expected Quaternion as first argument");
            }
            if(firstArg.z !== undefined) {
                z = parseFloat(firstArg.z);
                assertNotNaN(z, "Expected Quaternion as first argument");
            }
            if(firstArg.w !== undefined) {
                w = parseFloat(firstArg.w);
                assertNotNaN(w, "Expected Quaternion as first argument");
            }
        }
        else throw new Error("Argument must be a number, an array of 4 numbers or IQuaternion");
    }

    return [x, y, z, w];
}
 */

// Static properties
alt.Quaternion.zero = new alt.Quaternion(0, 0, 0, 0);
alt.Quaternion.one = new alt.Quaternion(1, 1, 1, 1);

// Instance methods
alt.Quaternion.prototype.toString = function() {
    return `Quaternion{ x: ${this.x.toFixed(4)}, y: ${this.y.toFixed(4)}, z: ${this.z.toFixed(4)}, w: ${this.w.toFixed(4)} }`;
}

alt.Quaternion.prototype.toArray = function() {
    return [this.x, this.y, this.z, this.w];
}

alt.Quaternion.prototype.toFixed = function(precision = 4) {
    return new alt.Quaternion(
        parseFloat(this.x.toFixed(precision)),
        parseFloat(this.y.toFixed(precision)),
        parseFloat(this.z.toFixed(precision)),
        parseFloat(this.w.toFixed(precision)),
    );
}
