#ifndef BACKGROUND_JOBS_H
#define BACKGROUND_JOBS_H
#include <stdbool.h>
#include <unistd.h>
#include "parse.h"

#define MAX_BG_JOBS 10

struct BGJob {
  const pid_t pid;
  const struct ParseInfo *info;
  const struct Command *cmd;
};

extern int num_bg_jobs;
extern struct BGJob *background_jobs[MAX_BG_JOBS];

/* Function prototypes. */
struct BGJob *
job_init(const pid_t, const struct ParseInfo *, const struct Command *);

void
free_job(struct BGJob *job);

void
handle_sigchld(const int signum);

bool
has_bg_jobs();

void
print_running_jobs();

void
jobs_help();

void
bg_help();

void
fg_help();
#endif
