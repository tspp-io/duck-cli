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

    // Automated GitHub Registry Publish Flow
    std::string home = std::getenv("HOME");
    std::string registryPath = home + "/.duck/registry";
    std::string registryUrl = "https://github.com/tspp-io/duck-registry.git";

    if (!std::filesystem::exists(registryPath)) {
        std::cout << "Cloning registry to " << registryPath << "...\n";
        std::filesystem::create_directories(home + "/.duck");
        std::string cmd = "git clone " + registryUrl + " " + registryPath;
        if (std::system(cmd.c_str()) != 0) {
             std::cerr << "Error: Failed to clone registry.\n";
             std::remove(tarball.c_str());
             return;
        }
    } else {
        std::cout << "Updating registry...\n";
        std::string cmd = "cd " + registryPath + " && git pull";
        if (std::system(cmd.c_str()) != 0) {
             std::cerr << "Warning: Failed to pull registry updates.\n";
        }
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
    
    std::cout << "Pushing to GitHub...\n";
    std::string pushCmd = "cd " + registryPath + " && git add . && git commit -m \"Publish " + name + " " + version + "\" && git push";
    if (std::system(pushCmd.c_str()) != 0) {
        std::cerr << "Error: Failed to push to GitHub. Check your credentials.\n";
    } else {
        std::cout << "Successfully published " << name << "@" << version << "\n";
    }

    // Cleanup
    std::remove(tarball.c_str());
}

} // namespace duck::commands
