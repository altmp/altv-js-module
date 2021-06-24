@echo off

:: Build the project
cmake . -BBUILD
cmake --build BUILD --config Release

:: Copy built binary to dist folder
IF NOT EXIST dist (
    mkdir dist
)
copy BUILD\Release\altv-client-js.dll dist
copy BUILD\Release\altv-client-js.lib dist
copy BUILD\Release\altv-client-js.pdb dist
