mkdir build-linux
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cd ..
