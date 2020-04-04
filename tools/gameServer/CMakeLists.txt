add_library(GameServer src/GameServer.cpp include/GameServer.h)
add_library(ParsedMessage src/ParsedMessage.cpp include/ParsedMessage.h)
add_library(SessionManager src/SessionManager.cpp include/SessionManager.h)
add_library(Session src/Session.cpp include/Session.h)
add_library(User src/User.cpp include/User.h)
add_library(engine src/engine.cpp include/engine.h)

target_include_directories(GameServer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(GameServer SessionManager User networking Utils nlohmann_json ParsedMessage)

target_include_directories(ParsedMessage
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(ParsedMessage nlohmann_json networking)

target_include_directories(SessionManager
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(SessionManager nlohmann_json Session networking User Utils ParsedMessage)

target_include_directories(Session
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(Session nlohmann_json User networking Utils ParsedMessage)

target_include_directories(User
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(User networking)

target_include_directories(engine
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(engine nlohmann_json jsonDSL)

add_executable(server src/main.cpp)
target_link_libraries(server GameServer Utils)

# https://stackoverflow.com/a/29871891
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads)
