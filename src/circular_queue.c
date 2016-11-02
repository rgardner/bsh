#include "circular_queue.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

circular_queue*
circular_queue_init(size_t capacity)
{
  circular_queue* queue = malloc(sizeof(circular_queue));
  queue->capacity = capacity;
  queue->entries = calloc(capacity, MEMBER_SIZE(circular_queue, entries));
  return queue;
}

void
circular_queue_push(circular_queue* queue, void* elem)
{
  UNUSED(queue);
  UNUSED(elem);
}

void
circular_queue_set_capacity(circular_queue* queue, size_t capacity)
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
