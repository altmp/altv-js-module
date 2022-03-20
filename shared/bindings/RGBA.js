/// <reference path="../../bindings.d.ts"/>
// clang-format off
// RGBA JS bindings

// Static properties
alt.RGBA.red = new alt.RGBA(255, 0, 0, 255);
alt.RGBA.green = new alt.RGBA(0, 255, 0, 255);
alt.RGBA.blue = new alt.RGBA(0, 0, 255, 255);
alt.RGBA.black = new alt.RGBA(0, 0, 0, 255);
alt.RGBA.white = new alt.RGBA(255, 255, 255, 255);
alt.RGBA.clear = new alt.RGBA(0, 0, 0, 0);

// Instance methods
alt.RGBA.prototype.toString = function() {
    return `RGBA{ r: ${this.r}, g: ${this.g}, b: ${this.b}, a: ${this.a} }`;
}

alt.RGBA.prototype.toBGRA = function() {
    return new alt.RGBA(this.b, this.g, this.r, this.a);
}

alt.RGBA.prototype.toARGB = function() {
    return new alt.RGBA(this.a, this.r, this.g, this.b);
}

alt.RGBA.prototype.toInt = function() {
    let int = this.r << 24;
    int |= this.g << 16;
    int |= this.b << 8;
    int |= this.a;
    return int;
}
