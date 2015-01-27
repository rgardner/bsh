#include "linked_list.h"

#include <stdlib.h>
#include "node.h"

struct linked_list {
  struct node *head;
  int size;
};

struct linked_list *
ll_init()
{
  struct linked_list *list = malloc(sizeof(struct linked_list));
  list->size = 0;
  return list;
}

void
ll_free(struct linked_list *list)
{
  struct node *current = list->head;
  while (current) {
    struct node *cp = current->next;
    free(current);
    current = cp;
  }
  free(list);
}

int
ll_add(struct linked_list *list, int index, void *element)
{
  struct node *new = malloc(sizeof(struct node));
  new->data = element;
  list->size++;

  if (index == 0) {
    new->next = list->head;
    list->head = new;
    return 0;
  }

  struct node *current = list->head;
  int i;
  for (i = 0; (i < index) && (current->next); i++) current = current->next;
  if (current->next) new->next = current->next->next;
  current->next = new;
  return i;
}

void
ll_add_first(struct linked_list *list, void *element)
{
  ll_add(list, 0, element);
}

void *
ll_get(struct linked_list *list, int index)
{
  struct node *current = list->head;
  if (index == 0) {
    if (current) return current->data;
    return NULL;
  }
  for (int i = 0; (i < index) && (current); i++) current = current->next;
  if (current) return current->data;
  return NULL;
}

void *
ll_remove(struct linked_list *list)
{
  struct node *n = list->head;
  if (n == NULL) return NULL;

  list->head = n->next;
  list->size--;
  return n->data;
}

int
ll_size(struct linked_list *list)
{
  return list->size;
}
