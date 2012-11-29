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

#ifndef STRUCTURES_H
    #include "structures.h"
#endif

#define clear_screen() if(system("clear")){ system("cls"); }
    
void query_competitor(Event *e);

int check_num_competitors(Linked_List *el, enum type_status type);

void add_new_time(Event *e, CP_Data data);

void manually_read_data(Event *e);

void read_updates(Event *e);

void print_results(Event *e);

void print_entrants_excluded(Event *e, enum type_status type);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

