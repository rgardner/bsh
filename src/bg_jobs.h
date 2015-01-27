#ifndef BACKGROUND_JOBS_H
#define BACKGROUND_JOBS_H
#include <stdbool.h>
#include <unistd.h>
#include "parse.h"

#define MAX_BG_JOBS 10

struct BGJob {
  pid_t pid;
  struct ParseInfo *info;
  struct Command *cmd;
};

extern int num_bg_jobs;
extern struct BGJob *background_jobs[MAX_BG_JOBS];

/* Function prototypes. */
void handle_sigchld(int signum);
bool has_bg_jobs();
void print_running_jobs();
void free_job(struct BGJob *job);
void jobs_help();
void bg_help();
void fg_help();
#endif
