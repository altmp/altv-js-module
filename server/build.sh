mkdir build-linux
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-9 -DJS_MODULE_VERSION=DEV ..
cmake --build . --config Release
cd ..

mkdir dist
cp ./build-linux/libjs-module.so dist
cp ./deps/nodejs/lib/libnode.so.102 dist
