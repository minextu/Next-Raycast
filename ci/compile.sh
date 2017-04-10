#!/bin/bash

mkdir bin && cd bin

if [ "$1" == "win64" ]; then
    cmake -DCMAKE_TOOLCHAIN_FILE=../ci/toolchain-win64.cmake ../
elif [ "$1" == "html5" ]; then
    cmake -DCMAKE_TOOLCHAIN_FILE=../ci/toolchain-html5.cmake ../
elif [ "$1" == "linux" ]; then
    cmake ../
else
    echo "Not implemented"
    exit
fi

make
