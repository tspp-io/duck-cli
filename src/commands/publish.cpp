#include "commands.h"
#include "../utils/config.h"
#include "../utils/http.h"
#include <iostream>
#include <cstdlib>

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
    std::string cmd = "tar -czf " + tarball + " --exclude=" + tarball + " --exclude=duck_modules --exclude=build --exclude=.git .";
    
    if (std::system(cmd.c_str()) != 0) {
        std::cerr << "Error: Failed to create tarball.\n";
        return;
    }

    // TODO: Read tarball into memory or stream it
    // For now, we'll just simulate the upload or send metadata
    
    // In a real implementation, we would read the file and send it as multipart/form-data
    // or send metadata first, get a signed URL, and upload there.
    
    // Let's just send metadata for now to demonstrate
    utils::json payload = {
        {"name", name},
        {"version", version},
        {"manifest", *manifest}
    };

    auto response = utils::HttpClient::post("http://localhost:8080/api/publish", payload.dump());
    if (response) {
        std::cout << "Response: " << *response << "\n";
    } else {
        std::cout << "Failed to connect to registry (expected if no server is running).\n";
        std::cout << "Simulated publish successful for " << tarball << "\n";
    }

    // Cleanup
    std::remove(tarball.c_str());
}

} // namespace duck::commands
