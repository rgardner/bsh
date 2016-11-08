/** Fixed size circular queue with explicit growth/shrink functionality.
 *
 * @file
 *
 */

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t

typedef struct
{
  void** entries;
  size_t count;     /**< The total number of entries ever saved. */
  size_t capacity;  /**< The number of entries allocated for. */
} circular_queue;

circular_queue* circular_queue_init(size_t capacity);

/** Free the queue, the entries must be free'd first.
 *
 *  @code
 *  #include <assert.h>
 *  #include <string.h>
 *
 *  circular_queue* queue = circular_queue_init(3);
 *  assert(!circular_queue_push(queue, strdup("elem0")));
 *  assert(!circular_queue_push(queue, strdup("elem1")));
 *  for (size_t i = 0; i < queue->capacity; i++) {
 *    if (queue->entries[i]) {
 *      free(queue->entries[i]);
 *    }
 *  }
 *  circular_queue_free(queue);
 *  @endcodek
 */
void circular_queue_free(circular_queue* queue);

/** Append an element to the end of the queue, returning the one it replaces.
 *
 *  @note If the return value is ignored and it was heap-allocated, memory will
 *        have been leaked.
 *  @code
 *  #include <assert.h>
 *  #include <string.h>
 *
 *  circular_queue* queue = circular_queue_init(2);
 *  assert(!circular_queue_push(queue, strdup("elem0")));
 *  assert(!circular_queue_push(queue, strdup("elem1")));
 *
 *  char* old = circular_queue_push(queue, strdup("elem2")));
 *  assert(old);
 *  free(old);
 *  
 *  for (size_t i = 0; i < queue->capacity; i++) {
 *    // we know the queue is currently full, so this check is technically
 *    // unnecessary here
 *    if (queue->entries[i]) {
 *      free(queue->entries[i]);
 *    }
 *  }
 *  circular_queue_free(queue);
 *  @endcode
 */
void* circular_queue_push(circular_queue* const queue, void* elem);

/** Get the element at an absolute position in the queue.
 *
 *  @code
 *  #include <assert.h>
 *
 *  circular_queue* queue = circular_queue_init(2);
 *  circular_queue_push(queue, "elem0");
 *  circular_queue_push(queue, "elem1");
 *  circular_queue_push(queue, "elem2");
 *  assert(!circular_queue_get(queue, 0));
 *  assert(circular_queue_get(queue, 1));
 *  assert(circular_queue_get(queue, 2));
 *  circular_queue_free(queue);
 *  @endcode
 *  @return The element if it exists; NULL if not.
 */
void* circular_queue_get(const circular_queue* const queue, size_t pos);

/** Change the capacity of the queue, reallocating and copying if necessary.
 *  @return true if queue capacity was successfully updated; false otherwise.
 */
bool circular_queue_set_capacity(circular_queue* queue, size_t capacity);
#endif
