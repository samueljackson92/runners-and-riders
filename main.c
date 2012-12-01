/* 
 * File:   main.c
 * Author: Samuel Jackson
 * 
 * File contains the main function along with functions for 
 * most of the major requirements of the assignment.
 * 
 * Created on 11 November 2012, 14:31
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "structures.h"
#include "fileio.h"
#include "util.h"

int main(int argc, char** argv) {
    event event;
    int option, result;
    
    /*init all linked lists to NULL for safety*/
    event.courselist.head = NULL;
    event.courselist.tail = NULL;
    event.entrantlist.head = NULL;
    event.entrantlist.tail = NULL;
    event.nodelist.head = NULL;
    event.nodelist.tail = NULL;
    event.tracklist.head = NULL;
    event.tracklist.tail = NULL;
    
    read_file_data(&event); /*read data files*/
    
    /*begin main menu*/
    do {
        printf("Enter an Option:\n"
                "0 - Exit\n"
                "1 - Query Competitor\n"
                "2 - Check how many competitors not yet started\n"
                "3 - Check how many competitors are out on courses\n"
                "4 - Check how many competitors have finished\n"
                "5 - Manually update a competitor\n"
                "6 - Read in a file of updates\n"
                "7 - Print table of results\n"
                "8 - Print entrants excluded at medical checkpoints\n"
                "9 - Print entrants excluded at regular checkpoints\n");
        
        scanf(" %d", &option);
        clear_screen();
        
        switch(option) {
            case 1: /*query a competitor based on there id*/
                query_competitor(event.entrantlist);
                break;
            case 2: /* Check the number of competitors not yet started*/
                result = check_num_competitors(event.entrantlist, NOT_STARTED);
                printf("%d competitors have not yet started\n", result);
                break;
            case 3: /* Check the number of competitors currently out on tracks */
                result = check_num_competitors(event.entrantlist, ON_TRACK);
                printf("%d competitors are out on a course\n", result);
                break;
            case 4: /* Check the number of competitors who have completed the course*/
                result = check_num_competitors(event.entrantlist, COMPLETED);
                printf("%d competitors have completed their course\n", result);
                break;
            case 5: /* Manually enter a time checkpoint update */
                manually_read_data(&event);
                break;
            case 6: /* Read a file of time checkpoint updates */
                read_updates(&event);
                break;
            case 7: /* Print out a table of results */
                print_results(event.entrantlist);
                break;
            case 8: /* Print a table of entrants who have been excluded at medical checkpoints*/
                print_entrants_excluded(event.entrantlist, EXCLUDED_MC);
                break;
            case 9: /* Print a table of entrants who have been excluded for getting lost */
                print_entrants_excluded(event.entrantlist, EXCLUDED_IR);
                break;
        }
    } while (option != 0);
    
    return (EXIT_SUCCESS);
}

/* Query a competitor by supplying a competitor ID */
void query_competitor(Linked_List entrantlist) {
    int entrant_id;
    track *track_data;
    entrant *entrant;
    char status_buff[OUTPUT_BUFF];
    
    /* get competitor id from user */
    printf("Enter id for the competitor:\n");
    scanf("%d", &entrant_id);
    
    clear_screen();
    
    entrant = find_entrant(entrantlist, entrant_id);
    convert_type_status(entrant->state.type, status_buff);
    
    /*output the current status of a competitor */
    
    printf("COMPETITOR %d:\n", entrant_id);
    printf("Name: %s\n", entrant->name);
    printf("Status: %s\n", status_buff);
    
    switch(entrant->state.type) {
        case ON_TRACK: /*Output for if they are presumed on a track */
            track_data = (track*) entrant->current_track->data;
            printf("Last recorded time: %s\n", entrant->state.current_cp_data->time);
            if(entrant->state.late) {
                printf("This entrant is currently running late to the next checkpoint\n");
            }
            printf("Location Reference: %d\n", entrant->state.location_ref);
            printf("Presumed on track between node %d and node %d\n", track_data->nodea, track_data->nodeb);
            break;
        default: /*output if they are at a checkpoint or have been excluded */
            printf("Currently at node: %d\n", entrant->state.location_ref);
            break;
    }
}

/* Check the number of competitors with a given entrant status e.g. ON_TRACK */
int check_num_competitors(Linked_List entrantlist, enum entrant_status type) {
    int count = 0;
    List_Node *current = entrantlist.head;
    entrant *entrant_data;
    
    /* loop through each entrant and check their state.
     * Display if equal.
     */
    while (current->next != NULL) {
        entrant_data = (entrant*) current->data;
        if (entrant_data->state.type == type) {
            count++;
            printf("%s\n", entrant_data->name);
        }
        current = current->next;
    }
    
    return count;
}

/* Let the user manually input a checkpoint update*/
void manually_read_data(event *evt) {
    CP_Data checkpoint_data;
    
    printf("Enter the type of check point (T|I|A|D|E):\n");
    scanf(" %[TIADE]c", &checkpoint_data.type);
    
    printf("Enter the competitor number:\n");
    scanf(" %d", &checkpoint_data.competitor_num);
    
    printf("Enter the check point number:\n");
    scanf(" %d", &checkpoint_data.node);
    
    printf("Enter the time recorded:\n");
    scanf(" %[0-9:]s", checkpoint_data.time);

    add_new_time(evt, checkpoint_data);
    update_others(evt, checkpoint_data);
}

/* Read in a file of updates and add them into the system. */
void read_updates(event *e) {
    char filename[MAX_FILEPATH_LENGTH];
    CP_Data checkpoint_data;
    
    printf("Enter name of the checkpoint files:\n");
    scanf(" %s", filename);
    
    FILE *file = fopen(filename, "r");
    
    /* Read in an update and add it to the system. */
    while (!feof(file)){
        fscanf(file, "%c %d %d %5[0-9:]s", &checkpoint_data.type, &checkpoint_data.node, 
                &checkpoint_data.competitor_num, checkpoint_data.time);
        add_new_time(e, checkpoint_data);
    }
    
    fclose(file);
    
    /* Update the rest of the competitors' positions relative to the most recent time. */
    update_others(e, checkpoint_data);
}

/* Function defines how to print an entrant in a format that fits the results table. */
void print_entrant(void *data) {
    entrant *entrant_data = (entrant*) data;
    int total_hours = 0;
    int total_mins = 0;
        
   /* Check whether the entrant has completed the course yet. If so then calculate their finish time. */
    if(entrant_data->state.type == COMPLETED) {
            total_hours = calc_time_diff(entrant_data->start_time, entrant_data->end_time);
            total_mins = calc_time_diff(&entrant_data->start_time[3], &entrant_data->start_time[3]);

            total_hours -= entrant_data->mc_time_delay_hours;
            total_mins -= entrant_data->mc_time_delay_mins;
    }

    /* Print the entrant in a formatted way.*/
    printf("|%-21s|    %c     |    %s    |    %s    |  %.2dhrs %.2dmins  |\n", 
            entrant_data->name, entrant_data->course, 
            entrant_data->start_time, entrant_data->end_time, 
            total_hours, total_mins);
}

/* Print each entrants results along with the table header/footer. */
void print_results(Linked_List entrantlist){
    printf("-------------------------------------------------------------------------------\n");
    printf("|Competitor           |  Course  |  Start Time |   End Time  |     Total      |\n");
    printf("|-----------------------------------------------------------------------------|\n");
    
    traverse_list(entrantlist.head, &print_entrant);
    
    printf("-------------------------------------------------------------------------------\n");
}

/* Print a table of entrant which have been excluded from the event. */
void print_entrants_excluded(Linked_List entrantlist, enum entrant_status type) {    
    List_Node *current = entrantlist.head;
    entrant *entrant_data;
    
    /* check what type of exclusion we're dealing with */
    if(type == EXCLUDED_MC) {
        printf("Competitors Excluded from Medical Checkpoints\n");
    } else {
        printf("Competitors Excluded from Regular Checkpoints\n");
    }
    
    /*output table header*/
    printf("------------------------------------------\n");
    printf("|Competitor           |  Node  |  Time   |\n");
    printf("|----------------------------------------|\n");
    
    /* For each entrant, if they are excluded, print them*/
    while (current->next != NULL) {
        entrant_data = (entrant*) current->data;
        if(entrant_data->state.type == type) {
            printf("|%-21s|   %.2d   |  %s  |\n", entrant_data->name, 
                entrant_data->state.location_ref, entrant_data->state.current_cp_data->time);
        }
        current = current->next;
    }
    
    printf("------------------------------------------\n");
}

/* Add a new checkpoint time update to the system. */
void add_new_time(event *evt, CP_Data checkpoint_data){
    entrant *entrant = find_entrant(evt->entrantlist, checkpoint_data.competitor_num);
    course *course = find_course(evt->courselist, entrant->course);
    int hours, mins;
    
    /* Select what time of checkpoint we're dealing with. */
    switch(checkpoint_data.type) {
        /*regular time update*/
        case 'T':
            /* if this is the first updated, set their start time. */
            if(entrant->state.type == NOT_STARTED){
                strcpy(entrant->start_time, checkpoint_data.time);
            }
            
            entrant->state.type = TIME_CHECKPOINT;
            
            /* update the number of nodes this competitor has visited */
            while(course->nodes[entrant->state.nodes_visited] != checkpoint_data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = checkpoint_data.node; 
            
            /* if this is the last node on the course,
             * the competitors has finished */
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
            
            /* update the number of nodes this competitor has visited */
            while(course->nodes[entrant->state.nodes_visited] != checkpoint_data.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = checkpoint_data.node;
            
            /* record the time they reached the MC for delay calculations*/
            strcpy(entrant->mc_time_stopped, checkpoint_data.time);
            break;
        /* Departed from medical checkpoint */
        case 'D':
            entrant->state.type = ON_TRACK;
            
            /* calculate the time spent a the checkpoint and record the delay accordingly*/
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

/* Update other competitors with estimations of where they are. */
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

/* Calculate a new estimate of where the entrant might be between two checkpoints */
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