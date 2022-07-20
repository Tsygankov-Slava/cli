cmake_minimum_required(VERSION 3.20)
project(cli)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES src/main.cpp src/Cli/Cli.cpp src/Cli/Cli.hpp src/Flag/Flag.cpp src/Flag/Flag.hpp src/Command/Command.cpp src/Command/Command.hpp)

#add_executable(cli ${SOURCE_FILES})
add_executable(cli src/main.cpp build/cli.hpp)
