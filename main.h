/* 
 * File:   main.h
 * Author: samuel
 *
 * Created on 11 November 2012, 15:01
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

char * convert_type_status(enum type_status type, char * output);
void queryCompetitor(Event *e);
int check_num_competitors(Linked_List *el, enum type_status type);
void add_new_time(Event *e, char time[TIME_STRING_SIZE], char type, 
                                int entrant_num, int checkpoint_num);
void manually_read_data(Event *e);
void update_others(Event *evt, int latest);
void read_updates(Event *e);
int calc_total_time (char *start, char *end);
void print_results(Event *e);
void clearScreen();

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

