#ifndef LinkedList_H
#define LinkedList_H
#include "node.h"

struct LinkedList
{
  struct Node* head;
  int size;
};

/* Return a pointer to a new linked list. O(1). */
struct LinkedList* ll_init();

/* Free every element in the list and the list itself. O(n). */
void ll_free(const struct LinkedList* list);

/* Inserts the specified element at the specified position in this list. O(n).
 * If index > ll_size(list), the element will be inserted at the end of the
 * list.
 * Returns the index of the newly inserted element. */
int ll_add(struct LinkedList* list, const int index, const void* element);

/* Inserts the specified element to the beginning of the list. O(1). */
void ll_add_first(struct LinkedList* list, const void* element);

/* Inserts the specified element after node n in the list. O(1).
 *
 * If n is NULL; then set element to the head of the list. */
void ll_add_after(struct LinkedList* list, struct Node* n, const void* element);

/* Returns the element at the specified position in this list. O(n). */
void* ll_get(const struct LinkedList* list, const int index);

/* Retrieves and removes the head (first element) of this list. O(1). */
void* ll_remove(struct LinkedList* list);

/* Returns the number of elements. O(1). */
int ll_size(const struct LinkedList* list);
#endif
