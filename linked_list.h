/* 
 * File:   linked_list.h
 * Author: Samuel Jackson
 *
 * Created on 16 November 2012, 10:45
 */

#ifndef LINKED_LIST_H
#define	LINKED_LIST_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Structure to hold data about a single node in the linked list
 * uses void pointers to hold a reference to data of any type.
 */
typedef struct list_node_s {
    void *data;
    struct list_node_s *next;
}list_node;
/* Structure to hold pointers to the head and tail of a whole list.
 * Uses the pointer to the tail for faster insertion.
 */
typedef struct linked_list_s {
    list_node *head;
    list_node *tail;
}linked_list;

/*add a new element to the specified linked list*/
void add_element(linked_list *list, list_node * node);

/*traverse the list and run the given function on each data item.*/
void traverse_list(list_node *current, void (*process_node)(void *data));

#ifdef	__cplusplus
}
#endif

#endif	/* LINKED_LIST_H */

