#include <stdlib.h>
#include <check.h>
#include "../src/stack.h"

static char data[1024] = "element 1";
static struct Stack *s;

void
setup(void)
{
  s = stack_init();
}

void
teardown(void)
{
  stack_free(s);
}

START_TEST(test_stack_create)
{
  ck_assert_msg(s != NULL, "new stack should not be NULL");
}
END_TEST

START_TEST(test_push_normal)
{
  char *elem = strndup(data, sizeof(char)*strlen(data));
  stack_push(s, elem);
  ck_assert_msg(stack_peak(s) == elem,
                "element should be on the top of the stack");
  free(elem);
}
END_TEST

START_TEST(test_pop_normal)
{
  char *elem = strndup(data, strlen(data));
  stack_push(s, elem);
  char *other = stack_pop(s);
  ck_assert_msg(elem == other, "pop should remove an element");
  free(elem);
}
END_TEST

START_TEST(test_push_pop)
{
  char *elem = strndup(data, strlen(data));
  stack_push(s, elem);
  stack_pop(s);
  ck_assert_msg(stack_peak(s) == NULL,
                "stack should be empty after push + pop");
  free(elem);
}
END_TEST

Suite * stack_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Stack");

  /* Core test case. */
  tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_stack_create);
  tcase_add_test(tc_core, test_push_normal);
  tcase_add_test(tc_core, test_pop_normal);
  tcase_add_test(tc_core, test_push_pop);
  suite_add_tcase(s, tc_core);

  return s;
}

int
main(void)
{
  Suite *s = stack_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
