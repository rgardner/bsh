/* The parsed command information. */
typedef struct {
  BOOL hasInputRedirection;
  BOOL hasOutputRedirection;
  char *command;
  char *arguments;
} parseInfo;

/* Initialize parseInfo struct. */
void init_info(parseInfo *p);

/* Parse a single command. */
parseInfo* parse(char *cmdLine);

/* Prints out parse struct. */
void print_info(parseInfo *info);

/* Free memory used in parseInfo. */
void free_info(parseInfo *info);
