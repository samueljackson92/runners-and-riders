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
#define TIME_STRING_SIZE 5

enum check_point {
    CP, JN, MC
};

typedef struct neighbour_node {
    int node;
    struct neighbour_node *next;
}Neighbour_node;

/*structure to hold information about a node and a pointer to the next node*/
typedef struct node {
    enum check_point type;
    int num;
    Neighbour_node *neighbours;
} Node;

/*structure to hold information about a track and the two nodes it points to.*/
typedef struct track {
    int time;
    int number;
    int nodea, nodeb;
    struct track *next;
} Track;

/*structure to hold information about a course in the graph.*/
typedef struct course {
    char name;
    int path_size;
    int *nodes;
    struct course *next;
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
    struct checkpoint_data *next;
} CP_Data;

/*Structure to hold information about an entrants status in the competition*/
typedef struct status {
    enum type_status type;
    CP_Data * cp_data;
    int location_ref; /*i.e. track number, node number etc.*/
} Status;

/*Structure to hold information about an entrant in the competition.*/
typedef struct entrant {
    int number;
    char course;
    char name[MAX_ENTRANT_NAME_SIZE];
    Status state;
    struct entrant *next;
} Entrant;

/*structure to hold details about a single event*/
typedef struct event {
    char name[MAX_EVENT_NAME_SIZE];
    char date[MAX_DATE_SIZE];
    char start_time[TIME_STRING_SIZE];
    int no_of_entrants;
    List_Node *nodelist;
    List_Node *tracklist;
    List_Node *courselist;
    List_Node *entrantlist;
} Event;

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTURES_H */

