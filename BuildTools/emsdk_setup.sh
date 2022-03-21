#!/bin/bash

# Download and install the latest SDK tools.
./BuildTools/emsdk/emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./BuildTools/emsdk/emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source ./BuildTools/emsdk/emsdk_env.sh

./BuildTools/emscripten/emcc -v