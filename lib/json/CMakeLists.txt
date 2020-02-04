add_library (jsonValidator ./src/jsonvalidator.cpp ./include/jsonvalidator.h)
add_library (jsonDSL ./src/jsonDSL.cpp ./include/jsonDSL.h)
target_link_libraries(jsonValidator nlohmann_json)
target_link_libraries(jsonDSL nlohmann_json)

# if you want to add a new library (for example, jsonParser) that uses nlohmann_json,
# then add the line { target_link_libraries(jsonParser nlohmann_json) } to the CMakeLists.txt that adds the library
# and add a { #include "json.hpp" } to the header file of the library
