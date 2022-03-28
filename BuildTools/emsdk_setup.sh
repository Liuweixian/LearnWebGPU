#!/bin/bash

pushd ./BuildTools/emsdk/

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source emsdk_env.sh

popd

./BuildTools/emscripten/emcc -v