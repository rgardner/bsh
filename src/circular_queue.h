/** Fixed size circular queue with explicit growth/shrink functionality.
 *
 * @file
 *
 */

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stddef.h>  // size_t

typedef struct
{
  void** entries;   /**< The entries must be allocated on the heap. */
  size_t count;     /**< The total number of entries ever saved. */
  size_t capacity;  /**< The number of entries allocated for. */
} circular_queue;

circular_queue* circular_queue_init(size_t capacity);

/** Append an element to the end of the queue.
 *
 *  @note elem must be allocated on the heap. If the queue is full, an existing
 *        entry will be removed and automatically deallocated.
 */
void circular_queue_push(circular_queue* const queue, void* elem);

/** Get the element at an absolute position in the queue.
 *
 *  @code
 *  #include <assert.h>
 *  #include <string.h>
 *
 *  circular_queue* queue = circular_queue_init(2);
 *  circular_queue_push(queue, strdup("elem0"));
 *  circular_queue_push(queue, strdup("elem1"));
 *  circular_queue_push(queue, strdup("elem2"));
 *  assert(!circular_queue_get(queue, 0));
 *  assert(circular_queue_get(queue, 1));
 *  assert(circular_queue_get(queue, 2));
 *  circular_queue_free(queue);
 *  @endcode
 *  @return The element if it exists; NULL if not.
 */
void* circular_queue_get(const circular_queue* const queue, size_t pos);

void circular_queue_set_capacity(circular_queue* queue, size_t capacity);

void circular_queue_free(circular_queue* queue);

#endif
