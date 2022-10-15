@echo off

:: If 1, builds dynamic library, otherwise builds static library
set DYNAMIC_BUILD=1
IF NOT "%1" == "" (
    set DYNAMIC_BUILD=%1
)

:: Set version
set VERSION=DEV
IF NOT "%2" == "" (
    set VERSION=%2
)

IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake . -A x64 -G "Visual Studio 17 2022" -DJS_MODULE_VERSION=%VERSION% -DDYNAMIC_BUILD=%DYNAMIC_BUILD% ..
cmake --build . --config Release
popd

:: Copy built binary to dist folder
IF NOT EXIST dist\ (
    mkdir dist
)

IF %DYNAMIC_BUILD% == 1 (
    copy build\Release\altv-client-js.dll dist
    copy build\Release\altv-client-js.pdb dist
) ELSE (
    copy build\Release\altv-client-js.lib dist
)
