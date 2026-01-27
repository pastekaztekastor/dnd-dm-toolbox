#!/bin/bash

set -e

echo "Setting up dependencies for DnD DM Toolbox..."

# Create libs directory if it doesn't exist
mkdir -p libs
cd libs

# Clone Dear ImGui
if [ ! -d "imgui" ]; then
    echo "Cloning Dear ImGui..."
    git clone https://github.com/ocornut/imgui.git
    cd imgui
    git checkout docking  # Use docking branch for better window management
    cd ..
else
    echo "Dear ImGui already exists, skipping..."
fi

# Clone nlohmann/json
if [ ! -d "json" ]; then
    echo "Cloning nlohmann/json..."
    git clone https://github.com/nlohmann/json.git
else
    echo "nlohmann/json already exists, skipping..."
fi

cd ..

echo "Dependencies setup complete!"
echo ""
echo "Please make sure you have the following packages installed:"
echo "  - CMake (>= 3.15)"
echo "  - A C++17 compatible compiler (GCC, Clang)"
echo "  - GLFW3 development libraries"
echo "  - OpenGL development libraries"
echo ""
echo "On Arch/Manjaro, install with:"
echo "  sudo pacman -S cmake glfw-x11 mesa"
echo ""
echo "After dependencies are installed, build with:"
echo "  mkdir build && cd build"
echo "  cmake .."
echo "  make -j$(nproc)"
