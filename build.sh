mkdir build-linux && cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-8.4 ..
cmake --build . --config Release
cd ..
mkdir dist
cp ./build-linux/libnode-module.so dist/
cp ./deps/nodejs/lib/libnode.so dist/
