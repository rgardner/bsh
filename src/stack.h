#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "node.h"

struct stack {
  struct node *head;
  int size;
};

/* Return a pointer to a new stack. */
struct stack *
stack_init();

/* Return the data on the top of the stack without removing it. */
void *
stack_peak(struct stack *);

/* Pop an element from the stack. */
void *
stack_pop(struct stack *);

/* Push an element on the stack. */
void
stack_push(struct stack *, void *);

/* Get an element at index i from the stack. */
void *
stack_get(struct stack *, int);

/* Free the stack. */
void
stack_free(struct stack *);

/* Tests if the stack is empty. */
bool
stack_empty(struct stack *);
#endif
