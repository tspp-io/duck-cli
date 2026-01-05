#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Error handling function
handle_error() {
    echo -e "${RED}Error: $1${NC}"
    exit 1
}

echo -e "${BLUE}Duck CLI Build Script${NC}"
echo -e "${YELLOW}Starting build process...${NC}"

# Check if build directory exists
if [ -d "build" ]; then
    echo -e "${YELLOW}Removing old build directory...${NC}"
    rm -rf build || handle_error "Failed to remove old build directory"
fi

# Create new build directory
echo -e "${YELLOW}Creating new build directory...${NC}"
mkdir build || handle_error "Failed to create build directory"
cd build || handle_error "Failed to enter build directory"

# Configure CMake
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Debug || handle_error "CMake configuration failed"

# Build project
echo -e "${YELLOW}Building project...${NC}"
cmake --build . || handle_error "Build failed"

echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${GREEN}Binary located at ./build/src/duck${NC}"
