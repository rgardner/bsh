#include "job.h"

#include <unistd.h>

/* The active jobs are linked into a list. This is its head. */
job *first_job = NULL;

/* Find the active job with the indicated pgid. */
job *
find_job(pid_t pgid)
{
  for (job *j = first_job; j; j = j->next) {
    if (j->pgid == pgid) {
      return j;
    }
  }
  return NULL;
}

/* Return true if all processes in the job have stopped or completed. */
int
job_is_stopped(job *j)
{
  for (process *p = j->first_process; p; p = p->next) {
    if (!p->completed && !p->stopped) {
      return 0;
    }
  }
  return 1;
}

/* Return true if all processes in the job have completed. */
int
job_is_completed(job *j)
{
  for (process *p = j->first_process; p; p = p->next) {
    if (!p->completed) {
      return 0;
    }
  }
  return 1;
}
