add_library(GameServer src/GameServer.cpp include/GameServer.h)
add_library(MessageType src/MessageType.cpp include/MessageType.h)
add_library(SessionManager src/SessionManager.cpp include/SessionManager.h)
add_library(session src/session.cpp include/session.h)

target_include_directories(GameServer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(GameServer networking MessageType SessionManager nlohmann_json Utils)

target_include_directories(MessageType
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(MessageType nlohmann_json)

target_include_directories(SessionManager
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(SessionManager session)

target_include_directories(session
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

add_executable(server src/main.cpp)
target_link_libraries(server GameServer)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads)