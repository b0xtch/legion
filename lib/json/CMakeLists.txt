add_library (jsonValidator ./src/jsonvalidator.cpp ./include/jsonvalidator.h)
target_include_directories(jsonValidator
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(jsonValidator nlohmann_json)

add_library (jsonDSL ./src/jsonDSL.cpp ./include/jsonDSL.h)
target_include_directories(jsonDSL
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(jsonDSL nlohmann_json)

add_subdirectory(ruleStructure)

add_library (specificationValidator ./src/specificationValidator.cpp ./include/fieldValidators.h)
target_include_directories(specificationValidator
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(specificationValidator nlohmann_json jsonDSL)

add_library (ruleValidator ./src/ruleValidator.cpp ./include/fieldValidators.h)
target_include_directories(ruleValidator
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(ruleValidator nlohmann_json jsonDSL)

add_library (configValidator ./src/configValidator.cpp ./include/fieldValidators.h)
target_include_directories(configValidator
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(configValidator nlohmann_json jsonDSL ruleValidator)

add_library (variableValidator ./src/variableValidator.cpp ./include/fieldValidators.h)
target_include_directories(variableValidator
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(variableValidator nlohmann_json jsonDSL)

# if you want to add a new library (for example, jsonParser) that uses nlohmann_json,
# then add the line { target_link_libraries(jsonParser nlohmann_json) } to the CMakeLists.txt that adds the library
# and add a { #include "json.hpp" } to the header file
