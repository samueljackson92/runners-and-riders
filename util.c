/* 
 * File:   util.c
 * Author: Samuel Jackson
 *
 * Created on 29 November 2012, 21:27
 */

#include <stdlib.h>
#include <string.h>
#include "util.h"

void add_new_time(Event *e, CP_Data data){
    Entrant *entrant = find_entrant(e->entrantlist, data.competitor);
       
    List_Node *current = e->courselist.head;
    Course *c;
    
    int hours, mins;
    
    while(current->next != NULL) {
        c = (Course *) current->data;
        if (c->name == entrant->course) {
            break;
        }
        current = current->next;
    }
    
    switch(data.type) {
        /*regular time update*/
        case 'T':
            if(entrant->state.type == NOT_STARTED){
                strcpy(entrant->start_time, data.time);
            }
            
            entrant->state.type = TIME_CHECKPOINT;
            
            while(c->nodes[entrant->state.nodes_visited] != data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = data.node; 
            
            if(entrant->state.nodes_visited == c->path_size-1) {
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
            
            while(c->nodes[entrant->state.nodes_visited] != data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = data.node;
            strcpy(entrant->mc_time_stopped, data.time);
            break;
        /* Departed from medical checkpoint */
        case 'D':
            hours = calc_time_diff(entrant->mc_time_stopped, data.time);
            mins = calc_time_diff(&entrant->mc_time_stopped[3], &data.time[3]);
            
            entrant->mc_time_delay_hours += hours;
            entrant->mc_time_delay_mins += mins;
            
            entrant->state.type = ON_TRACK;
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
    Track *track;
    Entrant *entrant;
    enum type_status status;
    int check_time, current_time, 
            track_total, hit_cp = 0;
    
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
                
                check_time = convert_time_to_mins(entrant->cp_data.time); /*time at last checkpoint*/
                current_time = convert_time_to_mins(data.time);     /*current time*/
                track_total = track->time;                          /*time it should take on track*/
                
                /*fast forward the entrant along the course*/
                while(!hit_cp && track_total < current_time - check_time) {
                    
                    track = (Track*) entrant->current_track->data;
                    
                    /* if we should have reached a checkpoint*/
                    if(track->nodea == entrant->state.next_cp || 
                            track->nodeb == entrant->state.next_cp){
                        hit_cp = 1;
                    } else {
                        entrant->current_track = entrant->current_track->next;
                        track = (Track*) entrant->current_track->data;
                    }
                    
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
    Track * current_track;
    int found =0;
    
    while (!found && current->next != NULL) {
        current_track = (Track*) current->data;
        if ((node_a == current_track->nodea && node_b == current_track->nodeb)
            || (node_a == current_track->nodeb && node_b == current_track->nodea)) {
            found = 1;
        }
        current = current->next;
    }
    
    return current_track;
}

Entrant * find_entrant(Linked_List list, int id) {
    List_Node *current = list.head;
    Entrant *entrant;
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

Course * findCourse(Linked_List *list, char c) {
    List_Node *current = list->head;
    Course * current_course;
    int found = 0;
    
    while (!found && current->next != NULL){
        current_course = (Course *) current->data;
        if(c == current_course->name){
            found = 1;
        }
        current = current->next;
    }
    
    return current_course;
}

Node * find_node(Linked_List list, int id) {
    List_Node *current = list.head;
    Node *node;
    int found = 0;
    
    while (!found && current->next != NULL) {
        node = (Node*) current->data;
        if(node->num == id) {
            found = 1;
        }
        current = current->next;
    }
    
    return node;
}

int find_next_checkpoint(Linked_List nodes, Entrant * e) {
    Node *node;
    int i;
    
    i = e->state.nodes_visited+2;
    node = find_node(nodes, i);
    while(node->type != CP) {
        i++;
        node = find_node(nodes, i);
    }
    
    return node->num;
}

void convert_type_status(enum type_status type, char buffer[OUTPUT_BUFF]) {
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