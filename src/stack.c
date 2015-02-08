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

void stack_free(const struct Stack *s)
{
  ll_free(s->list);
  free((struct Stack *)s);
}

bool
stack_empty(const struct Stack *s)
{
  return ll_size(s->list) == 0;
}

void *
stack_get(const struct Stack *s, const int index)
{
  return ll_get(s->list, index);
}

void *
stack_peak(const struct Stack *s)
{
  return ll_get(s->list, 0);
}

void *
stack_pop(const struct Stack *s)
{
  return ll_remove(s->list);
}

void
stack_push(const struct Stack *s, const void *data)
{
  return ll_add_first(s->list, data);
}

int
stack_size(const struct Stack *s)
{
  return ll_size(s->list);
}
