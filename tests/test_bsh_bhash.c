#include <check.h>
#include "../src/bhash.h"

void bhash_setup(void) {}

void bhash_teardown(void) {}

START_TEST(test_bhash_create) {
  hashtable_t *hashtable = ht_create(65536);
  ck_assert(hashtable != NULL);
}
END_TEST

Suite *make_bhash_suite(void) {
  Suite *s = suite_create("BHash");

  /* Core test case. */
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, bhash_setup, bhash_teardown);
  tcase_add_test(tc, test_bhash_create);

  return s;
}
