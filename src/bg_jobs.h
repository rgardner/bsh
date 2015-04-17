#ifndef BACKGROUND_JOBS_H
#define BACKGROUND_JOBS_H
#include <stdbool.h>

#define MAX_BG_JOBS 10

/* Function prototypes. */
void jobs_init();

void handle_sigchld(int signum);

bool has_bg_jobs();

void print_running_jobs();

void jobs_help();

void bg_help();

void fg_help();
#endif
