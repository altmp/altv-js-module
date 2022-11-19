/// <reference path="../../bindings.d.ts"/>
// clang-format off
// Vector2 JS bindings

function getXYFromArgs(args) {
    alt.Utils.assert(args.length === 1 || args.length === 2, "1 or 2 arguments expected");

    let x = 0, y = 0;

    if(args.length === 2) {
        x = parseFloat(args[0]);
        y = parseFloat(args[1]);

        assertNotNaN(x, "Expected a number as first argument");
        assertNotNaN(y, "Expected a number as second argument");
    }
    else {
        if(typeof args[0] === "number" || typeof args[0] === "string") {
            const number = parseFloat(args[0]);
            assertNotNaN(number, "Expected a number as first argument");

            x = number;
            y = number;
        }
        else if(Array.isArray(args[0])) {
            if(typeof args[0][0] === "number" || typeof args[0][0] === "string") {
                x = parseFloat(args[0][0]);
                assertNotNaN(x, "Expected an array of 2 numbers as first argument");
            }
            if(typeof args[0][1] === "number" || typeof args[0][1] === "string") {
                y = parseFloat(args[0][1]);
                assertNotNaN(y, "Expected an array of 2 numbers as first argument");
            }
        }
        else if(args[0] && typeof args[0] === "object") {
            if(args[0].x !== undefined) {
                x = parseFloat(args[0].x);
                assertNotNaN(x, "Expected Vector2 as first argument");
            }
            if(args[0].y !== undefined) {
                y = parseFloat(args[0].y);
                assertNotNaN(y, "Expected Vector2 as first argument");
            }
        }
        else throw new Error("Argument must be a number, an array of 2 numbers or IVector2");
    }

    return [x, y];
}

// Static properties
alt.Vector2.zero = new alt.Vector2(0, 0);
alt.Vector2.one = new alt.Vector2(1, 1);
alt.Vector2.up = new alt.Vector2(0, 1);
alt.Vector2.down = new alt.Vector2(0, -1);
alt.Vector2.left = new alt.Vector2(-1, 0);
alt.Vector2.right = new alt.Vector2(1, 0);
alt.Vector2.negativeInfinity = new alt.Vector2(-Infinity, -Infinity);
alt.Vector2.positiveInfinity = new alt.Vector2(Infinity, Infinity);

// Static getters
Object.defineProperty(alt.Vector2.prototype, "length", {
    get: function() {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }
});

// Instance methods
alt.Vector2.prototype.toString = function() {
    return `Vector2{ x: ${this.x.toFixed(4)}, y: ${this.y.toFixed(4)} }`;
}

alt.Vector2.prototype.toArray = function() {
    return [this.x, this.y];
}

alt.Vector2.prototype.toFixed = function (precision = 4) {
    return new alt.Vector2(
        parseFloat(this.x.toFixed(precision)),
        parseFloat(this.y.toFixed(precision)),
    );
};

alt.Vector2.prototype.add = function(...args) {
    const [x, y] = getXYFromArgs(args);
    return new alt.Vector2(this.x + x, this.y + y);
}

alt.Vector2.prototype.sub = function(...args) {
    const [x, y] = getXYFromArgs(args);
    return new alt.Vector2(this.x - x, this.y - y);
}

alt.Vector2.prototype.div = function(...args) {
    const [x, y] = getXYFromArgs(args);
    return new alt.Vector2(this.x / x, this.y / y);
}

alt.Vector2.prototype.mul = function(...args) {
    const [x, y] = getXYFromArgs(args);
    return new alt.Vector2(this.x * x, this.y * y);
}

alt.Vector2.prototype.negative = function() {
    return new alt.Vector2(-this.x, -this.y);
}

alt.Vector2.prototype.inverse = function() {
    return new alt.Vector2(1 / this.x, 1 / this.y);
}

alt.Vector2.prototype.normalize = function() {
    const length = this.length;
    return new alt.Vector2(this.x / length, this.y / length);
}

alt.Vector2.prototype.distanceTo = function(vector) {
    return Math.sqrt(this.distanceToSquared(vector));
}

alt.Vector2.prototype.distanceToSquared = function(vector) {
    alt.Utils.assert(vector != null, "Expected Vector2 as first argument");

    const x = this.x - parseFloat(vector.x);
    const y = this.y - parseFloat(vector.y);
    return x * x + y * y;
}

alt.Vector2.prototype.angleTo = function(vector) {
    alt.Utils.assert(vector != null, "Expected Vector2 as first argument");

    const posALength = Math.hypot(this.x, this.y);
    const posBLength = Math.hypot(vector.x, vector.y);
    if (posALength === 0 || posBLength === 0) throw new Error("Division by zero");
    return Math.acos((this.x * vector.x + this.y * vector.y) / (posALength * posBLength));
}

alt.Vector2.prototype.angleToDegrees = function(vector) {
    return this.angleTo(vector) * (180 / Math.PI);
}

alt.Vector2.prototype.toDegrees = function() {
    return new alt.Vector2((this.x * 180) / Math.PI, (this.y * 180) / Math.PI);
}

alt.Vector2.prototype.toRadians = function() {
    return new alt.Vector2((this.x * Math.PI) / 180, (this.y * Math.PI) / 180);
}

alt.Vector2.prototype.isInRange = function(vector, range) {
    alt.Utils.assert(vector != null, "Expected Vector2 as first argument");
    alt.Utils.assert(typeof range === "number", "Expected a number as second argument");

    const x = Math.abs(this.x - parseFloat(vector.x));
    const y = Math.abs(this.y - parseFloat(vector.y));

    return x <= range && y <= range        // Fast check
        && x * x + y * y <= range * range; // Slow check
}

alt.Vector2.prototype.lerp = function(vector, ratio) {
    alt.Utils.assert(vector != null, "Expected Vector2 as first argument");
    alt.Utils.assert(typeof ratio === "number", "Expected a number as second argument");

    const x = this.x + (vector.x - this.x) * ratio;
    const y = this.y + (vector.y - this.y) * ratio;
    return new alt.Vector2(x, y);
}
