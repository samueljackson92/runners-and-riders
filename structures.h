/* 
 * File:   structures.h
 * Author: Samuel Jackson
 *
 * Created on 13 November 2012, 10:04
 */

#ifndef STRUCTURES_H
#define	STRUCTURES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "linked_list.h"
    
/*Macro defining the maximum size of an event name.*/
#define MAX_EVENT_NAME_SIZE 51
    
/*Macro defining the maximum size of an entrant name.*/
#define MAX_ENTRANT_NAME_SIZE 51

/*Maximum size of a textual date string (e.g. 12th June 2012)*/
#define MAX_DATE_SIZE 21
    
/*Maximum size of time string (e.g. 09:00)*/
#define TIME_STRING_SIZE 6

/*enum to signify what type of checkpoint a node is*/
enum check_point {
    CP, JN, MC
};

/*structure to hold information about a node and a pointer to the next node*/
typedef struct node {
    enum check_point type;
    int num;
} Node;

/*structure to hold information about a track and the two nodes it points to.*/
typedef struct track {
    int time;
    int number;
    int nodea, nodeb;
} Track;

/*structure to hold information about a course in the graph.*/
typedef struct course {
    char name;
    int path_size;
    int *nodes;
    Linked_List tracks;
} Course;

/*enum to signify what the current status of an entrant is*/
enum entrant_status {
    NOT_STARTED,
    MC_CHECKPOINT,
    ON_TRACK,
    TIME_CHECKPOINT,
    COMPLETED,
    EXCLUDED_MC,
    EXCLUDED_IR
};

/*Structure to hold information about data for a checkpoint update*/
typedef struct checkpoint_data {
    int competitor;
    int node;
    char type;
    char time[TIME_STRING_SIZE];
} CP_Data;

/*Structure to hold information about an entrants status in the competition*/
typedef struct status {
    enum entrant_status type;
    CP_Data * cp_data;
    int nodes_visited;
    int location_ref; /*i.e. track number, node number etc.*/
    int next_cp;
} Status;

/*Structure to hold information about an entrant in the competition.*/
typedef struct entrant {
    int number; /*competitor number*/
    char course;
    char name[MAX_ENTRANT_NAME_SIZE];
    char start_time[TIME_STRING_SIZE];
    char end_time[TIME_STRING_SIZE];
    char mc_time_stopped[TIME_STRING_SIZE];
    int mc_time_delay_hours, mc_time_delay_mins;
    Status state;
    CP_Data cp_data;
    List_Node *current_track;
} Entrant;

/*structure to hold details about a single event*/
typedef struct event {
    char name[MAX_EVENT_NAME_SIZE];
    char date[MAX_DATE_SIZE];
    char start_time[TIME_STRING_SIZE];
    int num_of_entrants;
    Linked_List nodelist;
    Linked_List tracklist;
    Linked_List courselist;
    Linked_List entrantlist;
    Linked_List cp_data_buff;
} Event;

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTURES_H */

