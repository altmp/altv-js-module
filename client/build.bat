@echo off

node ../tools/convert-bindings.js ..

:: Build the project
cmake . -BBUILD
cmake --build BUILD --config Release

:: Copy built binary to dist folder
IF NOT EXIST dist (
    mkdir dist
)
copy BUILD\Release\altv-client-js-static.lib dist
copy BUILD\Release\altv-client-js-static.pdb dist
