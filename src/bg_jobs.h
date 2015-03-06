#ifndef BACKGROUND_JOBS_H
#define BACKGROUND_JOBS_H
#include <stdbool.h>
#include <unistd.h>
#include "parse.h"

#define MAX_BG_JOBS 10

/* Function prototypes. */
void
jobs_init();

void
handle_sigchld(const int signum);

bool
has_bg_jobs();

void
print_running_jobs();

void
put_job_in_background(const struct ParseInfo *, pid_t);

void
jobs_help();

void
bg_help();

void
fg_help();
#endif
