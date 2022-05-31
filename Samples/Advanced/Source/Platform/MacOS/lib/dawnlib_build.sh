#!/bin/bash
export DAWN_PROJ=/Users/liuweixian/Documents/github-workspace/dawn

pushd $DAWN_PROJ/

echo "----------------gclient syncing----------------"
cp scripts/standalone.gclient .gclient
gclient sync

if [[ $1 = "-d" ]]
then
echo "----------------mkdir----------------"
mkdir -p out/Debug
echo "----------------gn gen----------------"
gn gen out/Debug --args="use_system_xcode=true is_debug=true enable_dsyms=true"
echo "----------------autoninja----------------"
autoninja -C out/Debug
else
echo "----------------mkdir----------------"
mkdir -p out/Release
echo "----------------gn gen----------------"
gn gen out/Release
echo "----------------autoninja----------------"
autoninja -C out/Release
fi
popd