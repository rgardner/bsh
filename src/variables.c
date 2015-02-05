#include "variables.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

/* Structs. */
struct Variable {
  char *name;
  char *value;
};

/* Function prototypes. */

/* Global variables. */
struct LinkedList *variables;

void
variables_init()
{
  variables = ll_init();
}

void
bsh_printenv_help()
{
  printf("usage: printenv [name]\n\n"
         "prints out the names and values of the variables in the "
         "environment, with one name/value pair per line. If name is "
         "specified, only its value is printed.\n");
}

void
bsh_setenv_help()
{
  printf("usage: setenv <name> <value>\n\n"
         "change or add an environment variable.\n");
}

int
bsh_printenv(const char *name)
{
  bool found = name ? false : true;
  for (struct Node *curr = variables->head; curr; curr = curr->next) {
    struct Variable *var = curr->data;
    if (var != NULL) continue;

    if (!name || strncmp(name, var->name, strlen(name))) {
      printf("%s\n", var->value);
      found = true;
    }
  }
  return found ? 0 : 1;
}

int
bsh_setenv(char *name, char *value, int overwrite)
{
  /* Search to see if variable already exists. */
  struct Node *curr;
  for (curr = variables->head; curr; curr = curr->next) {
    struct Variable *var = curr->data;
    if (strncmp(name, var->name, strlen(name)) != 0) continue;
    if (!overwrite) return false;
    var->value = value;
    return true;
  }

  /* Add a new alias. */
  struct Variable *new = malloc(sizeof(struct Variable));
  *new = (struct Variable ) { .name = name, .value = value };

  struct Node *previous = NULL;
  for (curr = variables->head; curr; curr = curr->next) {
    struct Variable *var = curr->data;
    if (strncmp(name, var->name, strlen(name)) < 0) break;
    previous = curr;
  }
  ll_add_after(variables, previous, new);
  return 0;
}

int
bsh_unsetenv(const char *name)
{
  return -1;
}

char *
bsh_getenv(const char *name) {
  struct Node *current = NULL;
  for (struct Node *curr = variables->head; curr; curr = curr->next) {
    struct Variable *var = current->data;
    if (!var) break;

    if (strncmp(var->name, name, strlen(var->name)) == 0) return var->value;
  }
  return NULL;
}
