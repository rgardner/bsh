#include "node.h"

#include <stdlib.h>

/* Return a newly allocated node. */
struct Node *
node_init(const void *data)
{
  struct Node *node = malloc(sizeof(struct Node));
  node->data = (void *)data;
  node->next = NULL;
  return node;
}

/* Free the node. */
void *
node_free(const struct Node *node)
{
  if (!node) return NULL;
  void *data = node->data;
  free((struct Node *)node);
  return data;
}
