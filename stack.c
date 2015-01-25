#include "stack.h"

#include <stdlib.h>

struct node {
  void *data;
  struct node *next;
};

struct stack*
stack_init()
{
  return (struct stack*) malloc(sizeof(struct stack));
}

void *
stack_pop(struct stack *s)
{
  void *data;
  struct node *head;

  head = s->head;
  if (!(head)) return NULL;
  data = head->data;

  s->head = head->next;
  free(head);

  return data;
}

void
stack_push(struct stack *s, void *data)
{
  struct node *element;
  struct node *head;

  head = s->head;
  element = malloc(sizeof(struct node));
  element->data = data;
  element->next = head;

  s->head = element;
}
