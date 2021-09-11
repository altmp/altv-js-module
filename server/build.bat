@echo off
mkdir build
pushd build
cmake -G"Visual Studio 16" -A x64 ..
cmake --build . --config Release
popd

mkdir dist
copy build\Release\js-module.dll dist
copy deps\nodejs\lib\Release\libnode.dll dist
