#include "alias.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_List.h"

/* Structs. */
struct Alias {
  char *name;
  char *value;
};

/* Function prototypes. */
static bool alias_add(char *, char *, bool);
static struct Alias* alias_search(char *);

/* Global variables. */
struct LinkedList *aliases;

int
alias(int argc, char **argv)
{
  /* Print all aliases. */
  if (argc == 0) {
    int len = ll_size(aliases);
    for (int i = 0; i < len; i++) {
      struct Alias *al = ll_get(aliases, i);
      printf("alias %s = %s\n", al->name, al->value);
    }
    return 0;
  }

  /* Create an alias between a name/value pair. */
  char *value;
  if ((value = strsep(&argv[0], "="))) {
    char *name = strdup(argv[0]);
    alias_add(name, value, 1);
    return 0;
  }

  /* Print all aliases matching name arguments. */
  bool found = true;
  for (int i = 0; i < argc; i++) {
    struct Alias *al = alias_search(argv[i]);
    if (al) {
      printf("alias %s = %s\n", al->name, al->value);
    } else {
      printf("bsh: alias: %s not found.\n", argv[i]);
      found = false;
    }
  }
  return found ? 0 : 1;
}

void alias_help()
{
  printf("usage: alias [name[=value] ... ]\n\n"
         "`alias`: print all existing aliases.\n"
         "`alias name`: print the value associated with name.\n"
         "`alias name=value`: create / modify name to be an alias for value.\n");
}

static bool
alias_add(char *name, char *value, bool overwrite)
{
  int len = ll_size(aliases);

  /* Search to see if alias already exists. */
  for (int i = 0; i < len; i++) {
    struct Alias *al = ll_get(aliases, i);
    if (!al) break;

    if (strncmp(al->name, name, strlen(al->name)) != 0) continue;
    if (!overwrite) return false;
    al->value = value;
    return true;
  }
  struct Alias *new = malloc(sizeof(struct Alias));
  new->name = name;
  new->value = value;
  int i;
  for (i = 0; i < len; i++) {
    struct Alias *al = ll_get(aliases, i);
    if (strncmp(name, al->name, strlen(name)) < 0) break;
  }
  ll_add(aliases, i, new);
  return true;
}

static struct Alias *
alias_search(char *name)
{
  int len = ll_size(aliases);
  for (int i = 0; i < len; i++) {
    struct Alias *al = ll_get(aliases, i);
    if (!al) break;

    if (strncmp(al->name, name, strlen(al->name)) == 0) return al;
  }
  return NULL;
}
