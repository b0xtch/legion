if [ -d "build" ]; then rm -Rf build; fi
mkdir build && cd build
cmake .. && make
