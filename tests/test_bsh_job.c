#include "job.h"

#include <stdlib.h>
#include <termios.h>

#include "parse.h"
#include "check.h"

#include "test_utils.h"

struct termios tmodes;
job* j;
struct ParseInfo* p;

void
job_setup(void)
{
  j = malloc(sizeof(job));
}

void
job_teardown(void)
{
  free(j);
  free(p);
}

START_TEST(test_job_init)
{
  p = parse("command1 var1 | command2 var2");
  init_job(j, p, 0, tmodes);
  ck_assert(!j->next);
  ck_assert_int_eq(j->first_process->argc, 2);
}
END_TEST

Suite*
make_job_suite(void)
{
  Suite* s = suite_create("Job");

  /* Core test case. */
  TCase* tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, job_setup, job_teardown);

  BSH_TEST_ADD_TEST_DISABLED_ON_LINUX(tc, test_job_init, "https://github.com/rgardner/bsh/issues/16");

  return s;
}
