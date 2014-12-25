#include "types.h"

#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_SIZE 40

typedef struct {
  char *command;
  char *VarList[MAX_VAR_NUM];
  int VarNum;
} commandType;

/* The parsed command information. */
typedef struct {
  BOOL hasInputRedirection;   // infile specified
  BOOL hasOutputRedirection;  // outfile specified
  BOOL runInBackground;       // run in background
  commandType CommArray[PIPE_MAX_NUM];
  int pipeNum;
  char inFile[FILE_MAX_SIZE];   // file to be piped from
  char outFile[FILE_MAX_SIZE];  // file to pipe into
} parseInfo;

/* Function prototypes. */
parseInfo *parse(char *);
void free_info(parseInfo *);
void print_info(parseInfo *);
