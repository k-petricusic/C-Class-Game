# For macOS/Linux

#!/bin/bash
set -e

echo "Building C++ Game..."

MODE=${1:-Release} # default to Release if no argument is provided, first argument is the mode
mkdir -p build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=$MODE
cmake --build .
echo "Build complete."
