#include "../src/circular_queue.h"
#include "test_bsh.h"
#include "test_utils.h"

#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(test_cq_init_free)
{
  circular_queue* queue_normal = circular_queue_init(10);
  ck_assert_ptr_not_null(queue_normal);
  circular_queue_free(queue_normal, NULL);

  circular_queue* queue_one = circular_queue_init(1);
  ck_assert_ptr_not_null(queue_one);
  circular_queue_free(queue_one, NULL);
}
END_TEST

START_TEST(test_cq_push_one)
{
  circular_queue* queue = circular_queue_init(10);
  void* elem = "0th element";
  ck_assert_ptr_null(circular_queue_push(queue, elem));
  void* retval = circular_queue_get(queue, 0);
  ck_assert_ptr_eq(elem, retval);
  circular_queue_free(queue, NULL);
}
END_TEST

START_TEST(test_cq_push_above_capacity)
{
  // 1 capacity: push is in-place swap
  circular_queue* queue1 = circular_queue_init(1);
  ck_assert_ptr_not_null(queue1);
  void* elem = "count0";
  ck_assert_ptr_null(circular_queue_push(queue1, elem));
  void* retval = circular_queue_get(queue1, 0);
  ck_assert_ptr_eq(retval, elem);

  void* another_elem = "count1";
  ck_assert_ptr_not_null(circular_queue_push(queue1, another_elem));
  retval = circular_queue_get(queue1, 1);
  ck_assert_ptr_eq(retval, another_elem);
  circular_queue_free(queue1, NULL);

  // 2 capacity: simple to reason about
  circular_queue* queue2 = circular_queue_init(2);
  ck_assert_ptr_not_null(queue2);
  ck_assert_ptr_null(circular_queue_push(queue2, "count0"));
  ck_assert_ptr_null(circular_queue_push(queue2, "count1"));
  ck_assert_ptr_not_null(circular_queue_get(queue2, 0));
  ck_assert_ptr_not_null(circular_queue_get(queue2, 1));
  ck_assert_ptr_not_null(circular_queue_push(queue2, "count2"));
  ck_assert_ptr_null(circular_queue_get(queue2, 0));
  ck_assert_ptr_not_null(circular_queue_get(queue2, 1));
  ck_assert_ptr_not_null(circular_queue_get(queue2, 2));
  circular_queue_free(queue2, NULL);

  // N capacity: normal case
  const size_t capacity = 10;
  const size_t elems_to_add = 100;
  circular_queue* norm = circular_queue_init(capacity);
  ck_assert_ptr_not_null(norm);
  for (size_t i = 0; i < elems_to_add; i++) {
    char* dyn_elem = NULL;
    ck_assert_int_ge(asprintf(&dyn_elem, "e%zu", i), 0);
    char* old = circular_queue_push(norm, dyn_elem);
    if (i > capacity) {
      ck_assert_ptr_not_null(old);
      free(old);
    }
  }

  for (size_t i = 0; i < elems_to_add - capacity; i++) {
    ck_assert_ptr_null(circular_queue_get(norm, i));
  }

  for (size_t i = elems_to_add - capacity; i < elems_to_add; i++) {
    char* expected = NULL;
    ck_assert_int_ge(asprintf(&expected, "e%zu", i), 0);
    char* actual = circular_queue_get(norm, i);
    ck_assert_str_eq(actual, expected);
    free(expected);
  }

  circular_queue_free(norm, free);
}
END_TEST

START_TEST(test_cq_get)
{
  circular_queue* queue1 = circular_queue_init(1);
  ck_assert_ptr_not_null(queue1);
  ck_assert_ptr_null(circular_queue_get(queue1, 0));
  circular_queue_free(queue1, NULL);
}
END_TEST

START_TEST(test_cq_increase_capacity)
{
  // increase capacity without re-ordering
  circular_queue* queue1 = circular_queue_init(5);
  ck_assert_ptr_not_null(queue1);
  for (int i = 0; i < 5; i++) {
    char* elem = NULL;
    ck_assert_int_ge(asprintf(&elem, "elem%d", i), 0);
    ck_assert_ptr_null(circular_queue_push(queue1, elem));
  }

  ck_assert(circular_queue_set_capacity(queue1, 10, NULL));

  // verify set capacity does not affect queue get
  for (int i = 0; i < 5; i++) {
    char* expected = NULL;
    ck_assert_int_ge(asprintf(&expected, "elem%d", i), 0);
    char* actual = circular_queue_get(queue1, i);
    ck_assert_str_eq(actual, expected);
    free(expected);
  }

  for (int i = 5; i < 10; i++) {
    char* elem = NULL;
    ck_assert_int_ge(asprintf(&elem, "elem%d", i), 0);
    ck_assert_ptr_null(circular_queue_push(queue1, elem));
  }

  for (int i = 0; i < 10; i++) {
    char* expected = NULL;
    ck_assert_int_ge(asprintf(&expected, "elem%d", i), 0);
    char* actual = circular_queue_get(queue1, i);
    ck_assert_str_eq(actual, expected);
    free(expected);
  }

  circular_queue_free(queue1, free);

  // increase capacity with reordering
  circular_queue* queue2 = circular_queue_init(10);
  ck_assert_ptr_not_null(queue2);
  for (int i = 0; i < 15; i++) {
    char* elem = NULL;
    ck_assert_int_ge(asprintf(&elem, "elem%d", i), 0);
    char* old = circular_queue_push(queue2, elem);
    if (old) {
      free(old);
    }
  }

  ck_assert(circular_queue_set_capacity(queue2, 15, NULL));
  for (int i = 0; i < 5; i++) {
    char* elem = NULL;
    ck_assert_int_ge(asprintf(&elem, "elem%d", i + 15), 0);
    ck_assert_ptr_null(circular_queue_push(queue2, elem));
  }

  for (int i = 0; i < 20; i++) {
    char* actual = circular_queue_get(queue2, i);
    if (i < 5) {
      ck_assert_ptr_null(actual);
    } else {
      char* expected = NULL;
      ck_assert_int_ge(asprintf(&expected, "elem%d", i), 0);
      ck_assert_str_eq(actual, expected);
      free(expected);
    }
  }

  circular_queue_free(queue2, free);
}
END_TEST

START_TEST(test_cq_increase_capacity_correct_slots)
{
  circular_queue* queue = circular_queue_init(1);
  ck_assert_ptr_not_null(queue);
  ck_assert_ptr_null(circular_queue_push(queue, "elem0"));
  ck_assert_ptr_not_null(circular_queue_push(queue, "elem1"));
  ck_assert(circular_queue_set_capacity(queue, 3, NULL));
  ck_assert_str_eq(queue->entries[1], "elem1");
  ck_assert_ptr_null(queue->entries[0]);
  ck_assert_ptr_null(queue->entries[2]);
}
END_TEST

START_TEST(test_cq_decrease_capacity_no_rollover)
{
  // decrease non-rollover queue without loss
  circular_queue* queue_roll_noloss = circular_queue_init(5);
  ck_assert_ptr_not_null(queue_roll_noloss);
  ck_assert_ptr_null(circular_queue_push(queue_roll_noloss, "elem0"));
  ck_assert_ptr_null(circular_queue_push(queue_roll_noloss, "elem1"));
  ck_assert(circular_queue_set_capacity(queue_roll_noloss, 3, NULL));
  ck_assert_str_eq(circular_queue_get(queue_roll_noloss, 0), "elem0");
  ck_assert_str_eq(circular_queue_get(queue_roll_noloss, 1), "elem1");
  circular_queue_free(queue_roll_noloss, NULL);

  // decrease non-rollover with loss
  circular_queue* queue_noroll_loss = circular_queue_init(2);
  ck_assert_ptr_not_null(queue_noroll_loss);
  ck_assert_ptr_null(circular_queue_push(queue_noroll_loss, "elem0"));
  ck_assert_ptr_null(circular_queue_push(queue_noroll_loss, "elem1"));
  ck_assert(circular_queue_set_capacity(queue_noroll_loss, 1, NULL));
  ck_assert_ptr_null(circular_queue_get(queue_noroll_loss, 0));
  ck_assert_str_eq(circular_queue_get(queue_noroll_loss, 1), "elem1");
  circular_queue_free(queue_noroll_loss, NULL);

  // decrease non-rollover with larger loss
  circular_queue* queue_noroll_loss2 = circular_queue_init(5);
  ck_assert_ptr_not_null(queue_noroll_loss2);
  ck_assert_ptr_null(circular_queue_push(queue_noroll_loss2, "elem0"));
  ck_assert_ptr_null(circular_queue_push(queue_noroll_loss2, "elem1"));
  ck_assert(circular_queue_set_capacity(queue_noroll_loss2, 1, NULL));
  ck_assert_ptr_null(circular_queue_get(queue_noroll_loss2, 0));
  ck_assert_str_eq(circular_queue_get(queue_noroll_loss2, 1), "elem1");
  circular_queue_free(queue_noroll_loss2, NULL);
}
END_TEST

START_TEST(test_cq_decrease_capacity_rollover)
{
  circular_queue* queue_noroll_loss2 = circular_queue_init(5);
  ck_assert_ptr_not_null(queue_noroll_loss2);
  for (int i = 0; i < 5; i++) {
    char* elem = NULL;
    ck_assert_int_ge(asprintf(&elem, "elem%d", i), 0);
    ck_assert_ptr_null(circular_queue_push(queue_noroll_loss2, elem));
  }

  ck_assert(circular_queue_set_capacity(queue_noroll_loss2, 3, free));
  for (int i = 0; i < 5; i++) {
    char* actual = circular_queue_get(queue_noroll_loss2, i);
    if (i < 2) {
      ck_assert_ptr_null(actual);
    } else {
      char* expected = NULL;
      ck_assert_int_ge(asprintf(&expected, "elem%d", i), 0);
      ck_assert_str_eq(actual, expected);
      free(expected);
    }
  }

  circular_queue_free(queue_noroll_loss2, free);

  // decrease rollover with loss
  circular_queue* queue_roll_loss = circular_queue_init(5);
  ck_assert_ptr_not_null(queue_roll_loss);
  for (int i = 0; i < 7; i++) {
    char* elem = NULL;
    ck_assert_int_ge(asprintf(&elem, "elem%d", i), 0);
    char* old = circular_queue_push(queue_roll_loss, elem);
    if (old) {
      free(old);
    }
  }

  ck_assert(circular_queue_set_capacity(queue_roll_loss, 3, free));
  for (size_t i = 0; i < queue_roll_loss->count; i++) {
    char* actual = circular_queue_get(queue_roll_loss, i);
    if (i < 4) {
      ck_assert_ptr_null(actual);
    } else {
      char* expected = NULL;
      ck_assert_int_ge(asprintf(&expected, "elem%zu", i), 0);
      ck_assert_str_eq(actual, expected);
      free(expected);
    }
  }

  circular_queue_free(queue_roll_loss, free);
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
  tcase_add_test(tc, test_cq_get);
  tcase_add_test(tc, test_cq_increase_capacity);
  tcase_add_test(tc, test_cq_increase_capacity_correct_slots);
  tcase_add_test(tc, test_cq_decrease_capacity_rollover);
  tcase_add_test(tc, test_cq_decrease_capacity_no_rollover);

  return s;
}
