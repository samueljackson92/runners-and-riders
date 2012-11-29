/* 
 * File:   util.h
 * Author: samuel
 *
 * Created on 29 November 2012, 21:30
 */

#ifndef UTIL_H
#define	UTIL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#ifndef STRUCTURES_H
    #include "structures.h"
#endif
    
#define OUTPUT_BUFF 50

void add_new_time(Event *e, CP_Data data);

void update_others(Event *evt, CP_Data data);

void convert_type_status(enum type_status type, char buffer[OUTPUT_BUFF]);

int convert_time_to_mins(char time[TIME_STRING_SIZE]);

int calc_time_diff (char *start, char *end);

Track * find_track(Linked_List list, int node_a, int node_b);

Entrant * find_entrant(Linked_List list, int id);

Course * findCourse(Linked_List *list, char c);

Node * find_node(Linked_List list, int id);

int find_next_checkpoint(Linked_List nodes, Entrant * e);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_H */

