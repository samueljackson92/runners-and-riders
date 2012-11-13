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
    
#define MAX_EVENT_NAME_SIZE 50
#define MAX_ENTRANT_NAME_SIZE 50
#define MAX_DATE_SIZE 20
#define TIME_STRING_SIZE 5

enum check_point {
    CP, JN, MC
};

enum boolean {
    TRUE, FALSE
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

/*Structure to hold information about an entrant in the competition.*/
typedef struct entrant {
    int number;
    enum boolean excluded;
    char course;
    char name[MAX_ENTRANT_NAME_SIZE];
} Entrant;

typedef struct checkpoint_data {
    int competitor;
    int node;
    char type;
    char time[TIME_STRING_SIZE];
} CP_Data;

/*structure to hold details about a single event*/
typedef struct event {
    char name[MAX_EVENT_NAME_SIZE];
    char date[MAX_DATE_SIZE];
    char start_time[TIME_STRING_SIZE];
    Node *nodelist;
    Track *tracklist;
    Course *courselist;
    Entrant *entrantlist;
} Event;

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTURES_H */

