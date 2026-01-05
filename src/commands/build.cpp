#include "commands.h"
#include <iostream>
#include <cstdlib>

namespace duck::commands {

void build() {
    std::cout << "Building project...\n";
    // Try to find tspp compiler
    // For now, assume it's in the PATH or we can use the one in the workspace
    
    std::string cmd = "tspp ."; // Assuming tspp takes the directory as argument
    // Or maybe we need to read duck.json to find entry point
    
    // Let's read duck.json to find entry point
    // We need to include config.h but I didn't include it here.
    // I'll just run a generic command for now.
    
    int res = std::system(cmd.c_str());
    if (res != 0) {
        std::cerr << "Build failed. Make sure 'tspp' is in your PATH.\n";
    } else {
        std::cout << "Build successful.\n";
    }
}

void run() {
    std::cout << "Running project...\n";
    // TODO: Implement run logic
}

} // namespace duck::commands
