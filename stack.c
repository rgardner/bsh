#include "stack.h"

#include <stdlib.h>

struct node*
stack_init() {
  return (struct node *) malloc(sizeof(struct node));
}

void *
stack_pop(struct node *stack) {
  struct node *current;
  void *data;

  current = stack;
  // only one element in the stack.
  if (!(current->next)) {
    data = current->data;
    current->data = NULL;
    return data;
  }
  while (current->next->next) current = current->next;

  data = current->next->data;
  current->data = NULL;
  return data;
}

void
stack_push(struct node *stack, void *data)
{
  struct node *current = stack;
  while (current->next) current = current->next;

  struct node *element = malloc(sizeof(struct node));
  element->data = data;
  current->next = element;
}
