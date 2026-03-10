#!/bin/bash

set -e

cd `dirname $0`

# I keep forgetting to increase the build number
BUILD_NUM=$( echo "$(git rev-list --count main) + 1" | bc )
sed -i -E "/PROGRAM_VERSION_BUILD/ s/([0-9]+)\$/${BUILD_NUM}/" include/common.h

build_debug()
{
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build --config Debug --parallel
}

build_release()
{
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release --parallel
}

if [[ $1 = "debug" ]] then
    build_debug
elif [[ $1 = "release" ]] then
    build_release
else
    build_debug
fi

mv build/compile_commands.json ./compile_commands.json
