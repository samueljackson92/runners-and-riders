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
    event evt;
    int option, result;
    
    printf("|==============================================|\n");
    printf("|--------------RUNNERS AND RIDERS--------------|\n");
    printf("|----------------------------------------------|\n");
    printf("|----------Event tracking application----------|\n");
    printf("|==============================================|\n");
    
    printf("\nLoading data files...\n");
    
    /*init all linked lists to NULL for safety*/
    evt.courselist.head = NULL;
    evt.courselist.tail = NULL;
    evt.entrantlist.head = NULL;
    evt.entrantlist.tail = NULL;
    evt.nodelist.head = NULL;
    evt.nodelist.tail = NULL;
    evt.tracklist.head = NULL;
    evt.tracklist.tail = NULL;
    
    read_file_data(&evt); /*read data files*/
    
    /*begin main menu*/
    do {
        printf("Enter an Option:\n"
                "0  - Exit\n"
                "1  - Event Details\n"
                "2  - Query Competitor\n"
                "3  - Check how many competitors not yet started\n"
                "4  - Check how many competitors are out on courses\n"
                "5  - Check how many competitors have finished\n"
                "6  - Manually update a competitor\n"
                "7  - Read in a file of updates\n"
                "8  - Print table of results\n"
                "9  - Print entrants excluded at medical checkpoints\n"
                "10 - Print entrants excluded at regular checkpoints\n");
        
        scanf(" %d", &option);
        clear_screen();
        
        switch(option) {
            case 0: break;
            case 1:
                print_event_data(&evt);
                break;
            case 2: /*query a competitor based on there id*/
                query_competitor(evt.entrantlist);
                break;
            case 3: /* Check the number of competitors not yet started*/
                result = check_num_competitors(evt.entrantlist, NOT_STARTED);
                printf("%d competitors have not yet started\n", result);
                break;
            case 4: /* Check the number of competitors currently out on tracks */
                result = check_num_competitors(evt.entrantlist, ON_TRACK);
                printf("%d competitors are out on a course\n", result);
                break;
            case 5: /* Check the number of competitors who have completed the course*/
                result = check_num_competitors(evt.entrantlist, COMPLETED);
                printf("%d competitors have completed their course\n", result);
                break;
            case 6: /* Manually enter a time checkpoint update */
                manually_read_data(&evt);
                break;
            case 7: /* Read a file of time checkpoint updates */
                read_updates(&evt);
                break;
            case 8: /* Print out a table of results */
                print_results(evt.entrantlist);
                break;
            case 9: /* Print a table of entrants who have been excluded at medical checkpoints*/
                print_entrants_excluded(evt.entrantlist, EXCLUDED_MC);
                break;
            case 10: /* Print a table of entrants who have been excluded for getting lost */
                print_entrants_excluded(evt.entrantlist, EXCLUDED_IR);
                break;
            default:
                printf("That was not an option!");
                break;
        }
    } while (option != 0);
    
    return (EXIT_SUCCESS);
}

/*Print out the details about this event.*/
void print_event_data(event *evt) {
    printf("%s\n", evt->name);
    printf("%s\n", evt->date);
    printf("Start Time: %s\n", evt->start_time);
}

/* Query a competitor by supplying a competitor ID */
void query_competitor(linked_list entrantlist) {
    int entrant_id;
    track *track_data;
    entrant *entrant;
    char status_buff[OUTPUT_BUFF];
    
    /* get competitor id from user */
    printf("Enter id for the competitor:\n");
    scanf(" %d", &entrant_id);
    
    clear_screen();
    
    entrant = find_entrant(entrantlist, entrant_id);
    convert_type_status_verbose(entrant->state.type, status_buff);
    
    /*output the current status of a competitor */
    
    printf("COMPETITOR %d:\n", entrant_id);
    printf("Name: %s\n", entrant->name);
    printf("Status: %s\n", status_buff);
    
    switch(entrant->state.type) {
        case ON_TRACK: /*Output for if they are presumed on a track */
            track_data = (track*) entrant->current_track->data;
            printf("Last recorded time: %s\n", entrant->state.current_cp_data.time);
            if(entrant->state.late) {
                printf("This entrant is currently running late to the next checkpoint\n");
            }
            printf("Last checkpoint visited: %d\n", entrant->state.current_cp_data.node);
            printf("Track Reference No.: %d\n", entrant->state.location_ref);
            printf("Presumed on track between node %d and node %d\n", track_data->nodea, track_data->nodeb);
            break;
        default: /*output if they are at a checkpoint or have been excluded */
            printf("Currently at node: %d\n", entrant->state.location_ref);
            break;
    }
}

/* Check the number of competitors with a given entrant status e.g. ON_TRACK */
int check_num_competitors(linked_list entrantlist, enum entrant_status type) {
    int count = 0;
    list_node *current = entrantlist.head;
    entrant *entrant_data;
    
    /* loop through each entrant and check their state.
     * Display if equal.
     */
    while (current != NULL) {
        entrant_data = (entrant*) current->data;
        if (entrant_data->state.type == type) {
            count++;
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
    int status = 4, count = 0;
    
    printf("Enter name of the checkpoint files:\n");
    scanf(" %100s", filename);
    
    FILE *file = fopen(filename, "r");
    
    if(file != NULL) {
        /* Read in an update and add it to the system. */
        while (!feof(file) && status == 4){
            status = fscanf(file, "%c %d %d %5[0-9:]\n", &checkpoint_data.type, &checkpoint_data.node, 
                    &checkpoint_data.competitor_num, checkpoint_data.time);
            if(status == 4) {
                add_new_time(e, checkpoint_data);
            } else {
                printf("Parse error on line %d. Stopping read.\n", count);
            }
            count++;
        }
        fclose(file);
    
        /* Update the rest of the competitors' positions relative to the most recent time. */
        update_others(e, checkpoint_data);
    } else {
        printf("Error reading file!");
        exit(0);
    }

}

/* Function defines how to print an entrant in a format that fits the results table. */
void print_entrant(void *data) {
    entrant *entrant_data = (entrant*) data;
    int total_time_mins = 0;
    int hours = 0, mins = 0;
    int delay_hours, delay_mins;
    
    char status_buff[OUTPUT_BUFF];
    char start_time[TIME_STRING_SIZE];
    char end_time[TIME_STRING_SIZE];
    char delay_time[TIME_STRING_SIZE];
        
    convert_mins_to_time(entrant_data->start_time, start_time);
    convert_type_status(entrant_data->state.type, status_buff);
    
   /* Check whether the entrant has completed the course yet. If so then calculate their finish time. */
    if(entrant_data->state.type == COMPLETED) {
        
        total_time_mins = entrant_data->end_time - entrant_data->start_time;
        total_time_mins -= entrant_data->mc_time_delay;

        hours = total_time_mins /60;
        mins = total_time_mins %60;
        
        delay_hours = entrant_data->mc_time_delay /60;
        delay_mins = entrant_data->mc_time_delay %60;

        convert_mins_to_time(entrant_data->end_time, end_time);
        convert_mins_to_time(entrant_data->mc_time_delay, delay_time);


        printf("|%-21s|    %c     |%-16s|    %s    |    %s    |  %.2dhrs %.2dmins  |  %.2dhrs %.2dmins  |\n", 
                entrant_data->name, entrant_data->course, 
                status_buff, 
                start_time, end_time, 
                delay_hours, delay_mins,
                hours, mins);
            
    } else {
        printf("|%-21s|    %c     |%-16s|    %s    |     N/a     |       N/a      |       N/a      |\n", 
                entrant_data->name, entrant_data->course, 
                status_buff, start_time);
    }
}

/* Print each entrants results along with the table header/footer. */
void print_results(linked_list entrantlist){
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    printf("|Competitor           |  Course  |     Status     |  Start Time |   End Time  |    MC Delay    |     Total      |\n");
    printf("|---------------------------------------------------------------------------------------------------------------|\n");
    
    traverse_list(entrantlist.head, &print_entrant);
    
    printf("-----------------------------------------------------------------------------------------------------------------\n");
}

/* Print a table of entrant which have been excluded from the event. */
void print_entrants_excluded(linked_list entrantlist, enum entrant_status type) {    
    list_node *current = entrantlist.head;
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
                entrant_data->state.location_ref, entrant_data->state.current_cp_data.time);
        }
        current = current->next;
    }
    
    printf("------------------------------------------\n");
}

/* Add a new checkpoint time update to the system. */
void add_new_time(event *evt, CP_Data checkpoint_data){
    track *current_track;
    entrant *entrant_data = find_entrant(evt->entrantlist, checkpoint_data.competitor_num);
    course *course_data = find_course(evt->courselist, entrant_data->course);
    int found = 0;
    
    /* Select what time of checkpoint we're dealing with. */
    switch(checkpoint_data.type) {
        /*regular time update*/
        case 'T':
            /* if this is the first updated, set their start time. */
            if(entrant_data->state.type == NOT_STARTED){
                entrant_data->start_time = convert_time_to_mins(checkpoint_data.time);
            }
            
            entrant_data->state.type = TIME_CHECKPOINT;
            
            /* update the number of nodes this competitor has visited */
            while(course_data->nodes[entrant_data->state.nodes_visited] != checkpoint_data.node) {
                entrant_data->state.nodes_visited++;
            }
            
            entrant_data->state.location_ref = checkpoint_data.node; 

            /* if this is the last node on the course,
             * the competitor has finished */
            if(entrant_data->state.nodes_visited >= course_data->path_size-1) {
                entrant_data->state.type = COMPLETED;
                entrant_data->end_time = convert_time_to_mins(checkpoint_data.time);
            } else {
                while (!found && entrant_data->current_track->next != NULL) {
                    current_track = (track*) entrant_data->current_track->data;
                    if((current_track->nodea == checkpoint_data.node 
                            && current_track->nodeb == course_data->nodes[entrant_data->state.nodes_visited+1])
                            || (current_track->nodeb == checkpoint_data.node 
                            && current_track->nodea == course_data->nodes[entrant_data->state.nodes_visited+1])){
                        found = 1;
                    } else {
                         entrant_data->current_track = entrant_data->current_track->next;
                    }
                }
            }
            
            break;
        /* Excluded at checkpoint for taking wrong direction */
        case 'I':
            entrant_data->state.type = EXCLUDED_IR;
            entrant_data->state.location_ref = checkpoint_data.node;
            break;
        /* Arrived at medical checkpoint */
        case 'A':
            entrant_data->state.type = MC_CHECKPOINT;
            
            /* update the number of nodes this competitor has visited */
            while(course_data->nodes[entrant_data->state.nodes_visited] != checkpoint_data.node) {
                entrant_data->state.nodes_visited++;
            }
            
            entrant_data->state.location_ref = checkpoint_data.node;
            
            /* record the time they reached the MC for delay calculations*/
            entrant_data->mc_time_stopped = convert_time_to_mins(checkpoint_data.time);
            break;
        /* Departed from medical checkpoint */
        case 'D':
            /* calculate the time spent a the checkpoint and record the delay accordingly*/
            entrant_data->mc_time_delay += convert_time_to_mins(checkpoint_data.time) - entrant_data->mc_time_stopped;
            break;
        /* Excluded for failing medical checkpoint */
        case 'E':
            entrant_data->state.type = EXCLUDED_MC;
            entrant_data->state.location_ref = checkpoint_data.node;
            break;
    }
 
    entrant_data->state.current_cp_data = checkpoint_data;
}

/* Update other competitors with estimations of where they are. */
void update_others(event *evt, CP_Data checkpoint_data){
    list_node *current_entrant = evt->entrantlist.head;
    entrant *entrant_data = NULL;
    enum entrant_status status;
    
    /*Loop through each entrant in the competition*/
    while(current_entrant != NULL) {
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
void update_entrant_on_track(event *evt, entrant *entrant_data, CP_Data checkpoint_data) {
    int check_time, current_time, 
        track_total, hit_cp = 0;
    
    track *track_data = (track *) entrant_data->current_track->data;
    
    if(entrant_data->state.type == TIME_CHECKPOINT) {
        /*get next checkpoint*/
        entrant_data->state.next_cp = find_next_checkpoint(evt->nodelist, entrant_data);
    }

    check_time = convert_time_to_mins(entrant_data->state.current_cp_data.time);  /*time at last checkpoint*/
    current_time = convert_time_to_mins(checkpoint_data.time);            /*current time*/
    track_total = track_data->time;                                 /*time it should take on track*/

    /*Estimate how far between checkpoints an entrant is.*/
    /*fast forward the entrant along the course if applicable*/
    while(!hit_cp && track_total < current_time - check_time) {

        track_data = (track*) entrant_data->current_track->data;

        /* if we should have reached a checkpoint, but are late*/
        if(track_data->nodea == entrant_data->state.next_cp || 
                track_data->nodeb == entrant_data->state.next_cp){
            hit_cp = 1;
        } else {
            /*if we should not have reached a checkpoint move forward on the track*/
            entrant_data->current_track = entrant_data->current_track->next;
            track_data = (track*) entrant_data->current_track->data;
        }

        /* accumulate the estimated time travelled along the track*/
        track_total += track_data->time;
    }
    entrant_data->state.type = ON_TRACK;
    entrant_data->state.late = hit_cp;
    entrant_data->state.location_ref = track_data->number;
}