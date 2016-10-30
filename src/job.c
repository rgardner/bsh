#include "job.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Function prototypes. */
void init_process(process*, const struct Command);
void process_free(process*);
void job_free(job*);

/* The active jobs are linked into a list. This is its head. */
job* first_job = NULL;

/* Find the active job with the indicated pgid. */
job*
find_job(pid_t pgid)
{
  for (job* j = first_job; j; j = j->next) {
    if (j->pgid == pgid) {
      return j;
    }
  }
  return NULL;
}

/* Return true if all processes in the job have stopped or completed. */
int
job_is_stopped(job* j)
{
  for (process* p = j->first_process; p; p = p->next) {
    if (!p->completed && !p->stopped) {
      return 0;
    }
  }
  return 1;
}

/* Return true if all processes in the job have completed. */
int
job_is_completed(job* j)
{
  for (process* p = j->first_process; p; p = p->next) {
    if (!p->completed) {
      return 0;
    }
  }
  return 1;
}

void
init_process(process* p, const struct Command cmd)
{
  p->next = NULL;
  p->argc = cmd.VarNum + 1;  // command, *args

  p->argv = malloc((p->argc + 1) * sizeof(char*));  // command, *args, NULL
  p->argv[0] = strdup(cmd.command);
  for (int i = 0; i < cmd.VarNum; i++) {
    p->argv[i + 1] = strdup(cmd.VarList[i]);
  }
  p->argv[p->argc] = NULL;

  p->pid = 0;
  p->completed = false;
  p->stopped = false;
  p->status = 0;
}

void
init_job(job* j,
         const struct ParseInfo* info,
         pid_t pgid,
         struct termios tmodes)
{
  /* TODO(rgardner): allow multiple jobs. */
  j->next = NULL;
  /* TODO: use original command entered here, not stored in info. */
  j->command = NULL;

  process* prev = NULL;
  for (int i = 0; i <= info->pipeNum; i++) {
    process* p = malloc(sizeof(process));
    init_process(p, info->CommArray[i]);
    if (j->first_process && prev) {
      prev->next = p;
    } else {
      j->first_process = p;
    }
    prev = p;
  }

  j->pgid = pgid;
  j->tmodes = tmodes;

  if (info->hasInputRedirection) {
    j->infile = open(info->inFile, O_RDONLY);
  } else {
    j->infile = STDIN_FILENO;
  }

  if (info->hasOutputRedirection) {
    // The created file has -rw-r--r-- permissions
    j->outfile = open(info->outFile, O_WRONLY | O_CREAT, 0644);
  } else {
    j->outfile = STDOUT_FILENO;
  }

  j->errfile = STDERR_FILENO;
}

void
process_free(process* p)
{
  for (int i = 0; i < p->argc; i++) {
    free(p->argv[i]);
  }

  for (process* next = p->next; next; next = next->next) {
    process_free(next);
  }
}

void
job_free(job* j)
{
  free(j->command);
  for (process* p = j->first_process; p; p = p->next) {
    process_free(p);
  }
}

void
process_print(process* p)
{
  printf("prog: %s\n", p->argv[0]);
  for (int i = 1; i < p->argc; i++) {
    printf("%s, ", p->argv[i]);
  }
  printf("\n");
}

void
job_print(job* j)
{
  for (process* p = j->first_process; p; p = p->next) {
    process_print(p);
  }
}
