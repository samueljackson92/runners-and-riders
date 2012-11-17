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

void add_element(List_Node *, List_Node *);
void traverse_list(List_Node *current, void (*process_node) (List_Node *node));

#ifdef	__cplusplus
}
#endif

#endif	/* LINKED_LIST_H */

