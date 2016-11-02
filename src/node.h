/** A node in a singly-linked list.
 *
 *  @file
 *
 */

#ifndef NODE_H
#define NODE_H

typedef struct Node Node;

/** Node in a singly-linked list. */
struct Node
{
  void* data; /**< Arbitrary data. */
  Node* next; /**< Pointer to next node in singly-linked list. */
};

/** Return a newly allocated node.
 *
 *  @param data Pointer to data for node to store. Data is aliased, not copied.
 *  @return Pointer to newly allocated node.
 */
Node* node_init(const void* const data);

/** Free the node.
 *
 *  node->next is not freed; the caller is responsible for keeping a pointer to
 *  the next node.
 *
 *  @return Pointer to node data.
 */
void* node_free(Node* n);
#endif
