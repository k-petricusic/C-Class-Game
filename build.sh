# For macOS/Linux

#!/bin/bash
set -e

echo "Building C++ Game..."

MODE=${1:-Release} # default to Release if no argument is provided, first argument is the mode
TRIPLET=${2:-} # default to empty if no argument is provided, second argument is the triplet
TRIPLET_ARG=""
if [ -n "$TRIPLET" ]; then # if the triplet is not empty
  TRIPLET_ARG="-DVCPKG_TARGET_TRIPLET=$TRIPLET"
fi
mkdir -p build
cd build
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=~/dev/vcpkg/scripts/buildsystems/vcpkg.cmake $TRIPLET_ARG -DCMAKE_BUILD_TYPE=$MODE
cmake --build .
echo "Build complete."
