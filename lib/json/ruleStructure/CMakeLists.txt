add_library (ruleStructure ./src/ruleStructure.cpp ./include/ruleStructure.h)
target_include_directories(ruleStructure
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(ruleStructure jsonDSL)