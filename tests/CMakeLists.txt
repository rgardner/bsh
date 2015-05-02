include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
set(CHECK_INSTALL_DIR "/usr/local/Cellar/check/")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

set(TEST_BSH_SOURCES
  test_stack.c
  test_bsh_main.c
)
set(TEST_BSH_HEADERS test_bsh.h)
add_executable(check_bsh ${TEST_BSH_HEADERS} ${TEST_BSH_SOURCES})
target_link_libraries(check_bsh bsh ${CHECK_LIBRARIES})
