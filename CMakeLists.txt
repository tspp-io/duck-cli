cmake_minimum_required(VERSION 3.14)
project(duck-cli VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(CURL REQUIRED)
# find_package(ZLIB REQUIRED)

add_subdirectory(src)
