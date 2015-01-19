#ifndef BACKGROUND_JOBS_H
#define BACKGROUND_JOBS_H
#include <stdbool.h>
#include <unistd.h>
#include "parse.h"

#define MAX_BG_JOBS 10

typedef struct {
  pid_t pid;
  parse_info_t *info;
  command_t *cmd;
} bg_job_t;

extern int num_bg_jobs;
extern bg_job_t *background_jobs[MAX_BG_JOBS];

/* Function prototypes. */
void handle_sigchld(int signum);
bool has_bg_jobs();
void print_running_jobs();
void free_job(bg_job_t *job);
void jobs_help();
#endif
