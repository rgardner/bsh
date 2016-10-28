#include "node.h"

#include <stdlib.h>

Node* node_init(const void* data) {
  Node* node = malloc(sizeof(Node));
  node->data = (void*)data;
  node->next = NULL;
  return node;
}

void* node_free(Node* node) {
  if (!node) {
    return NULL;
  }

  void* data = node->data;
  free(node);
  return data;
}
