# Set version
VERSION=DEV
if [ $# -eq 1 ]; then
    VERSION=$1
fi

if [ ! -d "./build-linux" ]; then
    mkdir build-linux
fi
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-9 -DJS_MODULE_VERSION=$VERSION ..
cmake --build . --config Release
cd ..

if [ ! -d "./dist" ]; then
    mkdir dist
fi
cp ./build-linux/libjs-module.so dist
cp ./deps/nodejs/lib/libnode.so.108 dist
