#include "history.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circular_queue.h"
#include "util.h"

typedef void* histdata_t;

struct hist_entry
{
  char* line;
  int timestamp;
  histdata_t data;
};

/** A structure used to pass around the current global state of the history. */
struct hist_state
{
  circular_queue* queue;
};

// Function prototypes
static void history_free_entry_and_data(void*);
static histdata_t history_free_entry(struct hist_entry*);
static int parse_command(const char*, int*);

// Private global variables
struct hist_state g_state;

void
history_init()
{
  g_state.queue = circular_queue_init(HISTSIZE);
}

void
history_free()
{
  for (size_t i = 0; i < g_state.queue->capacity; i++) {
    struct hist_entry* entry = g_state.queue->entries[i];
    if (entry) {
      history_free_entry_and_data(entry);
    }
  }

  circular_queue_free(g_state.queue);
}

static void
history_free_entry_and_data(void* elem)
{
  histdata_t* data = history_free_entry((struct hist_entry*)elem);
  if (data) {
    free(data);
  }
}

static histdata_t
history_free_entry(struct hist_entry* histent)
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
    if (pos_command > g_state.queue->count) {
      fprintf(stderr, "-bsh: %s: event not found\n", string);
      return -1;
    }

    pos = g_state.queue->count - pos_command;
  } else {
    // !1 is the 0th history entry
    pos = command - 1;
  }

  struct hist_entry* entry = circular_queue_get(g_state.queue, pos);
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
  struct hist_entry* entry = malloc(sizeof(struct hist_entry));
  if (!entry) {
    return;
  }

  entry->line = strdup(string);
  if (!(entry->line)) {
    return;
  }

  entry->timestamp = g_state.queue->count + 1;
  entry->data = NULL;
  struct hist_entry* removed = circular_queue_push(g_state.queue, entry);
  if (removed) {
    history_free_entry_and_data(removed);
  }
}

void
history_stifle(const int max)
{
  circular_queue_set_capacity(g_state.queue, max, history_free_entry_and_data);
}

void
history_print_all()
{
  history_print(g_state.queue->count);
}

void
history_print(const size_t n_last_entries)
{
  // because the capacity can change, there may be fewer than max_stored_entries
  // in the queue (e.g. [a] -> [b] -> [_ b _]. The count is 2 but there is only
  // 1 stored entry)
  const size_t max_stored_entries = min(g_state.queue->count, g_state.queue->capacity);
  size_t start_pos = g_state.queue->count;
  if (n_last_entries < max_stored_entries) {
    start_pos -= n_last_entries;
  } else {
    start_pos -= max_stored_entries;
  }

  for (size_t pos = start_pos; pos < g_state.queue->count; pos++) {
    struct hist_entry* entry = circular_queue_get(g_state.queue, pos);
    // skip NULL entries if there are fewer than max_stored_entries in queue
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
