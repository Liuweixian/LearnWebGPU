#!/bin/bash

source ../../BuildTools/sh_export.sh
$EMCC EntryPoint.cpp -s USE_WEBGPU=1 -o hellowebgpu.js --preload-file Shaders