#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

struct stack;

/* Return a pointer to a new stack. */
struct stack *
stack_init();

/* Free the stack. */
void
stack_free(struct stack *);

/* Tests if the stack is empty. */
bool
stack_empty(struct stack *);

/* Get an element at index i from the stack. */
void *
stack_get(struct stack *, int);

/* Return the data on the top of the stack without removing it. */
void *
stack_peak(struct stack *);

/* Pop an element from the stack. */
void *
stack_pop(struct stack *);

/* Push an element on the stack. */
void
stack_push(struct stack *, void *);

/* Returns the number of elements in the list. */
int
stack_size(struct stack *);
#endif
