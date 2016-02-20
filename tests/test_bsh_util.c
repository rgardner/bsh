#include <check.h>
#include <stdlib.h>
#include "test_bsh.h"
#include "../src/util.h"

START_TEST(trim_newline) {
  char* s = strdup("user_input\n");
  size_t new_len = trim_right(s);
  ck_assert_str_eq(s, "user_input");
  ck_assert_int_eq(new_len, strlen("user_input"));
  free(s);
}
END_TEST

START_TEST(trim_no_newline) {
  char* s = strdup("user_input");
  size_t new_len = trim_right(s);
  ck_assert_str_eq(s, "user_input");
  ck_assert_int_eq(new_len, strlen("user_input"));
  free(s);
}
END_TEST

START_TEST(trim_lots_of_whitespace) {
  char* s = strdup("user_input        \n    ");
  size_t new_len = trim_right(s);
  ck_assert_str_eq(s, "user_input");
  ck_assert_int_eq(new_len, strlen("user_input"));
  free(s);
}
END_TEST

Suite *make_util_suite(void) {
  Suite *s = suite_create("Util");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_test(tc, trim_newline);
  tcase_add_test(tc, trim_no_newline);
  tcase_add_test(tc, trim_lots_of_whitespace);

  return s;
}
