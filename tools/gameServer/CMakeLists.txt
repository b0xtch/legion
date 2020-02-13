add_library(Session src/Session.cpp include/Session.h)
target_include_directories(Session
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(Session networking Utils)

add_library(MessageType src/MessageType.cpp include/MessageType.h)
target_include_directories(MessageType
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(MessageType nlohmann_json)


add_library(SessionManager src/SessionManager.cpp include/SessionManager.h)
target_include_directories(SessionManager
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(SessionManager Session networking)


add_library(GameServer src/GameServer.cpp include/GameServer.h)
target_include_directories(GameServer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(GameServer networking MessageType SessionManager nlohmann_json Utils)


add_executable(server src/main.cpp)
target_link_libraries(server GameServer)

# https://stackoverflow.com/a/29871891
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads)
