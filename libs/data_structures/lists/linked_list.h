#ifndef linked_list_h
#define linked_list_h

#include "nodes/node.h"

struct LinkedList {
    // Implementation of generic Linked List.

    // Pointer to the first nodes in the list.
    struct Node *head;
    // Length of the Linked List.
    int length;

    // Method that append provided data to the end of the list.
    void (*append)(struct LinkedList *linked_list, void *data, unsigned long size);

    // Method that return element with provided index.
    void *(*retrieve)(struct LinkedList *linked_list, int index);

    // Method that delete element with provided index.
    void (*delete)(struct LinkedList *linked_list, int index);
};


// Constructor for the linked list structure.
struct LinkedList linked_list_constructor(void);

// Method that free up the memory allocated for a linked list.
void linked_list_destructor(struct LinkedList *linked_list);

#endif