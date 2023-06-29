#!/bin/bash

echo "Start Running [Ray Tracing]"

UNAME=$(uname)

echo "Removing current exist scene.ppm file"
rm ./scene.ppm

if [[ "$UNAME" == "Darwin" || "$UNAME" == "Linux" ]]; then
    echo "[UNIX] Start to build the project"
    make -C build/
elif [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]]; then
    echo "[WIN] Start to build the project"
    make -C build/
fi

echo "Converting the result to [scene.ppm]"
if [[ "$UNAME" == "Darwin" || "$UNAME" == "Linux" ]]; then
    time build/ray_tracing > scene.ppm
elif [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]]; then
    time build/ray_tracing.exe > scene.ppm
fi

echo "Done!"