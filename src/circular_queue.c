#include "circular_queue.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

bool increase_capacity(circular_queue* queue, size_t capacity);
bool decrease_capacity(circular_queue* queue,
                       size_t capacity,
                       free_elem_fn free_elem);

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
circular_queue_set_capacity(circular_queue* queue,
                            const size_t capacity,
                            free_elem_fn free_elem)
{
  if (capacity == queue->capacity) {
    return true;
  } else if (capacity > queue->capacity) {
    return increase_capacity(queue, capacity);
  } else {
    return decrease_capacity(queue, capacity, free_elem);
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
  assert(capacity > queue->capacity);

  void** new_entries = calloc(capacity, MEMBER_SIZE(circular_queue, entries));
  if (!new_entries) {
    return false;
  }

  const size_t num_filled = min(queue->count, queue->capacity);
  for (size_t pos = queue->count - num_filled; pos < queue->count; pos++) {
    new_entries[pos % capacity] = queue->entries[pos % queue->capacity];
  }

  free(queue->entries);
  queue->entries = new_entries;
  queue->capacity = capacity;
  return true;
}

bool
decrease_capacity(circular_queue* queue,
                  const size_t capacity,
                  free_elem_fn free_elem)
{
  assert(capacity < queue->capacity);
  assert(capacity > 0);

  void** new_entries = malloc(capacity * MEMBER_SIZE(circular_queue, entries));
  if (!new_entries) {
    return false;
  }

  const size_t num_filled = min(queue->count, queue->capacity);
  const size_t num_excess_elems = safe_sub(num_filled, capacity, 0);
  size_t old_pos = 0;
  if (num_excess_elems > 0) {
    old_pos = safe_sub(queue->count, queue->capacity, 0);
    if (free_elem) {
      for (size_t i = 0; i < num_excess_elems; i++, old_pos++) {
        free_elem(queue->entries[old_pos % queue->capacity]);
      }
    } else {
      old_pos += num_excess_elems;
    }
  }

  for (size_t i = 0, new_pos = old_pos; i < capacity; i++, new_pos++, old_pos++) {
    new_entries[new_pos % capacity] = queue->entries[old_pos % queue->capacity];
  }

  free(queue->entries);
  queue->entries = new_entries;
  queue->capacity = capacity;
  return true;
}
