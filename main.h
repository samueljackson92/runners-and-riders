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

char * convert_type_status(enum type_status type);
void query_competitor(Event *e);
int check_num_competitors(Linked_List *el, enum type_status type);
void add_new_time(Event *e, CP_Data data);
void manually_read_data(Event *e);
void update_others(Event *evt, CP_Data data);
void read_updates(Event *e);
int calc_time_diff (char *start, char *end);
void print_results(Event *e);
void print_entrants_excluded(Event *e, enum type_status type);
void clear_screen();

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

