#include <stdlib.h>
#include <check.h>
#include "test_bsh.h"
#include "../src/stack.h"

static char data[1024] = "element 1";
static struct Stack *s;

void stack_setup(void) { s = stack_init(); }

void stack_teardown(void) { stack_free(s); }

START_TEST(test_stack_create) {
  ck_assert_msg(s != NULL, "new stack should not be NULL");
}
END_TEST

START_TEST(test_push_normal) {
  char *elem = strndup(data, sizeof(char) * strlen(data));
  stack_push(s, elem);
  ck_assert_msg(stack_peak(s) == elem,
                "element should be on the top of the stack");
  free(elem);
}
END_TEST

START_TEST(test_pop_normal) {
  char *elem = strndup(data, strlen(data));
  stack_push(s, elem);
  char *other = stack_pop(s);
  ck_assert_msg(elem == other, "pop should remove an element");
  free(elem);
}
END_TEST

START_TEST(test_push_pop) {
  char *elem = strndup(data, strlen(data));
  stack_push(s, elem);
  stack_pop(s);
  ck_assert_msg(stack_peak(s) == NULL,
                "stack should be empty after push + pop");
  free(elem);
}
END_TEST

Suite *make_stack_suite(void) {
  Suite *s = suite_create("Stack");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, stack_setup, stack_teardown);
  tcase_add_test(tc, test_stack_create);
  tcase_add_test(tc, test_push_normal);
  tcase_add_test(tc, test_pop_normal);
  tcase_add_test(tc, test_push_pop);

  return s;
}
