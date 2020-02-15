add_subdirectory(lib/googlemock)
enable_testing()

add_executable(runAllTests
  # Add all new test files to the list here.
  # Place each file name on a separate line.
  Dummy.cpp # This file is a placeholder and tests nothing.
  
  UserTests.cpp
)

target_link_libraries(runAllTests gmock gtest gtest_main)
target_link_libraries(runAllTests User networking)

add_test(NAME AllTests COMMAND runAllTests)
