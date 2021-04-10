set(linked_lists_files
        linked_list.c
        linked_list.h)

add_library(LinkedLists ${linked_lists_files})
add_subdirectory(nodes)
target_link_libraries(LinkedLists Nodes)
