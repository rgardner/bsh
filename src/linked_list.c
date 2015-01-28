#include "linked_list.h"

#include <stdlib.h>
#include "node.h"

struct LinkedList *
ll_init()
{
  struct LinkedList *list = malloc(sizeof(struct LinkedList));
  list->size = 0;
  return list;
}

void
ll_free(struct LinkedList *list)
{
  struct Node *current = list->head;
  while (current) {
    struct Node *cp = current->next;
    free(current);
    current = cp;
  }
  free(list);
}

int
ll_add(struct LinkedList *list, int index, void *element)
{
  struct Node *new = malloc(sizeof(struct Node));
  new->data = element;
  list->size++;

  if (index == 0) {
    new->next = list->head;
    list->head = new;
    return 0;
  }

  struct Node *current = list->head;
  int i;
  for (i = 0; (i < index) && (current->next); i++) current = current->next;
  if (current->next) new->next = current->next->next;
  current->next = new;
  return i;
}

void
ll_add_first(struct LinkedList *list, void *element)
{
  ll_add(list, 0, element);
}

void
ll_add_after(struct Node *n, void *element)
{
  struct Node *new = malloc(sizeof(struct Node));
  *new = (struct Node){ .data = element, .next = n->next };
  n->next = new;
}

void *
ll_get(struct LinkedList *list, int index)
{
  struct Node *current = list->head;
  if (index == 0) {
    if (current) return current->data;
    return NULL;
  }
  for (int i = 0; (i < index) && (current); i++) current = current->next;
  if (current) return current->data;
  return NULL;
}

void *
ll_remove(struct LinkedList *list)
{
  struct Node *n = list->head;
  if (n == NULL) return NULL;

  list->head = n->next;
  list->size--;
  return n->data;
}

int
ll_size(struct LinkedList *list)
{
  return list->size;
}
