#!/bin/bash

echo "starting build process..."
rm -rf build
cmake -B build -G "Unix Makefiles" -S .
make -C build
