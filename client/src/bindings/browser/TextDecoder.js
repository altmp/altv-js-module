// clang-format off

alt.TextDecoder.prototype.decode = function(input = new Uint8Array(0), options = {}) {
    if(this.__encoding !== "utf-8") return;

    // todo: don't ignore options

    // Copilot definitely didn't write this for me
    let str = "";
    for (let i = 0; i < input.length; i++) {
        const byte = input[i];
        if (byte < 128) {
            str += String.fromCharCode(byte);
        } else if (byte > 191 && byte < 224) {
            str += String.fromCharCode(((byte & 31) << 6) | (input[i + 1] & 63));
            i++;
        } else {
            str += String.fromCharCode(((byte & 15) << 12) | ((input[i + 1] & 63) << 6) | (input[i + 2] & 63));
            i += 2;
        }
    }

    return str;
};

globalThis.TextDecoder = alt.TextDecoder;
