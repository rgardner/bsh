#include "stack.h"

#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "linked_list.h"

struct Stack {
  void *list;
};

struct Stack*
stack_init()
{
  struct Stack *s = malloc(sizeof(struct Stack));
  s->list = ll_init();
  return s;
}

void stack_free(struct Stack *s)
{
  ll_free(s->list);
  free(s);
}

bool
stack_empty(struct Stack *s)
{
  return ll_size(s->list) == 0;
}

void *
stack_get(struct Stack *s, int index)
{
  return ll_get(s->list, index);
}

void *
stack_peak(struct Stack *s)
{
  return ll_get(s->list, 0);
}

void *
stack_pop(struct Stack *s)
{
  return ll_remove(s->list);
}

void
stack_push(struct Stack *s, void *data)
{
  return ll_add_first(s->list, data);
}

int
stack_size(struct Stack *s)
{
  return ll_size(s->list);
}
