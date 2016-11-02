#include "../src/circular_queue.h"
#include "test_bsh.h"

#include <check.h>

START_TEST(test_cq_init_free)
{
  circular_queue* queue_normal = circular_queue_init(10);
  ck_assert(queue_normal);
  circular_queue_free(queue_normal);

  circular_queue* queue_zero = circular_queue_init(0);
  ck_assert(queue_zero);
  circular_queue_free(queue_zero);
}
END_TEST

START_TEST(test_cq_push_one)
{
  
}
END_TEST

START_TEST(test_cq_push_above_capacity)
{
  
}
END_TEST

START_TEST(test_cq_increase_capacity)
{
  
}
END_TEST

START_TEST(test_cq_decrease_capacity)
{
  
}
END_TEST

Suite*
make_circular_queue_suite()
{
  Suite* s = suite_create("Circular Queue");
  TCase* tc = tcase_create("Core");
  suite_add_tcase(s, tc);
  // tcase_add_checked_fixture(tc, history_setup, history_teardown);

  tcase_add_test(tc, test_cq_init_free);
  tcase_add_test(tc, test_cq_push_one);
  tcase_add_test(tc, test_cq_push_above_capacity);
  tcase_add_test(tc, test_cq_increase_capacity);
  tcase_add_test(tc, test_cq_decrease_capacity);

  return s;
}
