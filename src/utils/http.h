#pragma once

#include <string>
#include <optional>

namespace duck::utils {

class HttpClient {
public:
    static std::optional<std::string> post(const std::string& url, const std::string& body);
    static bool download(const std::string& url, const std::string& outputPath);
};

} // namespace duck::utils
