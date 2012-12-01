#include <stdlib.h>
#include "linked_list.h"
/* 
 * File:   linked_list.c
 * Author: Samuel Jackson
 *
 * Created on 16 November 2012, 10:43
 */

/* Add an element to the list. Uses tail pointer for faster insertion. */
void add_element(linked_list *list, list_node *new) {
    if(list->head == NULL) {
        list->head = new;
        list->tail = new;
    } else if(list->tail->next == NULL) {
        list->tail->next = new;
        list->tail = new;
    }
}

/* Traverse the list and execute the given function each on each element. */
void traverse_list(list_node *current, void (*process_node)(void *data)) {
    if(current != NULL) {
        process_node(current->data);
        traverse_list(current->next, process_node);
    }
}