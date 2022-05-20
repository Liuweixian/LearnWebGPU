#!/bin/bash
# cmake -DCMAKE_TOOLCHAIN_FILE=<EmscriptenRoot>/cmake/Modules/Platform/Emscripten.cmake
#       -DCMAKE_BUILD_TYPE=<Debug|RelWithDebInfo|Release|MinSizeRel>
#       -G "Unix Makefiles" (Linux and OSX)
#       -G "MinGW Makefiles" (Windows)
#       <path/to/CMakeLists.txt> # Note, pass in here ONLY the path to the file, not the filename 'CMakeLists.txt' itself.

source ../../BuildTools/sh_export.sh
if [ $1 == "-f" ] 
then 
rm -rf Build
fi
mkdir Build
export EMCC_DEBUG=0
cmake -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN_ROOT/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ./CMakeLists.txt -B ./Build
cd Build
make