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

/*Query the current status of a competitor in this event*/
void query_competitor(Linked_List eventlist);

/*Check the number of competitors with a given status, e.g. ON_TRACK or COMPLETED*/
int check_num_competitors(Linked_List eventlist, enum entrant_status type);

/*Let the user manually enter a checkpoint update into the system.*/
void manually_read_data(Event *evt);

/*Read in a specified file of recorded checkpoint times into the system.*/
void read_updates(Event *evt);

/*Print each competitors along with the time they started and the time they finished.*/
void print_results(Linked_List eventlist);

/*Print a table of entrants that have been excluded depending on their type, 
 * e.g. excluded at a medical checkpoint
 */
void print_entrants_excluded(Linked_List eventlist, enum entrant_status type);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

