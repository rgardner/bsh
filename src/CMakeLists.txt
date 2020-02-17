set(LIB_SOURCES
    alias.c
    bg_jobs.c
    builtins.c
    circular_queue.c
    history.c
    linked_list.c
    job.c
    node.c
    parse.c
    stack.c
    util.c)

set(MAIN_SOURCES main.c)

set(HEADERS
    ${CONFIG_HEADERS}
    alias.h
    bg_jobs.h
    builtins.h
    circular_queue.h
    dbg.h
    history.h
    linked_list.h
    job.h
    node.h
    parse.h
    stack.h
    util.h)

add_library(bshLib STATIC ${LIB_SOURCES} ${HEADERS})
target_compile_options(bshLib PRIVATE ${BSH_C_FLAGS_COMMON})
target_compile_definitions(bshLib PRIVATE ${BSH_C_DEFINES_COMMON})

add_executable(bshBin ${HEADERS} ${MAIN_SOURCES})
target_compile_options(bshBin PRIVATE ${BSH_C_FLAGS_COMMON})
target_compile_definitions(bshBin PRIVATE ${BSH_C_DEFINES_COMMON})

if(APPLE)
  target_link_libraries(bshBin bshLib ${READLINE_LIBRARIES})
else(UNIX)
  target_link_libraries(bshBin bshLib ${READLINE_LIBRARIES} ${BSD_LIBRARY})
endif()

set_target_properties(bshBin PROPERTIES OUTPUT_NAME bsh)
