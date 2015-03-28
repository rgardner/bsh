#include "job.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/* Function prototypes. */
int init_process(process *, const struct Command);
void process_free(process *);
void job_free(job *);


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

int
init_process(process *p, const struct Command cmd)
{
  p->next = NULL;
  p->argc = cmd.VarNum;

  p->argv = malloc((p->argc+2) * sizeof(char *));  // command, *args, NULL
  p->argv[0] = cmd.command;
  for (int i = 0; i < p->argc; i++) {
    p->argv[i + 1] = cmd.VarList[i];
  }
  p->argv[cmd.VarNum + 1] = NULL;

  p->pid = 0;
  p->completed = false;
  p->stopped = false;
  p->status = 0;
  return 0;
}

int
init_job(job *j, const struct ParseInfo *info, pid_t pgid, struct termios tmodes)
{
  j->next = NULL;
  /* FIXME: use original command entered here, not stored in info. */
  j->command = NULL;

  process *prev = NULL;
  for (int i = 0; i < info->pipeNum; i++) {
    process *p = malloc(sizeof(process));
    init_process(p, info->CommArray[i]);
    if (j->first_process) {
      prev->next = p;
    } else {
      j->first_process = p;
    }
    prev = p;
  }

  j->pgid = pgid;
  j->tmodes = tmodes;

  if (info->hasInputRedirection) {
    j->stdin = open(info->inFile, O_RDONLY);
  } else {
    j->stdin = STDIN_FILENO;
  }

  if (info->hasOutputRedirection) {
    j->stdout = open(info->outFile, O_WRONLY | O_CREAT, 0644);
  } else {
    j->stdout = STDOUT_FILENO;
  }

  j->stderr = STDERR_FILENO;

  return 0;
}

void
process_free(process *p)
{
  for (int i = 0; i < p->argc; i++) {
    free(p->argv[i]);
  }

  for (process *next = p->next; next; next = next->next) {
    process_free(next);
  }
}

void
job_free(job *j)
{
  free(j->command);
  for (process *p = j->first_process; p; p = p->next) {
    process_free(p);
  }
}
