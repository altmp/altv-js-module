mkdir build-linux
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release -DJS_MODULE_VERSION=DEV ..
cmake --build . --config Release
cd ..
