// clang-format off

if(!globalThis.console) globalThis.console = {};
globalThis.console.log = alt.log;
globalThis.console.warn = alt.logWarning;
globalThis.console.error = alt.logError;
globalThis.console.info = alt.log;
globalThis.console.debug = alt.logDebug;
globalThis.console.time = alt.time;
globalThis.console.timeEnd = alt.timeEnd;
