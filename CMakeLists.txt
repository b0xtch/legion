cmake_minimum_required(VERSION 3.16.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(social-gaming)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

find_package(nlohmann_json 3.2.0 REQUIRED)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory (lib)
add_subdirectory (src)

