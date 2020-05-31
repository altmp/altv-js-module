@echo off
if not exist build mkdir build
cd build
cmake -G"Visual Studio 16" -A x64 ..
cd ../