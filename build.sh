#!/bin/bash

set -e

cd `dirname $0`

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