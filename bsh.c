#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  printf("Welcome to Bob shell.\n");
  while (argc--) {
    printf("%s\n", *argv++);
  }
  exit(EXIT_SUCCESS);
}
