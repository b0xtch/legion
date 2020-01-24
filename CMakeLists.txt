cmake_minimum_required(VERSION 3.16.2)
project(social-gaming)

find_package(nlohmann_json 3.2.0 REQUIRED)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)