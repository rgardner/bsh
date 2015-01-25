#ifndef STACK_H
#define STACK_H

struct stack {
  void *head;
};

/* Return a pointer to a new stack. */
struct stack *
stack_init();

/* Pop an element from the stack. */
void *
stack_pop(struct stack *);

/* Push an element on the stack. */
void
stack_push(struct stack *, void *);
#endif
