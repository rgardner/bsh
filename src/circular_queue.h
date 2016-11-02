/** Fixed size circular queue with explicit growth/shrink functionality.
 *
 * @file
 *
 */

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stddef.h>  // size_t

typedef struct {
  void** entries;
  size_t capacity;
} circular_queue;

circular_queue*
circular_queue_init(size_t capacity);

void
circular_queue_push(circular_queue* queue, void* elem);

void
circular_queue_set_capacity(circular_queue* queue, size_t capacity);

void
circular_queue_free(circular_queue* queue);

#endif
