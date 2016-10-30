#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

struct Stack;

/* Return a pointer to a new stack. */
struct Stack* stack_init();

/* Free the stack. */
void stack_free(const struct Stack*);

/* Tests if the stack is empty. */
bool stack_empty(const struct Stack*);

/* Get an element at index i from the stack. */
void* stack_get(const struct Stack*, const int);

/* Return the data on the top of the stack without removing it. */
void* stack_peak(const struct Stack*);

/* Pop an element from the stack. */
void* stack_pop(const struct Stack*);

/* Push an element on the stack. */
void stack_push(const struct Stack*, const void*);

/* Returns the number of elements in the list. */
int stack_size(const struct Stack*);
#endif
