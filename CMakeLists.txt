cmake_minimum_required(VERSION 3.20)
project(cli)

set (CLI_VERSION_MAJOR 0)
set (CLI_VERSION_MINOR 1)
set (CLI_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/Cli/Cli.cpp src/Cli/Cli.hpp src/Flag/Flag.cpp src/Flag/Flag.hpp src/Command/Command.cpp src/Command/Command.hpp)

#add_executable(cli ${SOURCE_FILES})
add_executable(cli src/main.cpp build/cli.hpp)

configure_file(cli_config.h.in ../src/cli_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})