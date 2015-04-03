#ifndef PARSE_H
#define PARSE_H
#include <stdbool.h>

#define MAXLINE 81
#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_SIZE 40

struct Command {
  char *command;
  char *VarList[MAX_VAR_NUM];
  int VarNum;
};

/* The parsed command information. */
struct ParseInfo {
  bool hasInputRedirection;   // infile specified?
  bool hasOutputRedirection;  // outfile specified?
  bool runInBackground;       // run in background?
  struct Command CommArray[PIPE_MAX_NUM];
  int pipeNum;
  char inFile[FILE_MAX_SIZE];   // file to be piped from
  char outFile[FILE_MAX_SIZE];  // file to pipe into
};

/* Function prototypes. */
struct ParseInfo *parse(const char *);

void free_info(const struct ParseInfo *);

void print_info(const struct ParseInfo *);
#endif
