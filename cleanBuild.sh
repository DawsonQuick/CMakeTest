#!/bin/bash

echo "Beginning Clean Install"

echo "Cleaning up previous build"

rm -rf bin
rm -rf build

cmake -B build
cmake --build build
cmake --build build --target run