/* 
 * File:   main.c
 * Author: Samuel Jackson
 *
 * Created on 11 November 2012, 14:31
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "main.h"
#include "fileio.h"


int main(int argc, char** argv) {
    int option, result;
    
    Event *e = malloc(sizeof(Event));
    
    read_file_data(e);
    
    puts("Event data loaded!");
    
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
        
        scanf("%d", &option);
        clearScreen();
        switch(option) {
            case 1:
                queryCompetitor(e);
                break;
            case 2:
                result = check_num_competitors(&e->entrantlist, NOT_STARTED);
                printf("%d competitors have not yet started\n", result);
                break;
            case 3:
                result = check_num_competitors(&e->entrantlist, ON_TRACK);
                printf("%d competitors are out on a course\n", result);
                break;
            case 4:
                result = check_num_competitors(&e->entrantlist, COMPLETED);
                printf("%d competitors have completed their course\n", result);
                break;
            case 5:
                manually_read_data(e);
                break;
            case 6:
                read_updates(e);
                break;
            case 7:
                print_results(e);
                break;
            case 8:
                print_entrants_excluded(e, EXCLUDED_MC);
                break;
            case 9:
                print_entrants_excluded(e, EXCLUDED_IR);
                break;
        }
    } while (option != 0);
    
    free(e);
    return (EXIT_SUCCESS);
}

void clearScreen() {
    if(system("clear")) {
        system("cls");
    }
}

char * convert_type_status(enum type_status type) {
    char output[50];
    switch(type) {
        case NOT_STARTED:
            strcpy(output, "Not yet started.");
            break;
        case MC_CHECKPOINT:
            strcpy(output, "At medical checkpoint.");
            break;
        case ON_TRACK:
            strcpy(output, "Out on track.");
            break;
        case TIME_CHECKPOINT:
            strcpy(output, "At time checkpoint.");
            break;
        case COMPLETED:
            strcpy(output, "Completed course.");
            break;
        case EXCLUDED_MC:
            strcpy(output, "Excluded at medical checkpoint.");
            break;
        case EXCLUDED_IR:
            strcpy(output, "Excluded at incorrect checkpoint.");
            break;
    }
    return output;
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

void queryCompetitor(Event *e) {
    int id;
    Track *t;
    Entrant *entrant;
    
    printf("Enter id for the competitor:\n");
    scanf("%d", &id);
    
    entrant = find_entrant(e->entrantlist, id);
    
    printf("COMPETITOR %d:\n", id);
    printf("Name: %s\n", entrant->name);
    printf("Status: %s\n", convert_type_status(entrant->state.type));
    
    if (entrant->state.type == ON_TRACK) {
        t = (Track *) entrant->current_track->data;
        printf("Last recorded time: %s\n", entrant->cp_data.time);
        printf("Location Reference: %d\n", entrant->state.location_ref);
        printf("Currently on track between node %d and node %d\n", t->nodea, t->nodeb);
    } else {
        printf("Currently at node: %d\n", entrant->state.location_ref);
    }
}

int check_num_competitors(Linked_List *el, enum type_status type) {
    int count = 0;
    List_Node *current = el->head;
    Entrant *entrant;
    
    while (current->next != NULL) {
        entrant = (Entrant*) current->data;
        if (entrant->state.type == type) {
            count++;
            printf("%s\n", entrant->name);
        }
        current = current->next;
    }
    
    return count;
} 

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

void manually_read_data(Event *e) {
    CP_Data data;
    
    printf("Enter the type of check point (T/I/A/D/E):\n");
    scanf(" %[TIADE]c", &data.type);
    
    printf("Enter the competitor number:\n");
    scanf(" %d", &data.competitor);
    
    printf("Enter the check point number:\n");
    scanf(" %d", &data.node);
    
    printf("Enter the time recorded:\n");
    scanf(" %[0-9:]s", data.time);

    add_new_time(e, data);
    update_others(e, data);
}

int convert_time_to_mins(char time[TIME_STRING_SIZE]) {
    int hours, mins;
    
    hours = atoi(time);
    mins = atoi(&time[3]);
    
    return (hours*60) + mins;
}

void update_others(Event *evt, CP_Data data){
    List_Node *current_entrant = evt->entrantlist.head;
    Track *track;
    Entrant *e;
    enum type_status status;
    int check_time, current_time, track_total;
    
    while(current_entrant->next != NULL) {
        e = (Entrant *) current_entrant->data;
        status = e->state.type;
        
        /*set others to be on track and update there position*/
        if((status == TIME_CHECKPOINT && data.competitor != e->number) || status == ON_TRACK) {
                /*entrant already on the track*/
                track  = (Track *) e->current_track->data;
                
                check_time = convert_time_to_mins(e->cp_data.time); /*time at last checkpoint*/
                current_time = convert_time_to_mins(data.time);     /*current time*/
                track_total = track->time;                          /*time it should take on track*/
                
                /*fast forward the entrant along the course*/
                while(track_total < current_time - check_time) {
                    e->current_track = e->current_track->next;
                    track = (Track*) e->current_track->data;
                    track_total += track->time;
                }
                e->state.type = ON_TRACK;
        }
        current_entrant = current_entrant->next;
    }
}

int calc_time_diff (char *start, char *end) {
    int start_t, end_t, total;

    start_t  = atoi(start);
    end_t  = atoi(end);
   
    total = end_t - start_t;
    
    return total;  
}

void print_entrant(void *data) {
    Entrant *entrant = (Entrant*) data;
    int total_hours = 0;
    int total_mins = 0;
        
    if(entrant->state.type == COMPLETED) {
            total_hours = calc_time_diff(entrant->start_time, entrant->end_time);
            total_mins = calc_time_diff(&entrant->start_time[3], &entrant->start_time[3]);

            total_hours -= entrant->mc_time_delay_hours;
            total_mins -= entrant->mc_time_delay_mins;
    }

    printf("|%-21s|    %c     |    %s    |    %s    |  %.2dhrs %.2dmins  |\n", 
            entrant->name, entrant->course, 
            entrant->start_time, entrant->end_time, 
            total_hours, total_mins);
}

void print_results(Event *e){
    printf("-------------------------------------------------------------------------------\n");
    printf("|Competitor           |  Course  |  Start Time |   End Time  |     Total      |\n");
    printf("|-----------------------------------------------------------------------------|\n");
    
    traverse_list(e->entrantlist.head, &print_entrant);
    
    printf("-------------------------------------------------------------------------------\n");
}

void print_entrants_excluded(Event *e, enum type_status type) {    
    List_Node *current = e->entrantlist.head;
    Entrant *entrant;
    
    if(type == EXCLUDED_MC) {
        printf("Competitors Excluded from Medical Checkpoints\n");
    } else {
        printf("Competitors Excluded from Regular Checkpoints\n");
    }
    
    printf("------------------------------------------\n");
    printf("|Competitor           |  Node  |  Time   |\n");
    printf("|----------------------------------------|\n");
    
    while (current->next != NULL) {
        entrant = (Entrant*) current->data;
        if(entrant->state.type == type) {
            printf("|%-21s|   %.2d   |  %s  |\n", entrant->name, 
                entrant->state.location_ref, entrant->cp_data.time);
        }
        current = current->next;
    }
    
    printf("------------------------------------------\n");
}