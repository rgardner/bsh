#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <check.h>

#ifdef __linux__
#define BSH_TEST_ADD_TEST_DISABLED_ON_LINUX(tc, test, message) (void)(tc), (void)(test), (void)(message)
#else
#define BSH_TEST_ADD_TEST_DISABLED_ON_LINUX(tc, test, message) tcase_add_test(tc, test); (void)(message)
#endif

#ifdef ck_assert_ptr_null
#define bsh_assert_ptr_null(expr) ck_assert_ptr_null(expr)
#else
#define bsh_assert_ptr_null(expr) ck_assert_ptr_eq((expr), NULL)
#endif

#define bsh_assert_ptr_not_null(expr) ck_assert_ptr_ne((expr), NULL)

#endif
