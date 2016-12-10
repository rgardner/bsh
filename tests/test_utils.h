#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include <check.h>

#define ck_assert_ptr_null(expr) ck_assert_ptr_eq(expr, NULL)
#define ck_assert_ptr_not_null(expr) ck_assert_ptr_ne(expr, NULL)

#endif
