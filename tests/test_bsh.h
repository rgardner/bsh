#ifndef TEST_BSH_H
#define TEST_BSH_H
#include <check.h>

void stack_setup();
void stack_teardown();

Suite* make_alias_suite();
Suite* make_circular_queue_suite();
Suite* make_history_suite();
Suite* make_job_suite();
Suite* make_stack_suite();
Suite* make_parse_suite();
Suite* make_util_suite();
#endif /* TEST_BSH_H */
