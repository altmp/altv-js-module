// clang-format off
(async () => {
  const alt = process._linkedBinding('alt');
  const path = require('path');
  const { esmLoader: loader } = require('internal/process/esm_loader');
  const { translators } = require('internal/modules/esm/translators');

  let _exports = null;

  try {
    // Supress the annoying warning from NodeJS
    const __emitWarning = process.emitWarning;
    process.emitWarning = () => {};
    const { ModuleWrap } = require('internal/test/binding').internalBinding('module_wrap');
    process.emitWarning = __emitWarning;

    // Set our custom translator for the 'alt' protocol that loads alt:V resources
    translators.set('alt', async function(url) {
      const name = url.slice(4); // Remove 'alt:' scheme
      const exports = process._linkedBinding('alt').getResourceExports(name);
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

    loader.addCustomLoaders({
        resolve(specifier, context, defaultResolve) {
            if (alt.hasResource(specifier)) return {
                url: 'alt:' + specifier
            };
            return defaultResolve(specifier, context, defaultResolve);
        },
        load(url, context, defaultLoad) {
            if(url.startsWith('alt:')) {
                return {
                    format: 'alt',
                    source: null,
                }
            }
            return defaultLoad(url, context, defaultLoad);
        },
    });

    // Get the path to the main file for this resource, and load it
    const _path = path.resolve(alt.getResourcePath(alt.resourceName), alt.getResourceMain(alt.resourceName));
    _exports = await loader.import("file://" + _path, "", {});
    if ('start' in _exports) {
      const start = _exports.start;
      if (typeof start === 'function') {
        await start();
      }
    }
  } catch (e) {
    console.error(e);
  }

  __resourceLoaded(alt.resourceName, _exports);
})();
