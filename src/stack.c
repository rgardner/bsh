#include "stack.h"

#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "linked_list.h"

struct stack {
  void *list;
};

struct stack*
stack_init()
{
  struct stack *s = malloc(sizeof(struct stack));
  s->list = ll_init();
  return s;
}

void stack_free(struct stack *s)
{
  ll_free(s->list);
  free(s);
}

bool
stack_empty(struct stack *s)
{
  return ll_size(s->list) == 0;
}

void *
stack_get(struct stack *s, int index)
{
  return ll_get(s->list, index);
}

void *
stack_peak(struct stack *s)
{
  return ll_get(s->list, 0);
}

void *
stack_pop(struct stack *s)
{
  return ll_remove(s->list);
}

void
stack_push(struct stack *s, void *data)
{
  return ll_add_first(s->list, data);
}

int
stack_size(struct stack *s)
{
  return ll_size(s->list);
}
