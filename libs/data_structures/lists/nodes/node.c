#include "node.h"

#include <stdlib.h>
#include <string.h>

struct Node node_constructor(void *data, unsigned long size) {
    // Constructor for the node structure.

    // Initialize nodes.
    struct Node node;
    // Store provided data.
    node.data = data;
    // Set next node to be NULL.
    node.next = NULL;
    return node;
}

void node_destructor(struct Node *node) {
    // Method that free up the memory allocated for a node.
    free(node->data);
    free(node);
}