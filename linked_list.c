#include <stdlib.h>

#include "linked_list.h"

void add_element(List_Node *current, List_Node *new) {
    if(current->next == NULL) {
        current->next = new;
    } else {
        add_element(current->next, new);
    }
}

void traverse_list(List_Node *current, void (*process_node) (List_Node *node)) {
    if(current != NULL) {
        process_node(current);
        if(current->next != NULL){
            traverse_list(current->next, process_node);
        }
    }
}