/* 
 * File:   main.h
 * Author: Samuel Jackson
 *
 * Created on 11 November 2012, 15:01
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

/*if the structures have not already been included,
 *include them.
 */
#ifndef STRUCTURES_H
    #include "structures.h"
#endif

/*macro to clear the command line*/
#define clear_screen() if(system("clear")){ system("cls"); }

/*Print out details about the current event*/
void print_event_data(event *evt);

/*Query the current status of a competitor in this event*/
void query_competitor(linked_list eventlist);

/*Check the number of competitors with a given status, e.g. ON_TRACK or COMPLETED*/
int check_num_competitors(linked_list eventlist, enum entrant_status type);

/*Let the user manually enter a checkpoint update into the system.*/
void manually_read_data(event *evt);

/*Read in a specified file of recorded checkpoint times into the system.*/
void read_updates(event *evt);

/*Print each competitors along with the time they started and the time they finished.*/
void print_results(linked_list eventlist);

/*Print a table of entrants that have been excluded depending on their type, 
 * e.g. excluded at a medical checkpoint
 */
void print_entrants_excluded(linked_list eventlist, enum entrant_status type);

/*Add a new time recorded at a checkpoint into the program.*/
void add_new_time(event *e, CP_Data data);

/*Update the other competitors in the competition relative to the last update*/
void update_others(event *evt, CP_Data data);

/*Update the position on an entrant currently out on a track*/
void update_entrant_on_track(event * evt, entrant * entrant, CP_Data data);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

