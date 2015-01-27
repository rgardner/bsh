#ifndef NODE_H
#define NODE_H
struct node {
  void *data;
  struct node *next;
};

/* Return a newly allocated node. */
struct node *
node_init(void *data);

/* Free the node. */
void
node_free(struct node *n);

/* Return the data attached to the node. */
void *
node_data(struct node *n);
#endif
