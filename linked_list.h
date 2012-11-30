/* 
 * File:   linked_list.h
 * Author: samuel
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
typedef struct list_node {
    void *data;
    struct list_node *next;
}List_Node;

/* Structure to hold pointers to the head and tail of a whole list.
 * Uses the pointer to the tail for faster insertion.
 */
typedef struct linked_list {
    List_Node *head;
    List_Node *tail;
}Linked_List;

/*add a new element to the specified linked list*/
void add_element(Linked_List *list, List_Node * node);

/*traverse the list and run the given function on each data item.*/
void traverse_list(List_Node *current, void (*process_node)(void *data));

#ifdef	__cplusplus
}
#endif

#endif	/* LINKED_LIST_H */

