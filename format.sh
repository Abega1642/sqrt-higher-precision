#!/bin/bash

find . -type f \( -name "*.cpp" -o -name "*.h" \) -print0 | while IFS= read -r -d '' file; do
    clang-format -i -style=Google "$file"
done
