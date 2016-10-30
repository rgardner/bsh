#include "../src/history.h"
#include "test_bsh.h"
#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void
history_setup(void)
{
  history_init();
}

void
history_teardown(void)
{
}

START_TEST(test_history_add_increments_length)
{
  ck_assert_int_eq(history_length, 0);
  char* s = strdup("new_command");
  history_add(s);
  ck_assert_int_eq(history_length, 1);
  free(s);
}
END_TEST

START_TEST(test_history_exp_new_command)
{
  char* expansion;
  const int his_res = history_exp("new_command", &expansion);
  ck_assert_int_eq(his_res, 0);
}
END_TEST

START_TEST(test_history_exp_error)
{
  char* expansion;
  const int his_res = history_exp("!invalid", &expansion);
  ck_assert_int_eq(his_res, -1);
}
END_TEST

START_TEST(test_history_exp_occurred)
{
  char* s = strdup("new_command");
  history_add(s);
  char* expansion;
  const int his_res = history_exp("!-1\n", &expansion);
  ck_assert_int_eq(his_res, 1);
  ck_assert_str_eq(expansion, "new_command");
  free(s);
}
END_TEST

Suite*
make_history_suite(void)
{
  Suite* s = suite_create("History");

  /* Core test case. */
  TCase* tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, history_setup, history_teardown);
  tcase_add_test(tc, test_history_add_increments_length);
  tcase_add_test(tc, test_history_exp_new_command);
  tcase_add_test(tc, test_history_exp_error);
  tcase_add_test(tc, test_history_exp_occurred);

  return s;
}
