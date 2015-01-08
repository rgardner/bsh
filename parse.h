#include <stdbool.h>

#define MAXLINE 81
#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_SIZE 40

typedef struct {
  char *command;
  char *VarList[MAX_VAR_NUM];
  int VarNum;
} command_t;

/* The parsed command information. */
typedef struct {
  bool hasInputRedirection;   // infile specified?
  bool hasOutputRedirection;  // outfile specified?
  bool runInBackground;       // run in background?
  command_t CommArray[PIPE_MAX_NUM];
  int pipeNum;
  char inFile[FILE_MAX_SIZE];   // file to be piped from
  char outFile[FILE_MAX_SIZE];  // file to pipe into
} parse_info_t;

/* Function prototypes. */
parse_info_t *parse(char *);
void free_info(parse_info_t *);
void print_info(parse_info_t *);
bool is_bg_job(parse_info_t *);
