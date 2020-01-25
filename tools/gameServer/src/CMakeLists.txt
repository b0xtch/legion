add_executable(server server.cpp)

find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(server 
    networking
    PRIVATE nlohmann_json::nlohmann_json
)

install(TARGETS server DESTINATION bin)
