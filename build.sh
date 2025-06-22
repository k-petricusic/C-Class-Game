# knows that #! is special instructions
#!/bin/bash
set -e

echo "Building C++ Game..."

MODE=${1:-Debug} # default to Debug if no argument is provided
echo "Build mode: $MODE"

# make this file executable, "$0" is the name of the file itself
chmod +x "$0"

# Create build directory if it doesn't exist
echo "Setting up build directory..."
mkdir -p build

# Navigate to build directory
cd build

# Run CMake to generate/update build files
# CMake will automatically detect if cache needs to be regenerated
echo "Running CMake..."
cmake -DCMAKE_BUILD_TYPE=$MODE ..

# Build the project
echo "Compiling..."
cmake --build .

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful! 🎉"
    echo "You can run the game with: ./game"
else
    echo "Build failed! ❌"
    exit 1
fi
