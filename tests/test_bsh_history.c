#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "test_bsh.h"
#include "../src/history.h"

void history_setup(void) { history_init(); }

void history_teardown(void) {}

char *create_test_string(char* static_str) {
  const int len = strlen(static_str);
  char *new_str = malloc(sizeof(char) * (len + 1));
  strncpy(new_str, static_str, len);
  return new_str;
}

START_TEST(test_history_add_increments_length) {
  char *new_str = create_test_string("new_command\n");
  history_add(new_str);
  ck_assert_int_eq(history_length, 1);
  free(new_str);
}
END_TEST

START_TEST(test_history_exp_new_command) {
  char *expansion;
  const int his_res = history_exp("new_command", &expansion);
  ck_assert_int_eq(his_res, 0);
}
END_TEST

START_TEST(test_history_exp_error) {
  char *expansion;
  const int his_res = history_exp("!invalid\n", &expansion);
  ck_assert_int_eq(his_res, -1);
}
END_TEST

START_TEST(test_history_exp_occurred) {
  char *new_str = create_test_string("new_command");
  history_add(new_str);
  char *expansion;
  const int his_res = history_exp("!-1\n", &expansion);
  ck_assert_int_eq(his_res, 1);
  ck_assert_str_eq(expansion, "new_command");
  free(new_str);
}
END_TEST

Suite *make_history_suite(void) {
  Suite *s = suite_create("History");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, history_setup, history_teardown);
  tcase_add_test(tc, test_history_add_increments_length);
  tcase_add_test(tc, test_history_exp_new_command);
  tcase_add_test(tc, test_history_exp_error);
  tcase_add_test(tc, test_history_exp_occurred);

  return s;
}
