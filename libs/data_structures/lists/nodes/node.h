#ifndef node_h
#define node_h

struct Node {
    // Implementation of Node of generic Linked List.

    // Data stored in the nodes.
    void *data;
    // Pointer to the next nodes.
    struct Node *next;
};

// Constructor for the node structure.
struct Node node_constructor(void *data, unsigned long size);

// Method that free up the memory allocated for a node.
void node_destructor(struct Node *node);

#endif