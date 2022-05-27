cmake_minimum_required(VERSION 3.20)
project(cli)

set(CMAKE_CXX_STANDARD 17)

add_executable(cli src/main.cpp src/CLI/CLI.cpp src/CLI/CLI.h)
