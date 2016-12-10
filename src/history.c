#include "history.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circular_queue.h"
#include "util.h"

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
static void history_free_entry_and_data(void*);
static histdata_t history_free_entry(HIST_ENTRY*);
static int parse_command(const char*, int*);

// Public global variables
int history_length;

// Private global variables
HISTORY_STATE state;

void
history_init()
{
  state.queue = circular_queue_init(HISTSIZE);
  history_length = 0;
}

void
history_free()
{
  for (size_t i = 0; i < state.queue->capacity; i++) {
    HIST_ENTRY* entry = state.queue->entries[i];
    if (entry) {
      history_free_entry_and_data(entry);
    }
  }

  circular_queue_free(state.queue);
}

static void
history_free_entry_and_data(void* elem)
{
  histdata_t* data = history_free_entry((HIST_ENTRY*)elem);
  if (data) {
    free(data);
  }
}

static histdata_t
history_free_entry(HIST_ENTRY* histent)
{
  if (histent->line) {
    free(histent->line);
  }

  histdata_t data = histent->data;
  free(histent);
  return data;
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
  HIST_ENTRY* entry = malloc(sizeof(HIST_ENTRY));
  if (!entry) {
    return;
  }

  entry->line = strdup(string);
  if (!(entry->line)) {
    return;
  }

  entry->timestamp = state.queue->count + 1;
  entry->data = NULL;
  HIST_ENTRY* removed = circular_queue_push(state.queue, entry);
  if (removed) {
    history_free_entry_and_data(removed);
  }

  history_length++;
}

void
history_stifle(const int max)
{
  circular_queue_set_capacity(state.queue, max, history_free_entry_and_data);
}

void
history_print(const size_t n_last_entries)
{
  const size_t num_stored_entries = min(state.queue->count, state.queue->capacity);
  size_t start_pos = state.queue->count;
  if (n_last_entries < num_stored_entries) {
    start_pos -= n_last_entries;
  } else {
    start_pos -= num_stored_entries;
  }

  for (size_t pos = start_pos; pos < state.queue->count; pos++) {
    HIST_ENTRY* entry = circular_queue_get(state.queue, pos);
    assert(entry);
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
