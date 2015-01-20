#include "background_jobs.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

int num_bg_jobs;
bg_job_t *background_jobs[MAX_BG_JOBS];

void handle_sigchld(int signum) {
  pid_t pid = waitpid((pid_t)-1, 0, WNOHANG);

  // find the bg job that exited.
  bg_job_t *job;
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

bool has_bg_jobs() {
  for (int i = 0; i < num_bg_jobs; i++) {
    bg_job_t *job = background_jobs[i];
    if (!job) continue;

    int status;
    pid_t result = waitpid(job->pid, &status, WNOHANG);
    if (result == 0) return true;
  }
  num_bg_jobs = 0;  // reset the number of jobs
  return false;
}

void print_running_jobs() {
  for (int i = 0; i < num_bg_jobs; i++) {
    bg_job_t *job = background_jobs[i];
    if (!job) continue;

    int status;
    pid_t result = waitpid(job->pid, &status, WNOHANG);
    if (result == 0) {
      printf("[%d]\tRunning\t%s\n", i+1, job->cmd->command);
    }
  }
}

void free_job(bg_job_t *job) {
  free_info(job->info);
  free(job);
}

void jobs_help() {
  printf("usage: jobs\n\n"
         "list the processes currently executing in the background.\n");
}

void bg_help() {
  printf("usage: bg [job]\n\n"
         "resume suspended jobs without bringing them to the foreground.\n");
}

void fg_help() {
  printf("usage: fg [ %%job_id ]\n\n"
         "continues a stopped job by running it in the foreground.\n");
}
