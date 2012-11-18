#include <stdlib.h>

#include "linked_list.h"

void add_element(Linked_List *list, List_Node *new) {
    if(list->head == NULL) {
        list->head = new;
        list->tail = new;
    } else if(list->tail->next == NULL) {
        list->tail->next = new;
        list->tail = new;
    }
}

List_Node * get_element(List_Node *list, int number){
    int count = 0;
    List_Node *current = list;
    
    while(count != number) {
        count++;
        current = current->next;
    }
    
    return current;
}

void * get_element_data(List_Node *list, int number) {
    List_Node *found = get_element(list, number);
    return found->data;
}