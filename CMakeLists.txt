cmake_minimum_required(VERSION 3.26)
project(torrent)

set(CMAKE_CXX_STANDARD 17)

# Include directories for your header files
include_directories(${PROJECT_SOURCE_DIR}/torrent)

# Add the main executable
add_executable(torrent
        torrent/main.cpp
        torrent/src/BencodeParser.cpp
)

# Setup Google Test
# Make sure to download or clone the googletest library into the 'lib/googletest' directory
add_subdirectory(lib/googletest)
enable_testing()

add_executable(torrent_test
        torrent/src/BencodeParser.cpp
        tests/test_main.cpp
        tests/test_BencodeParser.cpp
        tests/test_main.cpp
)

# Link your test executable with Google Test
target_link_libraries(torrent_test gtest gtest_main)

# Add the tests to be run
add_test(NAME BencodeParserTest COMMAND torrent_test)

# Specify the test configuration
set_target_properties(torrent_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

include(CTest)