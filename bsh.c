#include <stdio.h>
#include <stdlib.h>

void print_arguments(int, char**);


int main(int argc, char **argv) {
  printf("Welcome to Bob shell.\n");
  print_arguments(argc, argv);
  exit(EXIT_SUCCESS);
}

/* print arguments. */
void print_arguments(int argc, char **argv) {
  while (argc--) {
    printf("%s\n", *argv++);
  }
}
