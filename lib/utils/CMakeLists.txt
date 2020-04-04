# https://cmake.org/cmake/help/v3.7/module/FindBoost.html
find_package(Boost 1.66 COMPONENTS filesystem REQUIRED)

add_library(Utils ./include/Utils.h ./src/Utils.cpp)
target_include_directories(Utils
    PUBLIC ${Boost_INCLUDE_DIRS}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(Utils ${Boost_LIBRARIES} nlohmann_json)


