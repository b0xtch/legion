find_package(nlohmann_json 3.2.0 REQUIRED)
add_library (jsonparser jsonparser.cpp)
add_library (jsonDSL jsonDSL.cpp)
target_link_libraries(jsonparser
    PRIVATE nlohmann_json::nlohmann_json jsonDSL
)