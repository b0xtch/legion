add_library(Utils ./include/Utils.h ./src/Utils.cpp)
target_include_directories(Utils
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
