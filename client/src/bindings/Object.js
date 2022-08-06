/// <reference path="../../../bindings.d.ts"/>
// clang-format off
// Object JS bindings

alt.Object.waitForSpawn = function() {
    return alt.Utils.waitFor(() => this.isSpawned);
}
