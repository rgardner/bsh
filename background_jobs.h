#include "parse.h"

#define MAX_BG_JOBS 10

typedef struct {
  parse_info_t *info;
  command_t *cmd;
  pid_t pid;
} bg_job_t;

extern int num_bg_jobs;
extern bg_job_t *background_jobs[MAX_BG_JOBS];

/* Function prototypes. */
static void handle_sigchld(int);
void check_bg_jobs();
void free_job(bg_job_t *);
