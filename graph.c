
#include "structures.h"
#include <stdlib.h>

void add_edges(Track *tracks, Node *nodes){
    int i=0;
    int len = sizeof(tracks) / sizeof(Track);
    for (i=0; i<len; i++){
        /*bidirectional graph, add links both ways*/
        add_edge(nodes, tracks[i].nodea, tracks[i].nodeb);
        add_edge(nodes, tracks[i].nodeb, tracks[i].nodea);
    }
}

void add_edge(Node *nodes, int a, int b) {
    Neighbour_node n;
    n.node = b;
    Neighbour_node *current = nodes[a].neighbours;

    while(current->next) {
        current = current->next;
    }
    
    current->next = &n;
}