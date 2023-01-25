# Updating V8

The [clientside module](/client) uses the [V8 runtime](https://v8.dev) to run JS scripts.

Sometimes it may be necessary to update the V8 runtime to a newer version.

This guide will explain how to build the V8 runtime, update it in the client module and test it.

> Updating V8 is something usually done by the module maintainer(s), **do not** try to update V8 before
> getting approval by a module maintainer.

## Building V8

Building V8 is the hardest part of updating the runtime in the client.

As V8 is a google product, building it has many prerequisites and requires way too many steps.

Unfortunately, there isn't really a way around it.

Fortunately, the V8 blog has a detailed guide on how to build V8. We will follow that guide, to build it.

### Checking out the V8 source repository

First we follow the guide on [checking out the V8 source code](https://v8.dev/docs/source-code),
the instructions there should be pretty clear.

Now we have the V8 source code available, and the next step is to check out the correct branch
in the repository.
We always want to checkout the actual release branch of a version, and **not** a pre-release version.

V8 publishes an article for every minor version increase of V8, so we can check [their blog](https://v8.dev/blog)
for the article for the newest V8 version, and at the bottom of that article we can find the correct branch to checkout for that version.
For example, for [V8 version 9.5](https://v8.dev/blog/v8-release-95#v8-api) we can see that the correct branch is `9.5`.

To make sure the correct branch is checked out, open `include/v8-version.h` in the repository, and check the if the specified version there
is the correct version. And also make sure that the `V8_IS_CANDIDATE_VERSION` is `0`, otherwise it is a pre-release version.

As a final step for checking out the repository, follow the [V8 guide](https://v8.dev/docs/build#installing-build-dependencies) on how to install the needed dependencies.

### Building the V8 source code

Now we have the correct V8 source code checked out and can begin actually building it.

We will [build V8 manually using GN](https://v8.dev/docs/build-gn).
First install [GN](https://gn.googlesource.com/gn/) from Google to your machine, then we can start the manual workflow of building V8.

First, we have to make sure the binaries are compiled with `/MD` instead of the default `/MT`.
The clientside module uses the dynamic runtime so we need to also build the V8 binaries with dynamic runtime.

To do this, we go to `build/config/win/BUILD.gn` in the V8 repo and open that file and go to line `481`.
That line should look something like this:
```gn
configs = [ ":static_crt" ]
```
If not, search for the `# Desktop Windows: static CRT` comment, the line right below is what we are looking for.
We change that line to this:
```gn
configs = [ ":dynamic_crt" ]
```
And save the file.
Now we have made sure that the binaries will be compiled with `/MD` and work with our client module.

Next we generate the needed build files with GN, we do this by invoking the following command:
```sh
gn args out/release
```

> Make sure to be in the **main folder** of the repository when executing this command

This will open a new file in a text editor automatically, into this file we will need to copy the
needed build arguments. The build arguments used for building V8 for the client module,
can be found in [this file](/client/deps/v8/args-release.gn).
Just copy the contents of that file into the file automatically opened by GN, save it and close the editor.

Now the build files have been generated, and we can start compiling.

To start compilation we use the following command:
```sh
ninja -C out/release v8_monolith
```

> Make sure [Ninja](https://ninja-build.org/) is installed on your machine before running this command

Hopefully, after a while (can take up to 2 hours) the build is finished without errors.

We can now find the built binaries in `out/release/obj`. There, we should find the `v8_monolith.lib`.

### `obj\\v8_heap_base_headers.lib` can't be opened

If you get this error during compilation, go to the `BUILD.gn` file in the root of the v8 repository
and change the line `v8_source_set("v8_heap_base_headers")` to `v8_header_set("v8_heap_base_headers")`.

Then you just need to generate the build files again with `gn args out/release` and start compilation again.
(Don't worry, the compilation progress until the error occured is saved and you don't need to compile V8 from the start again.)

## Updating V8 in the client module

Now we have built V8 and have the new built binary for our new V8 version.

The next steps now are easy, first we copy the `v8_monolith.lib` we just built into the dependencies of the client module.
We replace the existing file in the `client/deps/v8/lib/Release` directory, with our new binary.

Next, we copy the whole `include` folder in our V8 source repository, into the `client/deps/v8/include` directory.

### Testing the update

The binary and includes are now updated, so the next step is to try [building the client module](building.md#client).

If we are lucky, the build just succeeds without any errors. But sometimes there will of course be API changes from V8,
so make sure to fix all the errors from updating V8 and trying to build again.

When we have successfully built the client module, we can now [test it by starting the alt:V client with our new client JS binary](building.md#testing-1).

After the client has started, open the debug console using `F8` and enter the following command:
```sh
js-module --version
```

The console output now shows the used versions by the module, the V8 version shown there, should now be the new V8 version we just built.

If not, make sure you correctly built the module, and correctly started the client with the custom client module.

## Final Steps

We have now successfully updated V8 in the client module!

Make sure to check the generated warning by the compiler, to see if any deprecations have appeared, and adjust the code to not use the deprecated code,
so the next V8 update will go smoothly.

After building V8 once, every subsequent build will be **much** easier, because the annoying first steps of properly checking out the source code
aren't needed anymore.
