#include "config.h"
#include <fstream>
#include <iostream>

namespace duck::utils {

std::optional<json> Config::readManifest(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return std::nullopt;
    
    try {
        json j;
        f >> j;
        return j;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing " << path << ": " << e.what() << std::endl;
        return std::nullopt;
    }
}

bool Config::writeManifest(const json& manifest, const std::string& path) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    f << manifest.dump(2);
    return true;
}

} // namespace duck::utils
