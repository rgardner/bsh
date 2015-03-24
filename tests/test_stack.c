#include <stdlib.h>
#include <check.h>
#include "../src/stack.h"

START_TEST(test_stack_create)
{
  struct Stack *s;

  s = stack_init();
  ck_assert(s != NULL);
  stack_free(s);
}
END_TEST

Suite * stack_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Stack");

  /* Core test case. */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_stack_create);
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

/*static char data[1024] = "element 1";*/
/*static struct Stack *s;*/

/*void*/
/*test_setup()*/
/*{*/
  /*s = stack_init();*/
/*}*/

/*void test_teardown()*/
/*{*/
/*}*/

/*MU_TEST(test_init_normal)*/
/*{*/
  /*mu_assert(s != NULL, "stack should not be NULL");*/
/*}*/

/*MU_TEST(test_push_normal)*/
/*{*/
  /*char *elem = strndup(data, sizeof(char)*strlen(data));*/
  /*stack_push(s, elem);*/
  /*mu_assert(stack_peak(s) == elem, "element should be on the top of the stack");*/
/*}*/

/*MU_TEST(test_pop_normal)*/
/*{*/
  /*char *elem = strndup(data, strlen(data));*/
  /*stack_push(s, elem);*/
  /*char *other = stack_pop(s);*/
  /*mu_assert(elem == other, "pop should remove an element");*/
/*}*/

/*MU_TEST(test_push_pop)*/
/*{*/
  /*char *elem = strndup(data, strlen(data));*/
  /*stack_push(s, elem);*/
  /*stack_pop(s);*/
  /*mu_assert(stack_peak(s) == NULL, "stack should be empty after push + pop");*/
/*}*/


/*MU_TEST_SUITE(test_suite) {*/
  /*MU_SUITE_CONFIGURE(&test_setup, &test_teardown);*/

  /*MU_RUN_TEST(test_init_normal);*/
  /*MU_RUN_TEST(test_push_normal);*/
  /*MU_RUN_TEST(test_pop_normal);*/
  /*MU_RUN_TEST(test_push_pop);*/
/*}*/

/*int*/
/*main(int argc, char **argv)*/
/*{*/
  /*MU_RUN_SUITE(test_suite);*/
  /*MU_REPORT();*/
  /*return 0;*/
/*}*/
