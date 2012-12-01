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

/*Macro specifying the maximum size of a buffer to convert
 *a type enum to a string*/    
#define OUTPUT_BUFF 50

/*Add a new time recorded at a checkpoint into the program.*/
void add_new_time(Event *e, CP_Data data);

/*Update the other competitors in the competition relative to the last update*/
void update_others(Event *evt, CP_Data data);

/*Update the position on an entrant currently out on a track*/
void update_entrant_on_track(Event * evt, Entrant * entrant, CP_Data data);

/*Convert a given entrant status into a user readable string*/
void convert_type_status(enum entrant_status type, char buffer[OUTPUT_BUFF]);

/*Convert a given time string into an integer representing the minutes since midnight*/
int convert_time_to_mins(char time[TIME_STRING_SIZE]);

/*Covert textual mins or hours into integers and return their difference*/
int calc_time_diff (char *start, char *end);

/*Find a track given by comparing if it consists of the two given nodes*/
Track * find_track(Linked_List list, int node_a, int node_b);

/*Find an entrant using its id*/
Entrant * find_entrant(Linked_List list, int id);

/*Find a course using its id*/
Course * find_course(Linked_List list, char c);

/*Find the given entrant's next checkpoint on their course*/
int find_next_checkpoint(Linked_List nodes, Entrant * e);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_H */

