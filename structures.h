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
#define MAX_EVENT_NAME_SIZE 80
    
/*Macro defining the maximum size of an entrant name.*/
#define MAX_ENTRANT_NAME_SIZE 51

/*Maximum size of a textual date string (e.g. 12th June 2012)*/
#define MAX_DATE_SIZE 80
    
/*Maximum size of time string (e.g. 09:00)*/
#define TIME_STRING_SIZE 6
    
/*enum to signify what type of checkpoint a node is*/
enum check_point {
    CP, JN, MC
};

/*structure to hold information about a node and a pointer to the next node*/
typedef struct node_s {
    enum check_point type;
    int num;
} node;

/*structure to hold information about a track and the two nodes it points to.*/
typedef struct track_s {
    int time;
    int number;
    int nodea, nodeb;
} track;

/*structure to hold information about a course in the graph.*/
typedef struct course_s {
    char name;
    int path_size;
    int *nodes;
    linked_list tracks;
} course;

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
    int competitor_num;
    int node;
    char type; /*type of update T/A/D/I/E*/
    char time[TIME_STRING_SIZE];
} CP_Data;

/*Structure to hold information about an entrants status in the competition*/
typedef struct status_s {
    enum entrant_status type; /*TIME_CHECKPOINT, ON_TRACK etc. */
    CP_Data current_cp_data;
    int nodes_visited;
    int location_ref; /*i.e. track number, node number etc.*/
    int next_cp; /*id of next checkpoint*/
    int late; /*are we late to the next checkpoint?*/
} status_struct;

/*Structure to hold information about an entrant in the competition.*/
typedef struct entrant_s {
    int number; /*competitor number*/
    char course;
    char name[MAX_ENTRANT_NAME_SIZE];
    int start_time;
    int end_time;
    int mc_time_stopped; /*time entrant arrived at a MC*/
    int mc_time_delay;/*cumulative delays occurred at MCs*/
    status_struct state; /*detailed state of the entrant*/
    list_node *current_track;
} entrant;

/*structure to hold details about a single event*/
typedef struct event_s {
    char name[MAX_EVENT_NAME_SIZE];
    char date[MAX_DATE_SIZE];
    char start_time[TIME_STRING_SIZE];
    int num_of_entrants;
    
    /*linked lists of all data input from files*/
    linked_list nodelist;
    linked_list tracklist;
    linked_list courselist;
    linked_list entrantlist;
} event;

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTURES_H */

