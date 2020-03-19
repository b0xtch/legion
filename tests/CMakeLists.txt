add_subdirectory(lib/googlemock)
enable_testing()

add_executable(runAllTests
  # Add all new test files to the list here.
  # Place each file name on a separate line.
  Dummy.cpp # This file is a placeholder and tests nothing.

  UserTest.cpp
  GameServerTest.cpp
  GameServerConfigTest.cpp
  ValidatorTest.cpp
  ParsedMessageTest.cpp
  SessionTest.cpp
  SessionManagerTest.cpp
)
target_include_directories(runAllTests PUBLIC ${CMAKE_SOURCE_DIR}/lib/json/include)
target_link_libraries(runAllTests gmock gtest gtest_main nlohmann_json)
target_link_libraries(runAllTests configValidator specificationValidator ruleValidator)
target_link_libraries(runAllTests User networking GameServer ParsedMessage Session SessionManager)

add_test(NAME AllTests COMMAND runAllTests)
