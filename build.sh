rm -rf build
mkdir build
cd build
cmake ..
make && cd bin && ./test-echelon
