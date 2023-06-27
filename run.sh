#!/bin/bash

echo "Start Running [Ray Tracing]"

echo "Start to build the project"
make -C build/

echo "Converting the result to [scene.ppm]"
build/ray_tracing.exe > scene.ppm

echo "Done!"