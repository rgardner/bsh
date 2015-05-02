#include <stdlib.h>
#include <check.h>
#include "test_bsh.h"


int main(void) {
  SRunner *sr = srunner_create(make_stack_suite());
  srunner_run_all(sr, CK_VERBOSE);
  int num_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
