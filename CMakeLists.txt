cmake_minimum_required(VERSION 3.20)
project(cli)

set(CMAKE_CXX_STANDARD 20)

add_executable(cli src/main.cpp src/Cli/Cli.cpp src/Cli/Cli.h src/Flag/Flag.cpp src/Flag/Flag.h)
