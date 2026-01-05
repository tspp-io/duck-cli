#include <iostream>
#include <string>
#include <vector>
#include "commands/commands.h"

void print_usage() {
    std::cout << "Usage: duck <command> [options]\n"
              << "\n"
              << "Commands:\n"
              << "  init              Initialize a new TSPP project\n"
              << "  install <pkg>     Install a dependency\n"
              << "  publish           Publish package to registry\n"
              << "  build             Build the project\n"
              << "  run               Run the project\n"
              << "  help              Show this help message\n";
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        print_usage();
        return 1;
    }

    const std::string& command = args[0];

    if (command == "init") {
        duck::commands::init();
    } else if (command == "install") {
        if (args.size() < 2) {
            std::cerr << "Usage: duck install <package>\n";
            return 1;
        }
        duck::commands::install(args[1]);
    } else if (command == "publish") {
        duck::commands::publish();
    } else if (command == "build") {
        duck::commands::build();
    } else if (command == "run") {
        duck::commands::run();
    } else if (command == "help" || command == "--help" || command == "-h") {
        print_usage();
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        print_usage();
        return 1;
    }

    return 0;
}
