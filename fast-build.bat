cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
mingw32-make test
cd ..