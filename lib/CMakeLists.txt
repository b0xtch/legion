set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)
add_library(nlohmannjson ./nlohmann_json/json.hpp)
target_include_directories(nlohmannjson PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> PUBLIC $<INSTALL_INTERFACE:include>)

add_subdirectory(networking)
add_subdirectory(util)
add_subdirectory(json)
