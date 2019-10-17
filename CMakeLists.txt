# =============================
# Preamble
# =============================
cmake_minimum_required(VERSION 3.12)
project(bsh VERSION 0.1.0 DESCRIPTION "Bash-like shell" LANGUAGES C)

# =============================
# Project-wide setup
# =============================
option(BSH_ENABLE_WERROR "Enable all warnings as errors" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS YES) # Enable GNU and POSIX extensions

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # improve Intellisense
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # use folders in IDEs

# Restrict valid build types for non-multi-config generators, e.g. cannot
# accidently use build type 'Dbg' with Ninja.
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig)
  set(allowableBuildTypes Debug Release RelWithDebInfo)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowableBuildTypes}")
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
  elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
  endif()
endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()

set(BSH_C_FLAGS_COMMON
    -Wall
    -Wextra
    -Werror
    -Wunreachable-code
    -Wformat=2
    -DUNIX)
if(BSH_ENABLE_WERROR)
  list(APPEND BSH_C_FLAGS_COMMON -Werror)
endif()

find_package(Readline REQUIRED)
include_directories(${READLINE_INCLUDE_DIR})
if(UNIX AND NOT APPLE)
  find_package(BSD)
  include_directories(${BSD_INCLUDE_DIR})
endif()

# =============================
# Externally provided content
# =============================
add_subdirectory(dependencies)

# =============================
# Main targets built by this project
# =============================
add_subdirectory(src)
add_subdirectory(doc)

# =============================
# Targets if this is the top-level project
# =============================
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR AND BUILD_TESTING)
  add_subdirectory(tests)

  add_custom_target(runTests COMMAND bshUnitTests DEPENDS bshUnitTests)
endif()
