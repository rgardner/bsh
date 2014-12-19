#include <stdio.h>
#include <stdlib.h>

#include <parse.h>
#include <types.h>

void print_prompt();

int main(int argc, char** argv) {
  parseInfo *info;
  while (TRUE) {
    char cmdLine[100];
    print_prompt();
    fgets(cmdLine, sizeof(cmdLine), stdin);  // read from stdin
    info = parse(cmdLine);
    print_info(info);
  }
  free_info(info);
}


void print_prompt()
{
  printf("shell: ");
}
