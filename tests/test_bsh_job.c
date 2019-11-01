#include "job.h"

#include <stdlib.h>
#include <termios.h>

#include "check.h"
#include "parse.h"

#include "test_utils.h"

struct termios tmodes;

START_TEST(test_job_init)
{
  struct ParseInfo* p = parse("command1 var1 | command2 var2");
  bsh_assert_ptr_not_null(p);

  struct job* j = malloc(sizeof(struct job));
  init_job(j, p, 0, tmodes);

  bsh_assert_ptr_null(j->next);
  ck_assert_int_eq(j->first_process->argc, 2);

  job_free(j);
  free(j);
  free_info(p);
}
END_TEST

Suite*
make_job_suite(void)
{
  Suite* s = suite_create("Job");

  /* Core test case. */
  TCase* tc = tcase_create("Core");

  suite_add_tcase(s, tc);

  tcase_add_test(tc, test_job_init);

  return s;
}
