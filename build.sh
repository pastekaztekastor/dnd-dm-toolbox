#!/bin/bash

set -e

echo "==================================="
echo "D&D DM Toolbox Build Script"
echo "==================================="

# Check if dependencies are installed
if [ ! -d "libs/imgui" ] || [ ! -d "libs/json" ]; then
    echo "Dependencies not found. Running setup script..."
    ./setup_dependencies.sh
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
echo "Building project..."
make -j$(nproc)

echo ""
echo "==================================="
echo "Build completed successfully!"
echo "==================================="
echo ""
echo "To run the application:"
echo "  cd build && ./DnD_DM_Toolbox"
echo ""
echo "Or use Docker:"
echo "  docker-compose up -d dev"
echo "==================================="
