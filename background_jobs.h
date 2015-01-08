#include "parse.h"

#define MAX_BG_JOBS 10

typedef struct {
  parseInfo *info;
  commandType *cmd;
  pid_t pid;
} BackgroundJob;

extern int num_bg_jobs;
extern BackgroundJob *background_jobs[MAX_BG_JOBS];

/* Function prototypes. */
static void handle_sigchld(int);
void check_bg_jobs();
void free_job(BackgroundJob *);
