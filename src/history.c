#include "history.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Function prototypes. */
histdata_t free_hist_entry(HIST_ENTRY *);

/* Global variables. */
/* Public global variables. */
int history_length;
int history_max_entries;

/* Private global variables. */
HISTORY_STATE state;

void history_init() {
  state.count = -1;
  state.length = 0;
  history_length = state.length;
  state.size = HISTSIZE;
  history_max_entries = state.size;
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
  int command;
  int ret = sscanf(string + i, "!%d", &command);
  if (ret != 1) return -1;

  // Ensure command is within the bounds of the size of the history.
  if (command < 0) {
    if (command <= -HISTSIZE) return -1;
    command = state.count + command + 1;
  } else {
    if (command < (state.count - HISTSIZE) || command > state.count) return -1;
  }

  char *line = state.entries[command]->line;
  length = strlen(line) + 1;
  *output = realloc(*output, sizeof(char) * length);
  strncpy(*output, line, length);
  return 1;
}


void history_add(char *string) {
  int length;        // length of string
  int pre;           // previous element in history
  int timestamp;     // timestamp to save
  char *dup_string;  // copy of string

  // Copy string.
  length = strlen(string) + 1;
  dup_string = strndup(string, sizeof(char) * length);

  state.count++;
  HIST_ENTRY *entry = state.entries[state.count % state.size];
  if (entry) {
    histdata_t data = free_hist_entry(entry);
    if (data) free(data);
  } else {
    state.length++;
    history_length++;
  }
  entry = malloc(sizeof(HIST_ENTRY));

  // Calculate timestamp.
  pre = (state.count-1) % state.size;
  if (pre < 0) pre = state.size;
  timestamp = (state.entries[pre]) ? state.entries[pre]->timestamp : 0;

  entry->line = dup_string;
  entry->timestamp = timestamp + 1;
  entry->data = NULL;
  state.entries[state.count % state.size] = entry;
}

void history_stifle(int max) {
  if (max < 0) return;  // can't record a negative number of commands
  if (state.size == max) return;  // nothing to change

  if (state.length > max) {
    int start = state.count - max + 1;
    HIST_ENTRY **list = malloc(sizeof(HIST_ENTRY) * max);
    for (int i = 0; i < max; i++) {
      HIST_ENTRY *entry = state.entries[(start + i) % state.size];
      list[i] = entry;
    }

    free(state.entries);
    state.entries = list;
    state.length = max;
    state.count = max - 1;
  } else if (state.length < max) {
    state.entries = realloc(state.entries, sizeof(HIST_ENTRY)*max);
  }
  state.size = max;
  history_max_entries = state.size;
}

histdata_t free_hist_entry(HIST_ENTRY *histent) {
  if (histent->line) free(histent->line);
  histdata_t data = histent->data;
  free(histent);
  return data;
}

void history_print(int num) {
  if (num > state.length) num = state.length;
  int start = state.count % state.size + 1;
  if (start >= state.length) start = 0;

  for (int i = start; (i < state.length) && (i < num); i++) {
    HIST_ENTRY *entry = state.entries[i];
    if (!entry) continue;
    printf("\t%d\t%s\n", entry->timestamp, entry->line);
  }
  for (int i = 0; (i < start) && (i < num); i++) {
    HIST_ENTRY *entry = state.entries[i];
    if (!entry) continue;
    printf("\t%d\t%s\n", entry->timestamp, entry->line);
  }
}

void history_help() {
  printf("usage: history [num] [-s num]\n\n"
         "history: print the list of previously executed commands.\n"
         "history num: return the last `num` commands.\n"
         "history -s num: set the size of the history buffer.\n"
         "!1 repeats the command numbered `1` in the list of commands "
         "return by history.\n"
         "!-1 repeats the last command.\n");
}