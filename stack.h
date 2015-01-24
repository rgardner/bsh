#ifndef STACK_H
#define STACK_H

struct node {
  void *data;
  struct node *next;
};

/* Return a pointer to a new stack. */
struct node *
stack_init();

/* Pop an element from the stack. */
void *stack_pop(struct node *);

/* Push an element on the stack. */
void stack_push(struct node *, void *);
#endif
