add_library(nlohmann_json ./include/json.hpp)

target_include_directories(nlohmann_json PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> PUBLIC $<INSTALL_INTERFACE:include>)

set_target_properties(nlohmann_json PROPERTIES LINKER_LANGUAGE CXX)