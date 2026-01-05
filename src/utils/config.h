#pragma once

#include <string>
#include <optional>
#include "json.hpp"

namespace duck::utils {

using json = nlohmann::json;

class Config {
public:
    static std::optional<json> readManifest(const std::string& path = "duck.json");
    static bool writeManifest(const json& manifest, const std::string& path = "duck.json");
};

} // namespace duck::utils
