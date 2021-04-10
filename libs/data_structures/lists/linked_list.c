#include <stdlib.h>
#include <stdio.h>

#include "linked_list.h"


// Deallocate memory for a specified nodes.
void _free_node(struct Node *node);

// Method that append provided data to the end of the list.
void _append(struct LinkedList *linked_list, void *data, unsigned long size);

// Method that return element with provided index.
void *_get(struct LinkedList *linked_list, int index);

// Method that return element with provided index.
void *_retrieve(struct LinkedList *linked_list, int index);

// Method that delete element with provided index.
void _delete(struct LinkedList *linked_list, int index);


struct LinkedList linked_list_constructor() {
    // Constructor for the linked list structure.

    // Create new linked list.
    struct LinkedList linked_list;
    // Initialize head of the linked list.
    linked_list.head = NULL;
    // Initialize length of the linked list.
    linked_list.length = 0;

    // Define public methods.
    linked_list.append = _append;
    linked_list.retrieve = _retrieve;
    linked_list.delete = _delete;

    return linked_list;
}

void linked_list_destructor(struct LinkedList *linked_list) {
    // Method that free up the memory allocated for a linked list.
    for (int i = 0; i < linked_list->length; i++)
        linked_list->delete(linked_list, 0);
}

void _free_node(struct Node *node) {
    // Deallocate memory for a specified nodes.
    node_destructor(node);
}

void _append(struct LinkedList *linked_list, void *data, unsigned long size) {
    // Method that append provided data to the end of the list.

    // Create pointer to node to be inserted.
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    *node = node_constructor(data, size);

    // Check whether there are no elements in the list.
    if (linked_list->length == 0)
        // Created node is head.
        linked_list->head = node;
    else {
        // Get the tail of the list.
        struct Node *tail = _get(linked_list, linked_list->length - 1);
        // Append the created node.
        tail->next = node;
    }
    // Increment the length.
    linked_list->length += 1;
}

void *_get(struct LinkedList *linked_list, int index) {
    // Method that return node with provided index.

    // Check whether an index is odd.
    if (index < 0 || index >= linked_list->length)
        return NULL;

    // Create pointer to the head node.
    struct Node *current = linked_list->head;
    // Traverse the list until the next element has provided index.
    for (int i = 0; i < index; i++)
        current = current->next;
    return current;
}

void *_retrieve(struct LinkedList *linked_list, int index) {
    // Method that find element with provided index.

    // Get the node with a provided index.
    struct Node *node = _get(linked_list, index);
    // Check whether the node does exist.
    if (node)
        return node->data;
    else
        return NULL;
}

void _delete(struct LinkedList *linked_list, int index) {
    // Method that delete element with provided index.

    // Check whether node to be removed is head of list.
    if (index == 0) {
        struct Node *node = linked_list->head;
        if (node) {
            // Set next node of a current head to be head.
            linked_list->head = node->next;
            _free_node(node);
        }
    } else {
        // Remove node from the list by changing pointer to next nodes.
        struct Node *predecessor = _get(linked_list, index - 1);
        struct Node *node = predecessor->next;
        predecessor->next = node->next;
        _free_node(node);
    }
    // Decrement the length.
    linked_list->length -= 1;
}
