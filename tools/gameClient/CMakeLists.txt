
add_executable(GameClient
  src/GameClient.cpp
  src/MenuPage.cpp
  src/MenuManager.cpp
  src/ChatWindow.cpp
)

set_target_properties(GameClient
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(GameClient
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIR}
)

target_link_libraries(GameClient
  networking
  ${CURSES_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  -lmenu
  -lform
  nlohmann_json
  Utils
)

install(TARGETS GameClient
  RUNTIME DESTINATION bin
)
