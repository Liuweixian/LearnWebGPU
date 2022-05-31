#!/bin/bash
export DAWN_PROJ=/Users/liuweixian/Documents/github-workspace/dawn
if [[ $1 = "release" ]]
then
export OUTPUT_FLAG=Release
else
export OUTPUT_FLAG=Debug
fi

cp $DAWN_PROJ/out/$OUTPUT_FLAG/libdawn_native.dylib ./dawn/bin/mac/$OUTPUT_FLAG/
cp $DAWN_PROJ/out/$OUTPUT_FLAG/libdawn_platform.dylib ./dawn/bin/mac/$OUTPUT_FLAG/
cp $DAWN_PROJ/out/$OUTPUT_FLAG/libdawn_proc.dylib ./dawn/bin/mac/$OUTPUT_FLAG/
#cp $DAWN_PROJ/out/$OUTPUT_FLAG/libc++_chrome.dylib ./dawn/bin/mac/$OUTPUT_FLAG/

cp -rf $DAWN_PROJ/include/ ./dawn/inc/

cp -rf $DAWN_PROJ/out/$OUTPUT_FLAG/gen/include/dawn/ ./dawn/inc/dawn/