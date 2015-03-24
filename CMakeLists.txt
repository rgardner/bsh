cmake_minimum_required(VERSION 3.1)
project(bsh)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunreachable-code -Wformat=2 -DUNIX")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG")

find_package(Readline REQUIRED)
include_directories(
  "src/"
  ${READLINE_INCLUDE_DIR}
)

set(SOURCE_FILES src/main.c src/alias.h src/alias.c src/bg_jobs.h src/bg_jobs.c
    src/builtins.h src/builtins.c src/history.h src/history.c src/linked_list.h
    src/linked_list.c src/node.h src/node.c src/parse.h src/parse.c src/stack.h
    src/stack.c src/variables.h src/variables.c
)
add_executable(bsh ${SOURCE_FILES})
target_link_libraries(bsh ${READLINE_LIBRARIES})
