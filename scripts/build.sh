cd ..
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DECHELON_CODE_GENERATOR=spider-monkey-js
make && cd bin && ./test-echelon
cd ../../scripts
