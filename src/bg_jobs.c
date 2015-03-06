#include "bg_jobs.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

/* Global variables. */
int num_bg_jobs;
struct BGJob *background_jobs[MAX_BG_JOBS];

struct BGJob *
job_init(const pid_t pid, const struct ParseInfo *info, const struct Command *cmd)
{
  struct BGJob init = { .pid = pid, .info = info, .cmd = cmd };
  struct BGJob *job = malloc(sizeof(struct BGJob));
  memcpy(job, &init, sizeof(struct BGJob));
  return job;
}

void
handle_sigchld(const int signum)
{
  if (signum != SIGCHLD) return;

  pid_t pid = waitpid((pid_t)-1, 0, WNOHANG);

  // find the bg job that exited.
  struct BGJob *job;
  int i;
  for (i = 0; i < num_bg_jobs; i++) {
    job = background_jobs[i];
    if (job && job->pid == pid) break;
  }

  // bg job not found.
  if (i >= num_bg_jobs) return;

  printf("[%d]\tDone\t%s\n", i+1, job->cmd->command);
  // Reset the number of jobs since all have completed.
  if (num_bg_jobs == 1) num_bg_jobs = 0;
  free_job(job);
  background_jobs[i] = NULL;
}

bool
has_bg_jobs()
{
  for (int i = 0; i < num_bg_jobs; i++) {
    struct BGJob *job = background_jobs[i];
    if (!job) continue;

    int status;
    pid_t result = waitpid(job->pid, &status, WNOHANG);
    if (result == 0) return true;
  }
  num_bg_jobs = 0;  // reset the number of jobs
  return false;
}

void
print_running_jobs()
{
  for (int i = 0; i < num_bg_jobs; i++) {
    struct BGJob *job = background_jobs[i];
    if (!job) continue;

    int status;
    pid_t result = waitpid(job->pid, &status, WNOHANG);
    if (result == 0) {
      printf("[%d]\tRunning\t%s\n", i+1, job->cmd->command);
    }
  }
}

void
free_job(struct BGJob *job)
{
  free_info(job->info);
  free(job);
}

void
jobs_help()
{
  printf("usage: jobs\n\n"
         "list the processes currently executing in the background.\n");
}

void
bg_help()
{
  printf("usage: bg [job]\n\n"
         "resume suspended jobs without bringing them to the foreground.\n");
}

void
fg_help()
{
  printf("usage: fg [ %%job_id ]\n\n"
         "continues a stopped job by running it in the foreground.\n");
}

void
put_job_in_background(const struct ParseInfo *info, pid_t pid)
{
  struct BGJob *job = job_init(pid, info, &info->CommArray[0]);
  background_jobs[num_bg_jobs] = job;
  num_bg_jobs++;
  printf("[%d] %d\n", num_bg_jobs, pid);
}
