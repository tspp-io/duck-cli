#include "commands.h"
#include "../utils/config.h"
#include "../utils/http.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

namespace duck::commands {

void install(const std::string& package) {
    std::cout << "Installing " << package << "...\n";

    bool isGitHub = package.find('/') != std::string::npos;
    std::string downloadUrl;
    std::string installName = package;
    std::string tarballPath = "temp_pkg.tar.gz";

    if (isGitHub) {
        // Assume GitHub: owner/repo
        // We'll try to download the main branch
        // In a real tool, we'd check the API for default branch or latest release
        downloadUrl = "https://github.com/" + package + "/archive/refs/heads/main.tar.gz";
        
        size_t slashPos = package.find('/');
        installName = package.substr(slashPos + 1);
    } else {
        // Default registry
        std::string registryUrl = "http://localhost:8080/api/packages/" + package;
        downloadUrl = "http://localhost:8080/tarballs/" + package + ".tar.gz";
    }

    std::cout << "Downloading from " << downloadUrl << "...\n";
    if (!utils::HttpClient::download(downloadUrl, tarballPath)) {
        if (isGitHub) {
            // Fallback to master
            std::cout << "Main branch not found, trying master...\n";
            downloadUrl = "https://github.com/" + package + "/archive/refs/heads/master.tar.gz";
            if (!utils::HttpClient::download(downloadUrl, tarballPath)) {
                 std::cerr << "Error: Failed to download package from GitHub.\n";
                 return;
            }
        } else {
            std::cerr << "Error: Failed to download package (Registry not reachable).\n";
            return;
        }
    }

    // 2. Extract
    std::string installPath = "duck_modules/" + installName;
    fs::create_directories(installPath);
    
    // Use --strip-components=1 because GitHub archives have a top-level folder
    std::string cmd = "tar -xzf " + tarballPath + " -C " + installPath + (isGitHub ? " --strip-components=1" : "");
    
    if (std::system(cmd.c_str()) != 0) {
        std::cerr << "Error: Failed to extract package.\n";
        std::remove(tarballPath.c_str());
        return;
    }
    std::remove(tarballPath.c_str());

    // 3. Update duck.json
    auto manifest = utils::Config::readManifest();
    if (manifest) {
        (*manifest)["dependencies"][installName] = isGitHub ? ("github:" + package) : "latest";
        utils::Config::writeManifest(*manifest);
        std::cout << "Added " << installName << " to dependencies.\n";
    } else {
        std::cout << "Warning: No duck.json found, skipping dependency update.\n";
    }

    std::cout << "Successfully installed " << installName << "\n";
}

} // namespace duck::commands
