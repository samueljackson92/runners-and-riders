/* 
 * File:   structures.h
 * Author: samuel
 *
 * Created on 13 November 2012, 10:04
 */

#ifndef STRUCTURES_H
#define	STRUCTURES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "linked_list.h"
    
#define MAX_EVENT_NAME_SIZE 50
#define MAX_ENTRANT_NAME_SIZE 50
#define MAX_DATE_SIZE 20
#define TIME_STRING_SIZE 6

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
} Course;

enum type_status {
    NOT_STARTED,
    MC_CHECKPOINT,
    ON_TRACK,
    TIME_CHECKPOINT,
    COMPLETED,
    EXCLUDED_MC,
    EXCLUDED_IR
};

typedef struct checkpoint_data {
    int competitor;
    int node;
    char type;
    char time[TIME_STRING_SIZE];
} CP_Data;

/*Structure to hold information about an entrants status in the competition*/
typedef struct status {
    enum type_status type;
    CP_Data * cp_data;
    int nodes_visited;
    int location_ref; /*i.e. track number, node number etc.*/
} Status;

/*Structure to hold information about an entrant in the competition.*/
typedef struct entrant {
    int number; /*competitor number*/
    char course;
    char name[MAX_ENTRANT_NAME_SIZE];
    char start_time[TIME_STRING_SIZE];
    char end_time[TIME_STRING_SIZE];
    Status state;
    CP_Data cp_data;
} Entrant;

/*structure to hold details about a single event*/
typedef struct event {
    char name[MAX_EVENT_NAME_SIZE];
    char date[MAX_DATE_SIZE];
    char start_time[TIME_STRING_SIZE];
    int no_of_entrants;
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

