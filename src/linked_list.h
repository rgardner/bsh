#ifndef LINKED_LIST_H
#define LINKED_LIST_H
struct linked_list;

/* Return a pointer to a new linked list. */
struct linked_list *
ll_init();

/* Free every element in the list and the list itself. */
void
ll_free(struct linked_list *list);

/* Inserts the specified element at the specified position in this list. */
void
ll_int(struct linked_list *list, int index, void *element);

/* Inserts the specified element to the beginning of the list. */
void
ll_add_first(struct linked_list *list, void *element);

/* Returns the element at the specified position in this list. */
void *
ll_get(struct linked_list *list, int index);

/* Retrieves and removes the head (first element) of this list. */
void *
ll_remove(struct linked_list *list);

/* Returns the number of elements. */
int
ll_size(struct linked_list *list);
#endif
