#include <check.h>
#include "test_bsh.h"
#include "test_util.h"
#include "../src/util.h"

void util_setup(void) {}

void util_teardown(void) {}

START_TEST(trim_normal) {
  char *s = new_str("user_input\n");
  ck_assert_str_eq(trim(s), "user_input");
}
END_TEST

START_TEST(trim_no_newline) {
  char *s = new_str("user_input");
  ck_assert_str_eq(trim(s), "user_input");
}
END_TEST

Suite *make_util_suite(void) {
  Suite *s = suite_create("Util");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, util_setup, util_teardown);
  tcase_add_test(tc, trim_normal);
  tcase_add_test(tc, trim_no_newline);

  return s;
}
