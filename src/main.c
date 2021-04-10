#include <stdio.h>
#include "../libs/data_structures/lists/linked_list.h"

int main() {
    // Linked list testing.
    struct LinkedList linked_list = linked_list_constructor();
    int array[5] = {1, 3, 4, 2, 5};
    for(int i = 0; i < 5; i++)
        linked_list.append(&linked_list, array[i], sizeof(int));
    printf("%d", linked_list.retrieve(&linked_list, 2));
    return 0;
} 