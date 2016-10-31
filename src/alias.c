#include "alias.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <bsd/string.h>
#else
#include <string.h>
#endif

#include "linked_list.h"

/** Alias mapping name to value. */
typedef struct
{
  const char* name;
  char* value;
} Alias;

// Global variables
struct LinkedList* aliases;

// Function prototypes
static Alias* alias_init(const char* const name, const char* const value);
static void alias_free(Alias* a);
static bool alias_add(const char* const name,
                      const char* const value,
                      bool overwrite);
static bool alias_remove(const char* name);
static void alias_insert_sorted(Alias* a);
static Alias* alias_search(const char* name);
static void aliases_print();

void
aliases_init()
{
  aliases = ll_init();
}

/** Initialize a new alias.
 *
 *  @param name This is copied.
 *  @param value This is copied.
 *  @return Heap allocated alias.
 */
static Alias*
alias_init(const char* const name, const char* const value)
{
  const char* const name_h = strdup(name);
  char* value_h = strdup(value);
  Alias al = {.name = name_h, .value = value_h };
  Alias* alias = malloc(sizeof(Alias));
  memcpy(alias, &al, sizeof(Alias));
  return alias;
}

int
alias(const int argc, char** argv)
{
  // Print all aliases.
  if (argc == 1) {
    aliases_print();
    return 0;
  }

  // Create an alias between a name/value pair.
  // copy arg1 so that argv is not mutated
  char *arg1, *tofree;
  tofree = arg1 = strdup(argv[1]);
  char* name = strsep(&arg1, "=");
  if (arg1) {
    alias_add(name, arg1, true);
    free(tofree);
    return 0;
  }

  free(tofree);

  // Print all aliases matching name arguments.
  bool all_found = true;
  for (int i = 1; i < argc; i++) {
    Alias* al = alias_search(argv[i]);
    if (al) {
      printf("alias %s=%s\n", al->name, al->value);
    } else {
      printf("bsh: alias: %s: not found\n", argv[i]);
      all_found = false;
    }
  }
  return all_found ? 0 : 1;
}

void
alias_help()
{
  printf(
    "usage: alias [name[=value] ... ]\n"
    "  alias: print all existing aliases.\n"
    "  alias name: print the value associated with name.\n"
    "  alias name=value: create / modify name to be an alias for value.\n");
}

int
unalias(const int argc, char** argv)
{
  if (argc == 1) {
    unalias_help();
    return 2;
  }

  // remove all aliases
  if (strcmp(argv[1], "-a") == 0) {
    for (Node* curr = aliases->head; curr; curr = curr->next) {
      Alias* al = curr->data;
      bool success = alias_remove(al->name);
      assert(success);
    }

    return 0;
  }

  bool all_found = true;
  for (int i = 1; i < argc; i++) {
    if (!alias_remove(argv[i])) {
      fprintf(stderr, "-bsh: unalias: %s: not found\n", argv[i]);
      all_found = false;
    }
  }

  return all_found ? 0 : 1;
}

void
unalias_help()
{
  printf(
    "usage: unalias [-a] name [name ...]\n"
    "  Remove NAMEs from the list of defined aliases. If the -a option is\n"
    "  given, then remove all alias definitions.\n");
}

int
alias_exp(const char* string, char** output)
{
  *output = NULL;

  // Return if no alias exists for string.
  Alias* result = alias_search(string);
  if (!result) {
    return 0;
  }

  // Copy string into output
  *output = strdup(result->value);
  return 1;
}

/** Add name->value mapping.
 *
 *  @param name Name of alias. This is copied.
 *  @param value The command to run when the alias is entered. This is copied.
 *  @param overwrite Should the value be overwritten if an alias already exists.
 *  @return
 *  true if an alias was inserted/updated;
 *  false if overwrite was true and the alias already existed.
 */
static bool
alias_add(const char* const name, const char* const value, bool overwrite)
{
  // Search to see if alias already exists.
  for (Node* curr = aliases->head; curr; curr = curr->next) {
    Alias* alias = curr->data;
    if (strcmp(name, alias->name) != 0) {
      continue;
    }

    if (!overwrite) {
      return false;
    }

    char* value_h = strdup(value);
    free(alias->value);
    alias->value = value_h;
    return true;
  }

  Alias* new = alias_init(name, value);
  alias_insert_sorted(new);
  return true;
}

static void
alias_insert_sorted(Alias* a)
{
  Node* prev = NULL;
  for (Node* curr = aliases->head; curr; curr = curr->next) {
    Alias* curr_alias = curr->data;
    if (strcmp(a->name, curr_alias->name) < 0) {
      break;
    }
    prev = curr;
  }

  ll_add_after(aliases, prev, a);
}

/** Remove the alias corresponding to the name.
 *
 *  @return true if found; false otherwise.
 */
static bool
alias_remove(const char* name)
{
  Node* prev = NULL;
  for (Node* curr = aliases->head; curr; curr = curr->next) {
    Alias* alias = curr->data;
    int cmp = strncmp(name, alias->name, strlen(name));
    if (cmp > 0) {
      break;
    }

    if (cmp < 0) {
      prev = curr;
      continue;
    }

    // Found the node
    if (prev) {
      prev->next = curr->next;
    } else {  // first element in the list
      aliases->head = curr->next;
    }
    alias_free(alias);
    node_free(curr);
    return true;
  }
  return false;
}

/** Find alias by its name.
 *
 * Do not deallocate pointer.
 * @see alias_remove
 * @return Pointer to alias if found; NULL if not found.
 */
static Alias*
alias_search(const char* name)
{
  for (Node* curr = aliases->head; curr; curr = curr->next) {
    Alias* al = curr->data;
    if (!al) {
      // TODO: should this be continue?
      break;
    }

    if (strncmp(al->name, name, strlen(al->name)) == 0) {
      return al;
    }
  }
  return NULL;
}

/** Print all aliases. */
static void
aliases_print()
{
  for (Node* curr = aliases->head; curr; curr = curr->next) {
    Alias* al = curr->data;
    printf("alias %s=%s\n", al->name, al->value);
  }
}

static void
alias_free(Alias* alias)
{
  assert(alias->name);
  assert(alias->value);

  free((char*)alias->name);
  free(alias->value);
  free(alias);
}
