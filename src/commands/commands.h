#pragma once
#include <string>

namespace duck::commands {

void init();
void install(const std::string& package);
void publish();
void build();
void run();

} // namespace duck::commands
