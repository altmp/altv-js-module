@echo off

node ../tools/convert-bindings.js ..

:: Build the project
cmake . -A x64 -BBUILD -DDYNAMIC_BUILD=1
cmake --build BUILD --config Release

:: Copy built binary to dist folder
IF NOT EXIST dist (
    mkdir dist
)
copy BUILD\Release\altv-client-js.dll dist
copy BUILD\Release\altv-client-js.pdb dist
