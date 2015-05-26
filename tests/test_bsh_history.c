#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "test_bsh.h"
#include "../src/history.h"

void history_setup(void) { history_init(); }

void history_teardown(void) {}

START_TEST(test_history_exp_new_command) {
  char *expansion;
  const int his_res = history_exp("new_command", &expansion);
  ck_assert(his_res == 0);
}
END_TEST

Suite *make_history_suite(void) {
  Suite *s = suite_create("History");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, history_setup, history_teardown);
  tcase_add_test(tc, test_history_exp_new_command);

  return s;
}
