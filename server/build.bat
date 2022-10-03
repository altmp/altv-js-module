@echo off

mkdir build
pushd build
cmake . -A x64 -DJS_MODULE_VERSION=DEV ..
cmake --build . --config Release
popd

mkdir dist
copy build\Release\js-module.dll dist
copy build\Release\js-module.pdb dist
copy deps\nodejs\lib\Release\libnode.dll dist
