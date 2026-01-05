#include "commands.h"
#include "../utils/config.h"
#include <iostream>
#include <filesystem>

namespace duck::commands {

void init() {
    if (std::filesystem::exists("duck.json")) {
        std::cerr << "Error: duck.json already exists.\n";
        return;
    }

    std::string name;
    std::cout << "Package name: ";
    std::getline(std::cin, name);
    if (name.empty()) name = "my-package";

    utils::json manifest = {
        {"name", name},
        {"version", "0.1.0"},
        {"exports", {
            {".", "./src/main.tspp"}
        }},
        {"dependencies", utils::json::object()}
    };

    if (utils::Config::writeManifest(manifest)) {
        std::cout << "Created duck.json\n";
    } else {
        std::cerr << "Error creating duck.json\n";
    }
}

} // namespace duck::commands
