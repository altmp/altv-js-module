mkdir build-linux
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-8 -DJS_MODULE_VERSION=DEV ..
cmake --build . --config Release
cd ..
