add_executable(server main.cpp)

add_library(GameServer GameServer.cpp ../include/GameServer.h)
target_include_directories(GameServer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(GameServer nlohmann_json)

add_library(SessionManager SessionManager.cpp ../include/SessionManager.h)
target_include_directories(SessionManager
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

