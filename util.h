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

/*Convert a given entrant status into a user readable string*/
void convert_type_status_verbose(enum entrant_status type, char buffer[OUTPUT_BUFF]);

/*Convert the given checkpoint string into an enum format*/
enum check_point convert_node_type(char type_string[3]);

/*Convert a given time string into an integer representing the minutes since midnight*/
int convert_time_to_mins(char time[TIME_STRING_SIZE]);

/*Covert textual mins or hours into integers and return their difference*/
int calc_time_diff (char *start, char *end);

/*Find a track given by comparing if it consists of the two given nodes*/
track * find_track(linked_list list, int node_a, int node_b);

/*Find an entrant using its id*/
entrant * find_entrant(linked_list list, int id);

/*Find a course using its id*/
course * find_course(linked_list list, char c);

/*Find the given entrant's next checkpoint on their course*/
int find_next_checkpoint(linked_list nodes, entrant * e);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_H */

