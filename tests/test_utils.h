#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <check.h>

#ifdef ck_assert_ptr_null
#define bsh_assert_ptr_null(expr) ck_assert_ptr_null(expr)
#else
#define bsh_assert_ptr_null(expr) ck_assert_ptr_eq((expr), NULL)
#endif

#define bsh_assert_ptr_not_null(expr) ck_assert_ptr_ne((expr), NULL)

#endif
