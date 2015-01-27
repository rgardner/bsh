#ifndef NODE_H
#define NODE_H
struct Node {
  void *data;
  struct Node *next;
};

/* Return a newly allocated node. */
struct Node *
node_init(void *data);

/* Free the node. */
void
node_free(struct Node *n);

/* Return the data attached to the node. */
void *
node_data(struct Node *n);
#endif
