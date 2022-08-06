/// <reference path="../../../bindings.d.ts"/>
// clang-format off
// Object JS bindings

alt.Object.prototype.waitForSpawn = function() {
    return alt.Utils.waitFor(() => this.isSpawned, 99999999);
}
