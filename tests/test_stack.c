#include <string.h>

#include "minunit.h"
#include "../stack.h"

static char data[1024] = "element 1";
static struct stack *s;

void
test_setup()
{
  s = stack_init();
}

void test_teardown()
{
}

MU_TEST(test_init_normal)
{
  mu_assert(s != NULL, "stack should not be NULL");
}

MU_TEST(test_push_normal)
{
  char *elem;

  elem = strndup(data, sizeof(char)*strlen(data));
  stack_push(s, elem);
  mu_assert(stack_peak(s) == elem, "element should be on the top of the stack");
}

MU_TEST(test_pop_normal)
{
  char *elem;
  char *other;

  elem = strndup(data, strlen(data));
  stack_push(s, elem);
  other = stack_pop(s);
  mu_assert(elem == other, "pop should remove an element");
}

MU_TEST(test_push_pop)
{
  char *elem;

  elem = strndup(data, strlen(data));
  stack_push(s, elem);
  stack_pop(s);
  mu_assert(stack_peak(s) == NULL, "stack should be empty after push + pop");
}


MU_TEST_SUITE(test_suite) {
  MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

  MU_RUN_TEST(test_init_normal);
  MU_RUN_TEST(test_push_normal);
  MU_RUN_TEST(test_pop_normal);
  MU_RUN_TEST(test_push_pop);
}

int
main(int argc, char **argv)
{
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return 0;
}
