file(GLOB_RECURSE SOURCES 
    "main.cpp"
    "commands/*.cpp"
    "utils/*.cpp"
)

add_executable(duck ${SOURCES})

target_include_directories(duck PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_link_libraries(duck PRIVATE CURL::libcurl)
