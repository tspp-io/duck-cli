#include "commands.h"
#include "../utils/config.h"
#include "../utils/http.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace duck::commands {

void publish() {
    auto manifest = utils::Config::readManifest();
    if (!manifest) {
        std::cerr << "Error: duck.json not found or invalid.\n";
        return;
    }

    std::string name = (*manifest)["name"];
    std::string version = (*manifest)["version"];

    std::cout << "Publishing " << name << "@" << version << "...\n";

    // Create tarball
    std::string tarball = name + "-" + version + ".tar.gz";
    // Use --exclude-vcs to ignore git files and explicitly exclude the tarball itself
    std::string cmd = "tar -czf " + tarball + " --exclude=" + tarball + " --exclude=duck_modules --exclude=build --exclude=.git .";
    
    // Note: tar might complain "file changed as we read it" if we are writing to the same dir we are reading from.
    // But usually it works if we exclude the output file.
    // Let's ignore the exit code 1 (warning) from tar if the file was created.
    
    int res = std::system(cmd.c_str());
    if (res != 0 && !std::filesystem::exists(tarball)) {
        std::cerr << "Error: Failed to create tarball (exit code " << res << ").\n";
        return;
    }

    // Static Registry Publish Flow
    // 1. Check if registry repo is cloned locally (or ask user for path)
    // 2. Copy tarball to packages/<name>/<version>.tar.gz
    // 3. Update metadata.json
    
    const char* registryPathEnv = std::getenv("DUCK_REGISTRY_PATH");
    std::string registryPath = registryPathEnv ? registryPathEnv : "../duck-registry";
    
    if (!std::filesystem::exists(registryPath)) {
        std::cerr << "Error: Registry path not found at " << registryPath << "\n";
        std::cerr << "Please clone the registry repo and set DUCK_REGISTRY_PATH or place it at ../duck-registry\n";
        std::remove(tarball.c_str());
        return;
    }
    
    std::string packageDir = registryPath + "/packages/" + name;
    std::filesystem::create_directories(packageDir);
    
    std::string targetTarball = packageDir + "/" + version + ".tar.gz";
    std::string latestTarball = packageDir + "/latest.tar.gz";
    
    std::filesystem::copy_file(tarball, targetTarball, std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy_file(tarball, latestTarball, std::filesystem::copy_options::overwrite_existing);
    
    // Update metadata.json
    std::string metaPath = packageDir + "/metadata.json";
    utils::json metadata;
    
    if (std::filesystem::exists(metaPath)) {
        std::ifstream f(metaPath);
        f >> metadata;
    } else {
        metadata = {
            {"name", name},
            {"versions", utils::json::object()}
        };
    }
    
    metadata["versions"][version] = *manifest;
    metadata["latest"] = version;
    
    std::ofstream f(metaPath);
    f << metadata.dump(2);
    
    std::cout << "Package staged in " << packageDir << "\n";
    std::cout << "Please commit and push the changes to the registry repository.\n";

    // Cleanup
    std::remove(tarball.c_str());
}

} // namespace duck::commands
