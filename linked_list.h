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

typedef struct list_node {
    void *data;
    struct list_node *next;
}List_Node;

typedef struct linked_list {
    List_Node *head;
    List_Node *tail;
}Linked_List;

void add_element(Linked_List *, List_Node *);
void * get_element_data(List_Node *list, int number);
List_Node * get_element(List_Node *list, int number);

#ifdef	__cplusplus
}
#endif

#endif	/* LINKED_LIST_H */

