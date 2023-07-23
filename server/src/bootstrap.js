// clang-format off
const { esmLoader } = require("internal/process/esm_loader");
const { translators } = require("internal/modules/esm/translators");
const { ModuleWrap } = internalRequire("internal/test/binding").internalBinding("module_wrap");
const path = require("path");
const alt = process._linkedBinding("alt");
const dns = require('dns');
const url = require("url");

(async () => {
  const resource = alt.Resource.current;
  let _exports = null;

  // We need this handler so that NodeJS doesn't
  // crash the process on oncaught exceptions
  process.on("uncaughtException", (err) => {
    alt.logError(`Uncaught exception: ${err.stack ? `${err.stack}` : `${err.message}`}`);
  });

  // Allows users to use "localhost" address instead of 127.0.0.1 for tcp connections (e.g. database)
  // https://github.com/nodejs/node/issues/40702#issuecomment-958157082
  dns.setDefaultResultOrder('ipv4first');

  try {
    setupImports();

    // Load the global bindings code
    const bindingsGlobal = {};
    new Function("alt", "__global", __internal_bindings_code)(alt, bindingsGlobal);
    __setLogFunction(bindingsGlobal.genericLog);

    const extraBootstrapFile = __getExtraBootstrapFile();
    if(extraBootstrapFile.length !== 0) new Function("alt", extraBootstrapFile)(alt);

    // Get the path to the main file for this resource, and load it
    const _path = path.resolve(resource.path, resource.main);
    _exports = await esmLoader.import(url.pathToFileURL(_path).toString(), "", {});
    /* No one used this and only caused problems for people using that function name,
       so let's just remove it for now and see if anyone complains
    if ("start" in _exports) {
      const start = _exports.start;
      if (typeof start === "function") {
        await start();
      }
    }
    */
  } catch (e) {
    console.error(e);
  }

  __resourceLoaded(resource.name, _exports);
})();

// Sets up our custom way of importing alt:V resources
function setupImports() {
  const altResourceInternalPrefix = "altresource";
  const altResourceImportPrefix = "alt";

  translators.set(altResourceInternalPrefix, async function(url) {
    const name = url.slice(altResourceInternalPrefix.length + 1); // Remove prefix
    const exports = alt.Resource.getByName(name).exports;
    return new ModuleWrap(url, undefined, Object.keys(exports), function() {
      for (const exportName in exports) {
        let value;
        try {
          value = exports[exportName];
        } catch {}
        this.setExport(exportName, value);
      }
    });
  });

  const _warningPackages = {
    "node-fetch": "Console hangs"
  };
  const customLoaders = [{
    exports: {
      resolve(specifier, context, defaultResolve) {
        const hasAltPrefix = specifier.startsWith(`${altResourceImportPrefix}:`);
        if (alt.hasResource(specifier) && !hasAltPrefix) {
          alt.logWarning(`Trying to import resource '${specifier}' without '${altResourceImportPrefix}:' prefix, this is deprecated behaviour.`);
          alt.logWarning(`Import '${altResourceImportPrefix}:${specifier}' instead to silence this warning.`);
          return {
            url: `${altResourceInternalPrefix}:${specifier}`,
            shortCircuit: true
          };
        }
        const specifierWithoutPrefix = specifier.slice(altResourceImportPrefix.length + 1);
        if(hasAltPrefix) {
          if(!alt.hasResource(specifierWithoutPrefix)) {
            alt.logError(`Trying to import resource '${specifierWithoutPrefix}' that doesn't exist`);
          }
          else return {
            url: `${altResourceInternalPrefix}:${specifierWithoutPrefix}`,
            shortCircuit: true
          };
        }

        if(_warningPackages.hasOwnProperty(specifier)) alt.logWarning(`Using the module "${specifier}" can cause problems. Reason: ${_warningPackages[specifier]}`);
        return defaultResolve(specifier, context, defaultResolve);
      },
      load(url, context, defaultLoad) {
        if(url.startsWith(`${altResourceInternalPrefix}:`))
          return {
            format: altResourceInternalPrefix,
            source: null,
            shortCircuit: true
          };
        return defaultLoad(url, context, defaultLoad);
      },
    }
  }];
  esmLoader.addCustomLoaders(customLoaders);
}

// ***** Utils

// Supresses the warning from NodeJS when importing "super-internal" modules,
// that the embedder isn't supposed to use
function internalRequire(id) {
  const __emitWarning = process.emitWarning;
  process.emitWarning = () => {};
  const result = require(id);
  process.emitWarning = __emitWarning;
  return result;
}
