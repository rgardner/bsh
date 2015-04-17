#ifndef JOB_H
#define JOB_H

#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include "parse.h"

/* A process is a single process.  */
typedef struct process {
  struct process *next;       /* next process in pipeline */
  char **argv;                /* for exec */
  int argc;
  pid_t pid;                  /* process ID */
  bool completed;             /* true if process has completed */
  bool stopped;               /* true if process has stopped */
  int status;                 /* reported status value */
} process;

/* A job is a pipeline of processes.  */
typedef struct job {
  struct job *next;           /* next active job */
  char *command;              /* command line, used for messages */
  process *first_process;     /* list of processes in this job */
  pid_t pgid;                 /* process group ID */
  bool notified;              /* true if user told about stopped job */
  struct termios tmodes;      /* saved terminal modes */
  int infile, outfile, errfile;  /* standard i/o channels */
} job;

void
process_free(process *);

void
init_job(job *, const struct ParseInfo *, pid_t, struct termios);

void
job_free(job *);

void
job_print(job *);
#endif
