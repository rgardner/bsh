project(bsh C)

cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

###############################################################################
# Set build features
set(CMAKE_BUILD_TYPE Debug)

###############################################################################


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wunreachable-code -Wformat=2 -DUNIX")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")

find_package(Readline REQUIRED)
include_directories(
  ${READLINE_INCLUDE_DIR}
)

###############################################################################
# Subdirectories
add_subdirectory(src)
add_subdirectory(tests)

###############################################################################
# Unit tests
enable_testing()
add_test(NAME check_bsh COMMAND check_bsh)