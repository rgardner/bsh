#ifndef LinkedList_H
#define LinkedList_H
struct LinkedList;

/* Return a pointer to a new linked list. */
struct LinkedList *
ll_init();

/* Free every element in the list and the list itself. */
void
ll_free(struct LinkedList *list);

/* Inserts the specified element at the specified position in this list. */
void
ll_int(struct LinkedList *list, int index, void *element);

/* Inserts the specified element to the beginning of the list. */
void
ll_add_first(struct LinkedList *list, void *element);

/* Returns the element at the specified position in this list. */
void *
ll_get(struct LinkedList *list, int index);

/* Retrieves and removes the head (first element) of this list. */
void *
ll_remove(struct LinkedList *list);

/* Returns the number of elements. */
int
ll_size(struct LinkedList *list);
#endif
