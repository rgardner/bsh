#include "../src/history.h"
#include "test_bsh.h"
#include "test_utils.h"

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
  history_free();
}

START_TEST(test_history_add)
{
  history_add("command");
  char* actual = NULL;
  ck_assert_int_eq(history_exp("!1", &actual), 1);
  ck_assert_str_eq(actual, "command");
}
END_TEST

START_TEST(test_history_add_rollover)
{
  for (int i = 0; i < HISTSIZE; i++) {
    char* s = NULL;
    ck_assert_int_ge(asprintf(&s, "command%d", i + 1), 0);
    history_add(s);
    free(s);
  }

  history_add("overflow");

  char* expansion = NULL;
  const int his_res = history_exp("!1", &expansion);
  ck_assert_int_eq(his_res, -1);
  ck_assert_ptr_null(expansion);
}
END_TEST

START_TEST(test_history_exp_not_found)
{
  char* expansion = NULL;
  const int his_res = history_exp("!1", &expansion);
  ck_assert_int_eq(his_res, -1);
  ck_assert_ptr_null(expansion);
}
END_TEST

START_TEST(test_history_exp_found_pos)
{
  history_add("command1");
  char* expansion = NULL;
  const int his_res = history_exp("!1", &expansion);
  ck_assert_int_eq(his_res, 1);
  ck_assert_str_eq(expansion, "command1");
  free(expansion);
}
END_TEST

START_TEST(test_history_exp_found_neg)
{
  history_add("command1");
  char* expansion = NULL;
  const int his_res = history_exp("!-1", &expansion);
  ck_assert_int_eq(his_res, 1);
  ck_assert_str_eq(expansion, "command1");
  free(expansion);

  history_add("command2");
  char* expansion2 = NULL;
  const int his_res2 = history_exp("!-1", &expansion2);
  ck_assert_int_eq(his_res2, 1);
  ck_assert_str_eq(expansion2, "command2");
  free(expansion2);

  char* expansion3 = NULL;
  const int his_res3 = history_exp("!-2", &expansion3);
  ck_assert_int_eq(his_res3, 1);
  ck_assert_str_eq(expansion2, "command1");
  free(expansion3);
}
END_TEST

START_TEST(test_history_exp_invalid_event)
{
  char* expansion = NULL;
  const int his_res = history_exp("!invalid", &expansion);
  ck_assert_int_eq(his_res, -1);
  ck_assert_ptr_null(expansion);
}
END_TEST

Suite*
make_history_suite()
{
  Suite* s = suite_create("History");
  TCase* tc = tcase_create("Core");
  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, history_setup, history_teardown);

  tcase_add_test(tc, test_history_add);
  tcase_add_test(tc, test_history_add_rollover);
  tcase_add_test(tc, test_history_exp_not_found);
  tcase_add_test(tc, test_history_exp_found_neg);
  tcase_add_test(tc, test_history_exp_found_pos);
  tcase_add_test(tc, test_history_exp_invalid_event);

  return s;
}
