/* 
 * File:   util.c
 * Author: Samuel Jackson
 *
 * File contains functions used by the main actions that the user can call from
 * the main menu.
 * 
 * Created on 29 November 2012, 21:27
 */

#include <stdlib.h>
#include <string.h>
#include "util.h"

/* Find a track using the two nodes it links to.*/
track * find_track(linked_list list, int node_a, int node_b) {
    list_node * current = list.head;
    track * track_data = NULL;
    int found = 0;
    
    while (!found && current->next != NULL) {
        track_data = (track*) current->data;
        if ((node_a == track_data->nodea && node_b == track_data->nodeb)
            || (node_a == track_data->nodeb && node_b == track_data->nodea)) {
            found = 1;
        }
        current = current->next;
    }
    
    return track_data;
}

/* Find an entrant using the entrants ID number */
entrant * find_entrant(linked_list list, int id) {
    list_node *current = list.head;
    entrant *entrant_data = NULL;
    int found = 0;
    
    while(!found && current->next != NULL) {
        entrant_data = (entrant*) current->data;
        if(entrant_data->number == id) {
            found = 1;
        }
        current = current->next;
    }
    
    return entrant_data;
}

/* Find a course using the courses ID character*/
course * find_course(linked_list list, char c) {
    list_node *current = list.head;
    course * course_data = NULL;
    int found = 0;
    
    while (!found && current->next != NULL){
        course_data = (course *) current->data;
        if(c == course_data->name){
            found = 1;
        }
        current = current->next;
    }
    
    return course_data;
}

/* Find the next checkpoint on the course given the current competitor */
int find_next_checkpoint(linked_list nodes, entrant * e) {
    node *node_data = NULL;
    list_node *current = nodes.head;
    int i, count = 0, found = 0;
    
    i = e->state.nodes_visited+2; /* ID is not zero based, so +2 */
    
    /* Keep looking ahead until we find a checkpoint */
    while(!found && current->next != NULL) {
        count++;
        node_data = (node*) current->data;
        if(count >= i && node_data->type == CP) {
            found = 1;
        }
        current = current->next;
    }
    
    return node_data->num;
}

/* Convert an entrants status into a human readable string */
void convert_type_status(enum entrant_status type, char buffer[OUTPUT_BUFF]) {
    switch(type) {
        case NOT_STARTED:
            strcpy(buffer, "Not yet started.");
            break;
        case MC_CHECKPOINT:
            strcpy(buffer, "At medical checkpoint.");
            break;
        case ON_TRACK:
            strcpy(buffer, "Out on track.");
            break;
        case TIME_CHECKPOINT:
            strcpy(buffer, "At time checkpoint.");
            break;
        case COMPLETED:
            strcpy(buffer, "Completed course.");
            break;
        case EXCLUDED_MC:
            strcpy(buffer, "Excluded at medical checkpoint.");
            break;
        case EXCLUDED_IR:
            strcpy(buffer, "Excluded at incorrect checkpoint.");
            break;
    }
}

/* Function to convert the type of this node to a string */
enum check_point convert_node_type(char type_string[3]){
    enum check_point type;
    if(!strcmp(type_string, "CP")) {
        type = CP;
    } else if (!strcmp(type_string, "JN")) {
        type = JN;
    }
    return type;
}

/* Convert a formatted time string into minutes */
int convert_time_to_mins(char time[TIME_STRING_SIZE]) {
    int hours, mins;

    hours = atoi(time);
    mins = atoi(&time[3]);

    return (hours*60) + mins;
}

/* Calculate the different between the hours or minutes of a formatted string*/
int calc_time_diff (char *start, char *end) {
    int start_t, end_t, total;

    start_t  = atoi(start);
    end_t  = atoi(end);
   
    total = end_t - start_t;
    
    return total;  
}
