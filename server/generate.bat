@echo off
if not exist build mkdir build
cd build
cmake . -A x64 -DJS_MODULE_VERSION="internal" ..
cd ../