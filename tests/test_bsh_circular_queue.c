#include "../src/circular_queue.h"
#include "test_bsh.h"

#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(test_cq_init_free)
{
  circular_queue* queue_normal = circular_queue_init(10);
  ck_assert(queue_normal);
  circular_queue_free(queue_normal);

  circular_queue* queue_one = circular_queue_init(1);
  ck_assert(queue_one);
  circular_queue_free(queue_one);
}
END_TEST

START_TEST(test_cq_push_one)
{
  circular_queue* queue = circular_queue_init(10);
  void* elem = strdup("0th element");
  circular_queue_push(queue, elem);
  void* ret = circular_queue_get(queue, 0);
  ck_assert_ptr_eq(ret, elem);
  circular_queue_free(queue);
}
END_TEST

START_TEST(test_cq_push_above_capacity)
{
  // 1 capacity: push is in-place swap
  circular_queue* queue1 = circular_queue_init(1);
  void* elem = strdup("count0");
  circular_queue_push(queue1, elem);
  void* ret = circular_queue_get(queue1, 0);
  ck_assert_ptr_eq(ret, elem);

  void* another_elem = strdup("count1");
  circular_queue_push(queue1, another_elem);
  ret = circular_queue_get(queue1, 1);
  ck_assert_ptr_eq(ret, another_elem);
  circular_queue_free(queue1);

  // 2 capacity: simple to reason about
  circular_queue* queue2 = circular_queue_init(2);
  circular_queue_push(queue2, strdup("count0"));
  circular_queue_push(queue2, strdup("count1"));
  ck_assert(circular_queue_get(queue2, 0));
  ck_assert(circular_queue_get(queue2, 1));
  circular_queue_push(queue2, strdup("count2"));
  ck_assert(!circular_queue_get(queue2, 0));
  ck_assert(circular_queue_get(queue2, 1));
  ck_assert(circular_queue_get(queue2, 2));
  circular_queue_free(queue2);

  // N capacity: normal case
  size_t capacity = 10;
  size_t elems_to_add = 100;
  circular_queue* norm = circular_queue_init(capacity);
  for (size_t i = 0; i < elems_to_add; i++) {
    char* dyn_elem;
    asprintf(&dyn_elem, "e%zu", i);
    circular_queue_push(norm, dyn_elem);
  }

  for (size_t i = 0; i < elems_to_add - capacity; i++) {
    ck_assert(!(circular_queue_get(norm, i)));
  }

  for (size_t i = elems_to_add - capacity; i < elems_to_add; i++) {
    char* computed;
    asprintf(&computed, "e%zu", i);
    char* actual = circular_queue_get(norm, i);
    ck_assert_str_eq(actual, computed);
    free(computed);
  }

  circular_queue_free(norm);
}
END_TEST

START_TEST(test_cq_increase_capacity)
{
  ck_assert(false);
}
END_TEST

START_TEST(test_cq_decrease_capacity)
{
  ck_assert(false);
}
END_TEST

Suite*
make_circular_queue_suite()
{
  Suite* s = suite_create("Circular Queue");
  TCase* tc = tcase_create("Core");
  suite_add_tcase(s, tc);

  tcase_add_test(tc, test_cq_init_free);
  tcase_add_test(tc, test_cq_push_one);
  tcase_add_test(tc, test_cq_push_above_capacity);
  tcase_add_test(tc, test_cq_increase_capacity);
  tcase_add_test(tc, test_cq_decrease_capacity);

  return s;
}
