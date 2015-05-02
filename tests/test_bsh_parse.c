#include <stdlib.h>
#include <check.h>
#include "test_bsh.h"
#include "../src/parse.h"

void setup(void) {}

void teardown(void) {}

START_TEST(test_empty) {
  struct ParseInfo *p = parse("");
  ck_assert(p == NULL);
}
END_TEST

Suite *make_parse_suite(void) {
  Suite *s = suite_create("Parse");

  /* Core test case. */
  TCase *tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_empty);
  suite_add_tcase(s, tc_core);

  return s;
}
