#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

START_TEST(test_parse_file_redirection) {
  struct ParseInfo *p = parse("echo <infile >outfile\n");
  ck_assert(p->hasInputRedirection == true);
  ck_assert(p->hasOutputRedirection == true);
  ck_assert(!strncmp(p->inFile, "infile", strlen("infile")));
  ck_assert(!strncmp(p->outFile, "outfile", strlen("outfile")));
}
END_TEST

START_TEST(test_parse_background) {
  struct ParseInfo *p = parse("long_running_task &\n");
  ck_assert(p->runInBackground == true);
}
END_TEST

START_TEST(test_parse_piping) {
  struct ParseInfo *p = parse("command1 | command2 | command3\n");
  ck_assert(p->pipeNum == 3);
  ck_assert(!strncmp(p->CommArray[0].command, "command1", strlen("command1")));
  ck_assert(!strncmp(p->CommArray[1].command, "command2", strlen("command2")));
  ck_assert(!strncmp(p->CommArray[2].command, "command3", strlen("command3")));
}
END_TEST

START_TEST(test_parse_variables) {
  struct ParseInfo *p = parse("command var1 var2 var3\n");
  ck_assert(p->CommArray[0].VarNum == 3);
  ck_assert(!strncmp(p->CommArray[0].VarList[0], "var1", strlen("var1")));
  ck_assert(!strncmp(p->CommArray[0].VarList[1], "var2", strlen("var2")));
  ck_assert(!strncmp(p->CommArray[0].VarList[2], "var3", strlen("var3")));
}
END_TEST

START_TEST(test_parse_free_null) {
  // This should not segfault.
  free_info(NULL);
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
  tcase_add_test(tc, test_parse_file_redirection);
  tcase_add_test(tc, test_parse_background);
  tcase_add_test(tc, test_parse_piping);
  tcase_add_test(tc, test_parse_variables);
  tcase_add_test(tc, test_parse_free_null);
  tcase_add_test(tc, test_parse_print_null);

  return s;
}
