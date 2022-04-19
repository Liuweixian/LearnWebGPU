#!/bin/bash

source ../../BuildTools/sh_export.sh
$EMCC hellowebgpu.c -s USE_WEBGPU=1 -o hellowebgpu.js