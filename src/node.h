#ifndef NODE_H
#define NODE_H
struct Node {
  void *data;
  struct Node *next;
};

/* Return a newly allocated node. */
struct Node *node_init(const void *data);

/* Free the node. */
void *node_free(const struct Node *n);
#endif
