#!/bin/bash

echo "Start Running [Ray Tracing]"

echo "Start to build the project"
make -C build/

echo "Converting the result to [image.ppm]"
build/ray_tracing.exe > image.ppm

echo "Done!"