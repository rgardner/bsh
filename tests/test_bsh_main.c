#include "test_bsh.h"
#include <check.h>
#include <stdlib.h>

int
main(void)
{
  SRunner* sr = srunner_create(make_history_suite());
  srunner_add_suite(sr, make_job_suite());
  srunner_add_suite(sr, make_stack_suite());
  srunner_add_suite(sr, make_parse_suite());
  srunner_run_all(sr, CK_VERBOSE);
  int num_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
