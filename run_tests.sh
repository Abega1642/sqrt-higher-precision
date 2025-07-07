#!/bin/bash
set -e

echo "Building project..."
cmake --build build

echo "Running tests..."
cd build
ctest --output-on-failure
