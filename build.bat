rmdir build /s /q
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
mingw32-make test