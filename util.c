/* 
 * File:   util.c
 * Author: Samuel Jackson
 *
 * Created on 29 November 2012, 21:27
 */

#include <stdlib.h>
#include <string.h>
#include "util.h"

void add_new_time(event *evt, CP_Data checkpoint_data){
    entrant *entrant = find_entrant(evt->entrantlist, checkpoint_data.competitor_num);
    course *course = find_course(evt->courselist, entrant->course);
    int hours, mins;
    
    switch(checkpoint_data.type) {
        /*regular time update*/
        case 'T':
            if(entrant->state.type == NOT_STARTED){
                strcpy(entrant->start_time, checkpoint_data.time);
            }
            
            entrant->state.type = TIME_CHECKPOINT;
            
            while(course->nodes[entrant->state.nodes_visited] != checkpoint_data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = checkpoint_data.node; 
            
            if(entrant->state.nodes_visited == course->path_size-1) {
                entrant->state.type = COMPLETED;
                strcpy(entrant->end_time, checkpoint_data.time);
            }
            
            break;
        /* Excluded at checkpoint for taking wrong direction */
        case 'I':
            entrant->state.type = EXCLUDED_IR;
            entrant->state.location_ref = checkpoint_data.node;
            break;
        /* Arrived at medical checkpoint */
        case 'A':
            entrant->state.type = MC_CHECKPOINT;
            
            while(course->nodes[entrant->state.nodes_visited] != checkpoint_data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = checkpoint_data.node;
            strcpy(entrant->mc_time_stopped, checkpoint_data.time);
            break;
        /* Departed from medical checkpoint */
        case 'D':
            entrant->state.type = ON_TRACK;
            
            hours = calc_time_diff(entrant->mc_time_stopped, checkpoint_data.time);
            mins = calc_time_diff(&entrant->mc_time_stopped[3], &checkpoint_data.time[3]);
            
            entrant->mc_time_delay_hours += hours;
            entrant->mc_time_delay_mins += mins;
            break;
        /* Excluded for failing medical checkpoint */
        case 'E':
            entrant->state.type = EXCLUDED_MC;
            entrant->state.location_ref = checkpoint_data.node;
            break;
    }
 
    entrant->state.current_cp_data = &checkpoint_data;
}

void update_others(event *evt, CP_Data checkpoint_data){
    List_Node *current_entrant = evt->entrantlist.head;
    entrant *entrant_data = NULL;
    enum entrant_status status;
    
    /*Loop through each entrant in the competition*/
    while(current_entrant->next != NULL) {
        entrant_data = (entrant*) current_entrant->data;
        status = entrant_data->state.type;
        
        /*set others to be on track and update there position*/
        if((status == TIME_CHECKPOINT && checkpoint_data.competitor_num != entrant_data->number) || status == ON_TRACK) {
            update_entrant_on_track(evt, entrant_data, checkpoint_data);
        }
        
        current_entrant = current_entrant->next;
    }
}

void update_entrant_on_track(event *evt, entrant *entrant, CP_Data checkpoint_data) {
    int check_time, current_time, 
        track_total, hit_cp = 0;
    
    track *track_data=  (track *) entrant->current_track->data;
    
    if(entrant->state.type == TIME_CHECKPOINT) {
        /*get next checkpoint*/
        entrant->state.next_cp = find_next_checkpoint(evt->nodelist, entrant);
    }

    check_time = convert_time_to_mins(entrant->state.current_cp_data->time);  /*time at last checkpoint*/
    current_time = convert_time_to_mins(checkpoint_data.time);            /*current time*/
    track_total = track_data->time;                                 /*time it should take on track*/

    /*Estimate how far between checkpoints an entrant is.*/
    /*fast forward the entrant along the course if applicable*/
    while(!hit_cp && track_total < current_time - check_time) {

        track_data = (track*) entrant->current_track->data;

        /* if we should have reached a checkpoint, but are late*/
        if(track_data->nodea == entrant->state.next_cp || 
                track_data->nodeb == entrant->state.next_cp){
            hit_cp = 1;
        } else {
            /*if we should not have reached a checkpoint move forward on the track*/
            entrant->current_track = entrant->current_track->next;
            track_data = (track*) entrant->current_track->data;
        }

        /* accumulate the estimated time travelled along the track*/
        track_total += track_data->time;
    }
    entrant->state.type = ON_TRACK;
    entrant->state.late = hit_cp;
    entrant->state.location_ref = track_data->number;
}

track * find_track(Linked_List list, int node_a, int node_b) {
    List_Node * current = list.head;
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

entrant * find_entrant(Linked_List list, int id) {
    List_Node *current = list.head;
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

course * find_course(Linked_List list, char c) {
    List_Node *current = list.head;
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

int find_next_checkpoint(Linked_List nodes, entrant * e) {
    node *node_data = NULL;
    List_Node *current = nodes.head;
    int i, count = 0, found = 0;
    
    i = e->state.nodes_visited+2;
    
    while (count < i) {
        current = current->next;
        count++;
    }
    
    while(!found && current->next != NULL) {
        node_data = (node*) current->data;
        if(node_data->type == CP) {
            found = 1;
        }
    }
    
    return node_data->num;
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