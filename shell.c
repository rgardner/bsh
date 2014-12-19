#include <stdio.h>
#include <stdlib.h>

#include <parse.h>
#include <types.h>

void print_prompt();
char *read_line();

int main(int argc, char** argv)
{
  parseInfo *info;
  while (TRUE) {
    char* cmdLine = NULL;
    print_prompt();
    cmdLine = read_line();
    info = parse(cmdLine);
    print_info(info);
  }
  free_info(info);
}


void print_prompt()
{
  printf("shell: ");
}

char* read_line()
{
  char *cmdLine = malloc(100 * sizeof(char));
  fgets(cmdLine, sizeof(cmdLine), stdin);  // read from stdin
  return cmdLine;
}
