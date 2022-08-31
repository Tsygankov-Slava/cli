cmake_minimum_required(VERSION 3.20)
project(cli)

set (CLI_VERSION_MAJOR 0)
set (CLI_VERSION_MINOR 2)
set (CLI_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 14)

#Build googletest
add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
        "googletest")

set(SOURCE_FILES src/main.cpp src/Cli/Cli.cpp src/Cli/Cli.hpp src/Flag/Flag.hpp src/Command/Command.hpp)

#Main target
add_executable(cli ${SOURCE_FILES})
#add_executable(cli src/main.cpp build/cli.hpp)

configure_file(cli_config.h.in ../src/cli_config.h)

#Test target
add_executable(
       test
       tests/test.cpp build/cli.hpp)

#Add src to the include directories for the test target
target_include_directories(
        test PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/build/cli.hpp"
)

#Link test target against gtest libraries
target_link_libraries(
        test PRIVATE
        gtest gtest_main
)