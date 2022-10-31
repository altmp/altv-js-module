/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Vector3 JS bindings

function getXYZFromArgs(args) {
    alt.Utils.assert(args.length === 1 || args.length === 3, "1 or 3 arguments expected");

    const firstArg = args[0];
    let x = 0, y = 0, z = 0;

    if(args.length === 3) {
        x = parseFloat(firstArg);
        y = parseFloat(args[1]);
        z = parseFloat(args[2]);

        assertNotNaN(x, "Expected a number as first argument");
        assertNotNaN(y, "Expected a number as second argument");
        assertNotNaN(z, "Expected a number as third argument");
    }
    else {
        if(typeof firstArg === "number" || typeof firstArg === "string") {
            const number = parseFloat(firstArg);
            assertNotNaN(number, "Expected a number or string as first argument");

            x = number;
            y = number;
            z = number;
        }
        else if(Array.isArray(firstArg)) {
            if(typeof firstArg[0] === "number" || typeof firstArg[0] === "string") {
                x = parseFloat(firstArg[0]);
                assertNotNaN(z, "Expected an array of 3 numbers as first argument");
            }
            if(typeof firstArg[1] === "number" || typeof firstArg[1] === "string") {
                y = parseFloat(firstArg[1]);
                assertNotNaN(y, "Expected an array of 3 numbers as first argument");
            }
            if(typeof firstArg[2] === "number" || typeof firstArg[2] === "string") {
                z = parseFloat(firstArg[2]);
                assertNotNaN(z, "Expected an array of 3 numbers as first argument");
            }
        }
        else if(firstArg && typeof firstArg === "object") {
            if(firstArg.x !== undefined) {
                x = parseFloat(firstArg.x);
                assertNotNaN(x, "Expected Vector3 as first argument");
            }
            if(firstArg.y !== undefined) {
                y = parseFloat(firstArg.y);
                assertNotNaN(y, "Expected Vector3 as first argument");
            }
            if(firstArg.z !== undefined) {
                z = parseFloat(firstArg.z);
                assertNotNaN(z, "Expected Vector3 as first argument");
            }
        }
        else throw new Error("Argument must be a number, an array of 3 numbers or IVector3");
    }

    return [x, y, z];
}

// Static properties
alt.Vector3.zero = new alt.Vector3(0, 0, 0);
alt.Vector3.one = new alt.Vector3(1, 1, 1);
alt.Vector3.back = new alt.Vector3(0, -1, 0);
alt.Vector3.up = new alt.Vector3(0, 0, 1);
alt.Vector3.down = new alt.Vector3(0, 0, -1);
alt.Vector3.forward = new alt.Vector3(0, 1, 0);
alt.Vector3.left = new alt.Vector3(-1, 0, 0);
alt.Vector3.right = new alt.Vector3(1, 0, 0);
alt.Vector3.negativeInfinity = new alt.Vector3(-Infinity, -Infinity, -Infinity);
alt.Vector3.positiveInfinity = new alt.Vector3(Infinity, Infinity, Infinity);

// Static getters
Object.defineProperty(alt.Vector3.prototype, "length", {
    get: function() {
        return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
    }
});

// Instance methods
alt.Vector3.prototype.toString = function() {
    return `Vector3{ x: ${this.x.toFixed(4)}, y: ${this.y.toFixed(4)}, z: ${this.z.toFixed(4)} }`;
}

alt.Vector3.prototype.toArray = function() {
    return [this.x, this.y, this.z];
}

alt.Vector3.prototype.add = function(...args) {
    const [x, y, z] = getXYZFromArgs(args);
    return new alt.Vector3(this.x + x, this.y + y, this.z + z);
}

alt.Vector3.prototype.sub = function(...args) {
    const [x, y, z] = getXYZFromArgs(args);
    return new alt.Vector3(this.x - x, this.y - y, this.z - z);
}

alt.Vector3.prototype.div = function(...args) {
    const [x, y, z] = getXYZFromArgs(args);
    return new alt.Vector3(this.x / x, this.y / y, this.z / z);
}

alt.Vector3.prototype.mul = function(...args) {
    const [x, y, z] = getXYZFromArgs(args);
    return new alt.Vector3(this.x * x, this.y * y, this.z * z);
}

alt.Vector3.prototype.dot = function(...args) {
    const [x, y, z] = getXYZFromArgs(args);
    return (this.x * x) + (this.y * y) + (this.z * z);
}

alt.Vector3.prototype.cross = function(...args) {
    const [x, y, z] = getXYZFromArgs(args);
    return new alt.Vector3((this.y * z) - (this.z * y), (this.z * x) - (this.x * z), (this.x * y) - (this.y * x));
}

alt.Vector3.prototype.negative = function() {
    return new alt.Vector3(-this.x, -this.y, -this.z);
}

alt.Vector3.prototype.inverse = function() {
    return new alt.Vector3(1 / this.x, 1 / this.y, 1 / this.z);
}

alt.Vector3.prototype.normalize = function() {
    const length = this.length;
    return new alt.Vector3(this.x / length, this.y / length, this.z / length);
}

alt.Vector3.prototype.distanceTo = function(vector) {
    return Math.sqrt(this.distanceToSquared(vector));
}

alt.Vector3.prototype.distanceToSquared = function(vector) {
    alt.Utils.assert(vector != null, "Expected Vector3 as first argument");

    const x = this.x - parseFloat(vector.x);
    const y = this.y - parseFloat(vector.y);
    const z = this.z - parseFloat(vector.z);
    return x * x + y * y + z * z;
}

alt.Vector3.prototype.angleTo = function(vector) {
    alt.Utils.assert(vector != null, "Expected Vector3 as first argument");

    const posALength = Math.hypot(this.x, this.y);
    const posBLength = Math.hypot(vector.x, vector.y);
    if (posALength === 0 || posBLength === 0) throw new Error("Division by zero");

    return Math.acos((this.x * vector.x + this.y * vector.y) / (posALength * posBLength));
}

alt.Vector3.prototype.angleToDegrees = function(vector) {
    return this.angleTo(vector) * (180 / Math.PI);
}

alt.Vector3.prototype.toDegrees = function() {
    return new alt.Vector3((this.x * 180) / Math.PI, (this.y * 180) / Math.PI, (this.z * 180) / Math.PI);
}

alt.Vector3.prototype.toRadians = function() {
    return new alt.Vector3((this.x * Math.PI) / 180, (this.y * Math.PI) / 180, (this.z * Math.PI) / 180);
}

alt.Vector3.prototype.isInRange = function(vector, range) {
    alt.Utils.assert(vector != null, "Expected Vector3 as first argument");
    alt.Utils.assert(typeof range === "number", "Expected a number as second argument");
    
    const x = Math.abs(this.x - parseFloat(vector.x));
    const y = Math.abs(this.y - parseFloat(vector.y));
    const z = Math.abs(this.z - parseFloat(vector.z));

    return x <= range && y <= range && z <= range  // Fast check
        && x * x + y * y + z * z <= range * range; // Slow check
}

alt.Vector3.prototype.lerp = function(vector, ratio) {
    alt.Utils.assert(vector != null, "Expected Vector3 as first argument");
    alt.Utils.assert(typeof ratio === "number", "Expected a number as second argument");

    const x = this.x + (parseFloat(vector.x) - this.x) * ratio;
    const y = this.y + (parseFloat(vector.y) - this.y) * ratio;
    const z = this.z + (parseFloat(vector.z) - this.z) * ratio;
    return new alt.Vector3(x, y, z);
}
