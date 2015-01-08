#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "background_jobs.h"

int num_bg_jobs;
bg_job_t *background_jobs[MAX_BG_JOBS];

static void handle_sigchld(int signum) {
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
  free_job(job);
  background_jobs[i] = NULL;
}

void check_bg_jobs() {
  bool remaining_bg_jobs = false;
  for (int i = 0; i < num_bg_jobs; i++) {
    bg_job_t *job = background_jobs[i];
    if (job == NULL) continue;
    int status;

    pid_t result = waitpid(job->pid, &status, WNOHANG);
    if (result == 0) {
      printf("[%d]\tRunning\t%s\n", i+1, job->cmd->command);
      remaining_bg_jobs = true;
      continue;
    }

    if (result == -1) {
      fprintf(stderr, "Job %d (%s) encountered an error.\n", i+1, job->cmd->command);
    } else {
      printf("[%d]\tDone\t%s\n", i+1, job->cmd->command);
    }
    free_job(job);
    background_jobs[i] = NULL;
  }
  if (!remaining_bg_jobs) num_bg_jobs = 0;
}

void free_job(bg_job_t *job) {
  free_info(job->info);
  free(job);
}
