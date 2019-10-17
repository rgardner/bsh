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

add_executable(bshUnitTests ${bshTestHeaders} ${bshTestSources})
target_compile_options(bshUnitTests PRIVATE ${BSH_C_FLAGS_COMMON})

if(UNIX AND NOT APPLE)
  target_compile_definitions(bshUnitTests PUBLIC _GNU_SOURCE)
endif()

include(FetchContent)
fetchcontent_getproperties(check)
target_include_directories(bshUnitTests
                           PRIVATE ${PROJECT_SOURCE_DIR}/src ${check_BINARY_DIR}
                                   ${check_BINARY_DIR}/src)

target_link_libraries(bshUnitTests bshLib check Threads::Threads)
