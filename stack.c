#include "stack.h"

#include <stdlib.h>

struct node {
  void *data;
  struct node *next;
};

struct stack {
  struct node *head;
};

struct stack*
stack_init()
{
  return (struct stack*) malloc(sizeof(struct node));
}

void *
stack_pop(struct stack *s)
{
  void *data;

  return data;
}

void
stack_push(struct stack *s, void *data)
{
  struct node *current = s->head;
  while (current->next) current = current->next;

  struct node *element = malloc(sizeof(struct node));
  element->data = data;
  current->next = element;
}
