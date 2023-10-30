#!/bin/bash

echo "starting build process..."
rm -rf build
cmake  -DCMAKE_BUILD_TYPE=Debug -B build -G "Unix Makefiles" -S .
make -C build
