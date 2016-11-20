cd ..
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make && cd bin && ./test-echelon
cd ../../scripts
