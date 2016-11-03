#include "history.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circular_queue.h"

typedef void* histdata_t;

typedef struct _hist_entry
{
  char* line;
  int timestamp;
  histdata_t data;
} HIST_ENTRY;

/** A structure used to pass around the current state of the history. */
typedef struct _hist_state
{
  circular_queue* queue;
} HISTORY_STATE;

// Function prototypes
histdata_t free_hist_entry(HIST_ENTRY*);

// Public global variables
int history_length;
int history_max_entries;

// Private global variables
HISTORY_STATE state;

void
history_init()
{
  state.queue = circular_queue_init(HISTSIZE);
  history_length = 0;
  history_max_entries = HISTSIZE;
}

void
history_free()
{
  for (size_t i = 0; i < state.queue->capacity; i++) {
    HIST_ENTRY* e = state.queue->entries[i];
    if (e) {
      void* data = free_hist_entry(e);
      if (data) {
        free(data);
      }
    }
  }

  circular_queue_free(state.queue);
}

/** Parse history event from history command.
 *
 *  @return 1 if valid history command; 0 if not a history command; -1 if an
 *          error occurred.
 */
static int
parse_command(const char* string, int* output)
{
  *output = 0;

  if (string[0] == '\0') {
    return 0;
  }

  // Skip blank characters at the start of the string
  int i = 0;
  for (; isspace(string[i]) && string[i] != '\n' && string[i] != '\0'; i++)
    ;

  if (string[i] != '!') {
    return 0;
  }

  int command;
  if (sscanf(string + i, "!%d", &command) != 1) {
    return -1;
  }

  *output = command;
  return 1;
}

int
history_exp(const char* string, char** output)
{
  *output = NULL;

  int command;
  const int result = parse_command(string, &command);
  if (result != 1) {
    return result;
  }

  size_t pos = 0;
  if (command == 0) {
    fprintf(stderr, "-bsh: %s: event not found\n", string);
    return -1;
  } else if (command < 0) {
    const size_t pos_command = -command;
    if (pos_command > state.queue->count) {
      fprintf(stderr, "-bsh: %s: event not found\n", string);
      return -1;
    }

    pos = state.queue->count - pos_command;
  } else {
    // !1 is the 0th history entry
    pos = command - 1;
  }

  HIST_ENTRY* entry = circular_queue_get(state.queue, pos);
  if (!entry) {
    fprintf(stderr, "-bsh: %s: event not found\n", string);
    return -1;
  }

  if (!(*output = strdup(entry->line))) {
    return -1;
  }

  return 1;
}

void
history_add(const char* string)
{
  if (state.queue->count < state.queue->capacity) {
    history_length++;
  }

  HIST_ENTRY* entry = malloc(sizeof(HIST_ENTRY));
  entry->line = strdup(string);
  entry->timestamp = state.queue->count + 1;
  entry->data = NULL;
  HIST_ENTRY* removed = circular_queue_push(state.queue, entry);
  if (removed) {
    void* data = free_hist_entry(removed);
    if (data) {
      free(data);
    }
  }
}

void
history_stifle(const int max)
{
  circular_queue_set_capacity(state.queue, max);
  // use below as reference for circular_queue_set_capacity
  // if (max < 0) {
  // // can't record a negative number of commands
  // return;
  // }

  // if (state.size == max) {
  // return;
  // }

  // if (state.length > max) {
  // int start = state.count - max + 1;
  // HIST_ENTRY** list = malloc(sizeof(HIST_ENTRY) * max);
  // for (int i = 0; i < max; i++) {
  // HIST_ENTRY* entry = state.entries[(start + i) % state.size];
  // list[i] = entry;
  // }

  // free(state.entries);
  // state.entries = list;
  // state.length = max;
  // state.count = max - 1;
  // } else if (state.length < max) {
  // state.entries = realloc(state.entries, sizeof(HIST_ENTRY) * max);
  // }

  // state.size = max;
  // history_max_entries = state.size;
}

histdata_t
free_hist_entry(HIST_ENTRY* histent)
{
  if (histent->line) {
    free(histent->line);
  }

  histdata_t data = histent->data;
  free(histent);
  return data;
}

void
history_print(const int num)
{
  const int start = state.queue->count % state.queue->capacity;
  for (int i = start; i < num; i++) {
    HIST_ENTRY* entry = state.queue->entries[i];
    if (!entry) {
      return;
    }

    printf("\t%d\t%s\n", entry->timestamp, entry->line);
  }

  for (int i = 0; (i < start) && (i < num); i++) {
    HIST_ENTRY* entry = state.queue->entries[i];
    if (!entry) {
      continue;
    }
    printf("\t%d\t%s\n", entry->timestamp, entry->line);
  }
}

void
history_help()
{
  printf(
    "usage: history [-c] [n] [-s n]\n"
    "\n"
    "  Display or manipulate the history list.\n"
    "\n"
    "  Display the history list. An argument of N lists only the last N "
    "entries.\n"
    "\n"
    "  Options:\n"
    "    -s       set the size of the history buffer\n"
    "\n"
    "HISTORY SUBSTITUTION\n"
    "  !1 repeats the command numbered `1' in the list of commands returned "
    "by\n"
    "  history.\n"
    "  !-1 repeats the last command.\n");
}
