find_package(Check REQUIRED)
find_package(Threads)

set(bshTestSources
    test_bsh_alias.c
    test_bsh_circular_queue.c
    test_bsh_history.c
    test_bsh_job.c
    test_bsh_main.c
    test_bsh_stack.c
    test_bsh_parse.c
    test_bsh_util.c)

set(bshTestHeaders test_bsh.h test_utils.h)

add_executable(check_bsh ${bshTestHeaders} ${bshTestSources})

if(UNIX AND NOT APPLE)
  target_compile_definitions(check_bsh PUBLIC _GNU_SOURCE)
endif()

target_include_directories(check_bsh
                           PRIVATE ${PROJECT_SOURCE_DIR}/src
                                   ${CHECK_INCLUDE_DIRS})

target_link_libraries(check_bsh bsh Check::Check Threads::Threads)
