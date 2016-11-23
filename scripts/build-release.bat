cd ..
rmdir build /s /q
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make && cd bin && test-echelon
cd ..\\..\\scripts
