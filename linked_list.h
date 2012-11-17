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
void traverse_list(List_Node *current, void (*process_node) (List_Node *node));

#ifdef	__cplusplus
}
#endif

#endif	/* LINKED_LIST_H */

