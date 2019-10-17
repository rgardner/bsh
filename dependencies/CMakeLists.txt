include(FetchContent)

fetchcontent_declare(check
                     GIT_REPOSITORY
                     https://github.com/libcheck/check.git
                     GIT_TAG
                     0b80af232b9d899f876faf3930093d0a3b19f378)

fetchcontent_getproperties(check)
if(NOT check_POPULATED)
  fetchcontent_populate(check)
  add_subdirectory(${check_SOURCE_DIR} ${check_BINARY_DIR})
endif()
