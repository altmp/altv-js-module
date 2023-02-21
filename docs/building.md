# Building the module

To build the module there are only a few prerequisites needed:

- [*Windows*] Visual Studio 2019 or later installed (With the C++ extensions installed)
- [*Linux*] GCC 9 installed
- CMake installed

To simplify the build process, the module has build scripts on both sides which only need to be executed to fully build the module.

## Server

### Building
To build the server module simply run the `build.bat` / `build.sh` script.
> There are also VSCode tasks provided to run the build, to build the server simply run the
`Build server` task from VSCode.

The compiled `js-module.dll` will then be in the `dist` directory.

> Make sure the `libnode.dll` in your server directory is the same one as the one used for building

### Testing
Drag this file into your `modules` folder for your alt:V server and start the server.
The JS module should then be running, and by typing `js-module --version` it should show
you that the current JS module version is `DEV`.

> If it shows another version, you are using an official build instead of your custom build,
so make sure you have properly replaced the file.

## Client

### Building
To build the client module simply run the `build.bat` script.

Make sure that `DYNAMIC_BUILD` equals `1`. Otherwise it builds the static module,
which is only used internally.

> For the client module there is also a VSCode task available, to build it
simply run the `Build client` task from VSCode.

After running the script the compiled `altv-client-js.dll` will be in the `dist` directory,
along with the debug symbols to debug crashdumps in the client module.

### Testing
To use the custom client module build, create a folder named `modules` in your alt:V installation (where the `altv.exe` is located),
and *inside* that folder create another folder named `altv-client-js`.
Now you can drag the `altv-client-js.dll` into that folder.

Make sure you are on the `dev` branch and to enable **debug mode**, then start the client. A prompt should be displayed after the Rockstar launcher is shown, asking
whether you want to load the custom module. Press `Yes` here to start alt:V with your custom module build.
alt:V should now normally start, but you are now using the previously compiled custom module build.

> The client module does not have a Linux build script, because it can only be used on Windows, so building
it on Linux would be useless.


## Shared

This part cannot be built on its own. The shared part is a dependency of both the client- and serverside module,
and is included and built by them individually.

## Tips

### Symlinks
To make the development experience smoother when working on the module, it is recommended to create a *symlink* to the custom build,
instead of copying the file after every compilation.

A *symlink* can be created like this: (This should be ran inside the `modules` directory of your alt:V server)
```bash
mklink js-module.dll "C:/example/altv-js-module/server/dist/js-module.dll"
```

When you now compile your module and start the server, it will run with the new module, without needing any extra work.

> Make sure to stop the server before compilation finishes, otherwise you will get a file access error,
and the compiled module DLL won't be copied to the `dist` directory.

### Submodules
If you get errors about missing headers for the `cpp-sdk` make sure you have pulled the *submodules*.
You can do this by cloning the repo with the `--recurse-submodules` flag, or by running `git submodule init && git submodule update` in the repository directory.
