#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "background_jobs.h"

int num_bg_jobs;
bg_job_t *background_jobs[MAX_BG_JOBS];

void handle_sigchld(int signum) {
  pid_t pid = waitpid((pid_t)-1, 0, WNOHANG);

  // find the bg job that exited.
  bg_job_t *job;
  int i;
  for (i = 0; i < num_bg_jobs; i++) {
    job = background_jobs[i];
    if (job != NULL && job->pid == pid) break;
  }

  // bg job not found.
  if (i >= num_bg_jobs) return;

  printf("[%d]\tDone\t%s\n", i+1, job->cmd->command);
  // Reset the number of jobs since all have completed.
  if (num_bg_jobs == 1) num_bg_jobs = 0;
  free_job(job);
  background_jobs[i] = NULL;
}

bool has_bg_jobs(bool print_running) {
  bool has_bg_jobs = false;
  for (int i = 0; i < num_bg_jobs; i++) {
    bg_job_t *job = background_jobs[i];
    if (job == NULL) continue;

    int status;
    pid_t result = waitpid(job->pid, &status, WNOHANG);
    if (result == 0) {
      if (print_running) printf("[%d]\tRunning\t%s\n", i+1, job->cmd->command);
      has_bg_jobs = true;
      continue;
    }
  }
  // Reset num_bg_jobs if no running jobs found.
  if (!has_bg_jobs) num_bg_jobs = 0;
  return has_bg_jobs;
}

void free_job(bg_job_t *job) {
  free_info(job->info);
  free(job);
}
