set(LIB_SOURCES
  alias.c
  bg_jobs.c
  builtins.c
  history.c
  linked_list.c
  job.c
  node.c
  parse.c
  stack.c
  variables.c
)

set(MAIN_SOURCES
  main.c
)

set(HEADERS
  ${CONFIG_HEADERS}
  alias.h
  bg_jobs.h
  builtins.h
  history.h
  linked_list.h
  job.h
  node.h
  parse.h
  stack.h
  variables.h
)

add_library(bsh STATIC ${LIB_SOURCES} ${HEADERS})

add_executable(main ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(main bsh ${READLINE_LIBRARIES})
