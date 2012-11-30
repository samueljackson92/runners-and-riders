/* 
 * File:   util.c
 * Author: Samuel Jackson
 *
 * Created on 29 November 2012, 21:27
 */

#include <stdlib.h>
#include <string.h>
#include "util.h"

void add_new_time(Event *evt, CP_Data data){
    Entrant *entrant = find_entrant(evt->entrantlist, data.competitor);
    Course *course = find_course(evt->courselist, entrant->course);
    int hours, mins;
    
    switch(data.type) {
        /*regular time update*/
        case 'T':
            if(entrant->state.type == NOT_STARTED){
                strcpy(entrant->start_time, data.time);
            }
            
            entrant->state.type = TIME_CHECKPOINT;
            
            while(course->nodes[entrant->state.nodes_visited] != data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = data.node; 
            
            if(entrant->state.nodes_visited == course->path_size-1) {
                entrant->state.type = COMPLETED;
                strcpy(entrant->end_time, data.time);
            }
            
            break;
        /* Excluded at checkpoint for taking wrong direction */
        case 'I':
            entrant->state.type = EXCLUDED_IR;
            entrant->state.location_ref = data.node;
            break;
        /* Arrived at medical checkpoint */
        case 'A':
            entrant->state.type = MC_CHECKPOINT;
            
            while(course->nodes[entrant->state.nodes_visited] != data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = data.node;
            strcpy(entrant->mc_time_stopped, data.time);
            break;
        /* Departed from medical checkpoint */
        case 'D':
            entrant->state.type = ON_TRACK;
            
            hours = calc_time_diff(entrant->mc_time_stopped, data.time);
            mins = calc_time_diff(&entrant->mc_time_stopped[3], &data.time[3]);
            
            entrant->mc_time_delay_hours += hours;
            entrant->mc_time_delay_mins += mins;
            break;
        /* Excluded for failing medical checkpoint */
        case 'E':
            entrant->state.type = EXCLUDED_MC;
            entrant->state.location_ref = data.node;
            break;
    }
 
    entrant->cp_data = data;
}

void update_others(Event *evt, CP_Data data){
    List_Node *current_entrant = evt->entrantlist.head;
    Track *track = NULL;
    Entrant *entrant = NULL;
    enum entrant_status status;
    int check_time, current_time, 
            track_total, hit_cp = 0;
    
    /*Loop through each entrant in the competition*/
    while(current_entrant->next != NULL) {
        entrant = (Entrant *) current_entrant->data;
        status = entrant->state.type;
        
        /*set others to be on track and update there position*/
        if((status == TIME_CHECKPOINT && data.competitor != entrant->number) || status == ON_TRACK) {
            
            if(status == TIME_CHECKPOINT) {
                /*get next checkpoint*/
                entrant->state.next_cp = find_next_checkpoint(evt->nodelist, entrant);
            }

            track  = (Track *) entrant->current_track->data;

            check_time = convert_time_to_mins(entrant->cp_data.time);  /*time at last checkpoint*/
            current_time = convert_time_to_mins(data.time);            /*current time*/
            track_total = track->time;                                 /*time it should take on track*/

            /*Estimate how far between checkpoints an entrant is.*/
            /*fast forward the entrant along the course if applicable*/
            while(!hit_cp && track_total < current_time - check_time) {

                track = (Track*) entrant->current_track->data;

                /* if we should have reached a checkpoint, but are late*/
                if(track->nodea == entrant->state.next_cp || 
                        track->nodeb == entrant->state.next_cp){
                    hit_cp = 1;
                } else {
                    /*if we should not have reached a checkpoint move forward on the track*/
                    entrant->current_track = entrant->current_track->next;
                    track = (Track*) entrant->current_track->data;
                }

                /* accumulate the estimated time travelled along the track*/
                track_total += track->time;
            }
            entrant->state.type = ON_TRACK;
            entrant->state.location_ref = track->number;
        }
        current_entrant = current_entrant->next;
    }
}

Track * find_track(Linked_List list, int node_a, int node_b) {
    List_Node * current = list.head;
    Track * track = NULL;
    int found = 0;
    
    while (!found && current->next != NULL) {
        track = (Track*) current->data;
        if ((node_a == track->nodea && node_b == track->nodeb)
            || (node_a == track->nodeb && node_b == track->nodea)) {
            found = 1;
        }
        current = current->next;
    }
    
    return track;
}

Entrant * find_entrant(Linked_List list, int id) {
    List_Node *current = list.head;
    Entrant *entrant = NULL;
    int found = 0;
    
    while(!found && current->next != NULL) {
        entrant = (Entrant*) current->data;
        if(entrant->number == id) {
            found = 1;
        }
        current = current->next;
    }
    
    return entrant;
}

Course * find_course(Linked_List list, char c) {
    List_Node *current = list.head;
    Course * course = NULL;
    int found = 0;
    
    while (!found && current->next != NULL){
        course = (Course *) current->data;
        if(c == course->name){
            found = 1;
        }
        current = current->next;
    }
    
    return course;
}

int find_next_checkpoint(Linked_List nodes, Entrant * e) {
    Node *node = NULL;
    List_Node *current = nodes.head;
    int i, count = 0, found = 0;
    
    i = e->state.nodes_visited+2;
    
    while (count < i) {
        current = current->next;
        count++;
    }
    
    while(!found && current->next != NULL) {
        node = (Node*) current->data;
        if(node->type == CP) {
            found = 1;
        }
    }
    
    return node->num;
}

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


int convert_time_to_mins(char time[TIME_STRING_SIZE]) {
    int hours, mins;

    hours = atoi(time);
    mins = atoi(&time[3]);

    return (hours*60) + mins;
}

int calc_time_diff (char *start, char *end) {
    int start_t, end_t, total;

    start_t  = atoi(start);
    end_t  = atoi(end);
   
    total = end_t - start_t;
    
    return total;  
}