add_executable(server main.cpp)

add_library(GameServer GameServer.cpp ../include/GameServer.h)
target_include_directories(GameServer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

add_library(SessionManager SessionManager.cpp ../include/SessionManager.h)
target_include_directories(SessionManager
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(server 
    networking
    PRIVATE nlohmann_json::nlohmann_json
)

install(TARGETS server DESTINATION bin)
