#include "stack.h"

#include <stdlib.h>
#include <string.h>

#include "node.h"

struct stack*
stack_init()
{
  struct stack *s = malloc(sizeof(struct stack));
  s->size = 0;
  return s;
}

void *
stack_peak(struct stack *s)
{
  struct node *head;

  head = s->head;
  if (!head) return NULL;
  return head->data;
}

void *
stack_pop(struct stack *s)
{
  struct node *head = s->head;
  if (!head) return NULL;

  void *data = head->data;
  s->head = head->next;
  free(head);
  s->size--;

  return data;
}

void
stack_push(struct stack *s, void *data)
{
  struct node *element = malloc(sizeof(struct node));
  element->data = data;
  element->next = s->head;

  s->head = element;
  s->size++;
}

void *
stack_get(struct stack *s, int index)
{
  if (index > s->size) return NULL;

  struct node *current = s->head;
  if (index == 0) return current->data;

  for (int i = 0; (i < index) && (current); i++) {
    current = current->next;
  }

  if (current) return current->data;
  return NULL;
}

bool
stack_empty(struct stack *s)
{
  return (s->head == NULL);
}

void stack_free(struct stack *s)
{
  struct node *element;
  struct node *other;

  if (!s) return;

  element = s->head;
  while (element) {
    if (element->data) free(element->data);
    other = element;
    element = element->next;
    free(other);
  }
  free(s);
}
