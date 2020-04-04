add_library (variableHelper ./variableHelper.cpp ./variableHelper.h)
target_include_directories(variableHelper
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(variableHelper jsonDSL absl::strings nlohmann_json)