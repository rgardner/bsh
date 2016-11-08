#include "circular_queue.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

bool increase_capacity(circular_queue* queue, size_t capacity);
bool decrease_capacity(circular_queue* queue, size_t capacity);

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
circular_queue_free(circular_queue* queue)
{
  if (queue->entries) {
    free(queue->entries);
  }

  free(queue);
}

void*
circular_queue_push(circular_queue* const queue, void* elem)
{
  void* old = NULL;
  void** entry = &(queue->entries[queue->count % queue->capacity]);
  if (*entry) {
    old = *entry;
  }

  *entry = elem;
  queue->count++;
  return old;
}

void*
circular_queue_get(const circular_queue* const queue, const size_t pos)
{
  if (pos >= queue->count) {
    return NULL;
  }

  if (queue->count > queue->capacity) {
    if (pos < (queue->count - queue->capacity)) {
      return NULL;
    }
  }

  return queue->entries[pos % queue->capacity];
}

bool
circular_queue_set_capacity(circular_queue* queue, const size_t capacity)
{
  if (capacity == queue->capacity) {
    return true;
  } else if (capacity > queue->capacity) {
    return increase_capacity(queue, capacity);
  } else {
    return decrease_capacity(queue, capacity);
  }
}

/** Increase capacity of queue in O(capacity) time.
 *
 *  If increase_capacity() fails, queue is unchanged and false is returned.
 *  @return true if queue capacity was successfully increased, false otherwise.
 */
bool
increase_capacity(circular_queue* queue, const size_t capacity)
{
  void** temp = malloc(capacity * MEMBER_SIZE(circular_queue, entries));
  if (!temp) {
    return false;
  }

  size_t pivot = queue->count % queue->capacity;
  for (size_t i = 0; i < pivot; i++) {
    temp[i] = NULL;
  }

  for (size_t i = pivot; i < queue->capacity; i++) {
    temp[i] = queue->entries[i];
  }

  for (size_t i = 0, j = queue->capacity; i < pivot; i++, j++) {
    temp[j] = queue->entries[i];
  }

  free(queue->entries);
  queue->entries = temp;
  queue->capacity = capacity;
  return true;
}

bool
decrease_capacity(circular_queue* queue, const size_t capacity)
{
  UNUSED(queue);
  UNUSED(capacity);
  return true;
}
