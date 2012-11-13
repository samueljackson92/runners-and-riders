/* 
 * File:   graph.h
 * Author: Samuel Jackson (slj11@aber.ac.uk)
 *
 * Created on 13 November 2012, 18:13
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include "structures.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
/*Add all edges from the tracks list to the graph*/
void add_edges(Track *tracks, Node *nodes);

/*Add an edge from node a to node b in the graph*/
void add_edge(Node *nodes, int a, int b);

#ifdef	__cplusplus
}
#endif

#endif	/* GRAPH_H */

