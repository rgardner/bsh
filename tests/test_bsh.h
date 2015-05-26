#ifndef TEST_BSH_H
#define TEST_BSH_H
#include <check.h>

void stack_setup(void);
void stack_teardown(void);

Suite *make_history_suite(void);
Suite *make_stack_suite(void);
Suite *make_parse_suite(void);
#endif /* TEST_BSH_H */
