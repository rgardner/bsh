cmake_minimum_required(VERSION 3.1)
project(bsh)

find_package(Readline)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunreachable-code -Wformat=2 -DUNIX")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG")

set(SOURCE_FILES src/main.c)
add_executable(bsh ${SOURCE_FILES})
