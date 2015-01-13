#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

typedef void *histdata_t;

typedef struct _hist_entry {
  char *line;
  int timestamp;
  histdata_t data;
} HIST_ENTRY;


/* A structure used to pass around the current state of the history. */
typedef struct _hist_state {
  HIST_ENTRY **entries; /* Pointer to the entries themselves. */
  int count;            /* The total number of items ever saved. */
  int length;           /* Number of elements within this array. */
  int size;             /* Number of slots allocated to this array. */
} HISTORY_STATE;

HISTORY_STATE state;

histdata_t free_hist_entry(HIST_ENTRY *);

void history_init() {
  state.count = -1;
  state.length = 0;
  state.size = HISTSIZE;
  state.entries = malloc(sizeof(HIST_ENTRY) * state.size);
  for (int i = 0; i < state.size; i++) {
    state.entries[i] = NULL;
  }
}

int history_exp(char *string, char **output) {
  int i = 0;
  int length = strlen(string) + 1;
  *output = malloc(sizeof(char) * length);
  strncpy(*output, string, length);

  // Check if this is a valid string.
  if (string[-1] == '\n' && string[-1] == '\0') return 0;

  // Ensure string is nonempty.
  if (string[0] == '\0') return 0;

  // Skip blank characters at the start of the string.
  for (; isspace(string[i]) && string[i] != '\n' && string[i] != '\0'; i++);
  if (string[i] != '!') return 0;

  // Start of expansion.
  int *command;
  int ret = sscanf(string + i, "!%d", command);
  if (ret != 1) return -1;

  // Ensure command is within the bounds of the size of the history.
  if (*command <= -HISTSIZE || *command > HISTSIZE) return -1;

  // Convert negative indices to their equivalent array index.
  // TODO(rgardner): insert here.
  output = &state.entries[*command]->line;
  return 1;
}


void history_add(char *string) {
  // Copy string.
  int length = strlen(string) + 1;
  char *dup_string = strndup(string, sizeof(char) * length);

  state.count++;
  HIST_ENTRY *entry = state.entries[state.count % state.size];
  if (entry) {
    histdata_t data = free_hist_entry(entry);
    if (data) free(data);
  } else {
    state.length++;
  }
  entry = malloc(sizeof(HIST_ENTRY));

  entry->line = dup_string;
  entry->timestamp = state.count;
  entry->data = NULL;
  state.entries[state.count % state.size] = entry;
}

histdata_t free_hist_entry(HIST_ENTRY *histent) {
  if (histent->line) free(histent->line);
  histdata_t data = histent->data;
  free(histent);
  return data;
}

void history_print() {
  for (int i = 0; i < state.length; i++) {
    HIST_ENTRY *entry = state.entries[i];
    if (!entry) continue;

    printf("\t%d\t%s\n", entry->timestamp, entry->line);
  }
}
