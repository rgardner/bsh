#include "stack.h"

#include <stdlib.h>
#include <string.h>

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
stack_peak(struct stack *s)
{
  struct node *head;

  head = s->head;
  if (!(head)) return NULL;
  return head->data;
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
