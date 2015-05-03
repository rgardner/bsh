#include <stdlib.h>
#include <check.h>
#include "test_bsh.h"
#include "../src/parse.h"

void parse_setup(void) {}

void parse_teardown(void) {}

START_TEST(test_parse_empty) {
  struct ParseInfo *p = parse("");
  ck_assert(p == NULL);
}
END_TEST

START_TEST(test_parse_blanks) {
  struct ParseInfo *p = parse("       ");
  ck_assert(p == NULL);
}
END_TEST

START_TEST(test_parse_print_null) {
  // This should not segfault.
  print_info(NULL);
}
END_TEST

Suite *make_parse_suite(void) {
  Suite *s = suite_create("Parse");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, parse_setup, parse_teardown);
  tcase_add_test(tc, test_parse_empty);
  tcase_add_test(tc, test_parse_blanks);
  tcase_add_test(tc, test_parse_print_null);

  return s;
}
