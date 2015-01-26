#ifndef STACK_H
#define STACK_H

struct stack {
  void *head;
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

/* Free the stack. */
void
stack_free(struct stack *);
#endif
