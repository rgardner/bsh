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
  util.c
)

set(MAIN_SOURCES
  main.c
)

set(HEADERS
  ${CONFIG_HEADERS}
  alias.h
  bg_jobs.h
  builtins.h
  dbg.h
  history.h
  linked_list.h
  job.h
  node.h
  parse.h
  stack.h
  util.h
)

add_library(bsh STATIC ${LIB_SOURCES} ${HEADERS})

add_executable(bsh-bin ${HEADERS} ${MAIN_SOURCES})
if(APPLE)
  target_link_libraries(bsh-bin bsh ${READLINE_LIBRARIES})
else(UNIX)
  target_link_libraries(bsh-bin bsh ${READLINE_LIBRARIES} ${BSD_LIBRARY})
endif(APPLE)
set_target_properties(bsh-bin PROPERTIES OUTPUT_NAME bsh)
