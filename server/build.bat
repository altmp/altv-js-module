@echo off

:: Set version
set VERSION=DEV
IF NOT "%1" == "" (
    set VERSION=%1
)


IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake . -A x64 -G "Visual Studio 17 2022" -DJS_MODULE_VERSION=%VERSION% ..
cmake --build . --config Release
popd

IF NOT EXIST dist\ (
    mkdir dist
)
copy build\Release\js-module.dll dist
copy build\Release\js-module.pdb dist
copy deps\nodejs\lib\Release\libnode.dll dist
