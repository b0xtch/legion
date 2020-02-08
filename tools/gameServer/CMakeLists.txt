add_executable(server src/main.cpp)

add_library(GameServer src/GameServer.cpp include/GameServer.h)
add_library(MessageType src/MessageType.cpp include/MessageType.h)
add_library(SessionManager src/SessionManager.cpp include/SessionManager.h)

target_include_directories(GameServer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(GameServer networking)
target_link_libraries(GameServer MessageType)

target_include_directories(MessageType
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(GameServer nlohmann_json)

target_include_directories(SessionManager
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
