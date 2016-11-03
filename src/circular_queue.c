#include "circular_queue.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

circular_queue*
circular_queue_init(const size_t capacity)
{
  assert(capacity > 0);
  circular_queue* queue = malloc(sizeof(circular_queue));
  if (!queue) {
    return NULL;
  }

  queue->entries = calloc(capacity, MEMBER_SIZE(circular_queue, entries));
  if (!queue->entries) {
    free(queue);
    return NULL;
  }

  queue->count = 0;
  queue->capacity = capacity;
  return queue;
}

void
circular_queue_push(circular_queue* const queue, void* elem)
{
  void** entry = &(queue->entries[queue->count % queue->capacity]);
  if (*entry) {
    free(*entry);
  }

  *entry = elem;
  queue->count++;
}

void*
circular_queue_get(const circular_queue* const queue, const size_t pos)
{
  if (pos > queue->count) {
    return NULL;
  }

  // because size_t is unsigned and casting to int is dangerous, we can't simply
  // do: (pos < (queue->count - queue->capacity))
  if (queue->count < queue->capacity) {
    if (pos > queue->count) {
      return NULL;
    }
  } else {
    if (pos < (queue->count - queue->capacity)) {
      return NULL;
    }
  }

  return queue->entries[pos % queue->capacity];
}

void
circular_queue_set_capacity(circular_queue* queue, const size_t capacity)
{
  UNUSED(queue);
  UNUSED(capacity);
}

void
circular_queue_free(circular_queue* queue)
{
  if (queue->entries) {
    for (size_t i = 0; i < queue->capacity; i++) {
      if (queue->entries[i]) {
        free(queue->entries[i]);
      }
    }

    free(queue->entries);
  }

  free(queue);
}
