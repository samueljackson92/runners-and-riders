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
    
    e->courselist.head = NULL;
    e->courselist.tail = NULL;
    e->entrantlist.head = NULL;
    e->entrantlist.tail = NULL;
    e->nodelist.head = NULL;
    e->nodelist.tail = NULL;
    e->tracklist.head = NULL;
    e->tracklist.tail = NULL;
    
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
                "7 - Print table of results\n");
        
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

char * convert_type_status(enum type_status type, char * output) {
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

void queryCompetitor(Event *e) {
    char output[35];
    int id;
    List_Node *competitor;
    List_Node *node = e->tracklist.head;
    Track *t;
    Entrant *entrant;
    
    printf("Enter id for the competitor:\n");
    scanf("%d", &id);
    
    id--; /*linked lists are zero based*/
    
    competitor = get_element(e->entrantlist.head, id);
    entrant = (Entrant*) competitor->data;
    
    printf("COMPETITOR %d:\n", id+1);
    printf("Name: %s\n", entrant->name);
    printf("Status: %s\n", convert_type_status(entrant->state.type, output));
    
    if (entrant->state.type == ON_TRACK) {
        while (node->next != NULL){
            t = (Track*) node->data;
            if(t->number == entrant->state.location_ref) {
                break;
            }
            node = node->next;
        }
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

void add_new_time(Event *e, char time[TIME_STRING_SIZE], char type, 
                                int entrant_num, int checkpoint_num){
    
    List_Node *competitor = get_element(e->entrantlist.head, entrant_num);
    Entrant *entrant = (Entrant*) competitor->data;
    CP_Data cp;
       
    List_Node *current = e->courselist.head;
    Course *c;
    
    while(current->next != NULL) {
        c = (Course *) current->data;
        if (c->name == entrant->course) {
            break;
        }
        current = current->next;
    }
    
    
    cp.competitor = entrant_num;
    cp.node = checkpoint_num;
    strcpy(cp.time, time);
    cp.type = type;
    
    switch(type) {
        case 'T':
            if(entrant->state.type == NOT_STARTED){
                strcpy(entrant->start_time, cp.time);
            }
            entrant->state.type = TIME_CHECKPOINT;
            
            while(c->nodes[entrant->state.nodes_visited] != cp.node) {
                entrant->state.nodes_visited++;
            }
            
            entrant->state.location_ref = cp.node; 
            
            if(entrant->state.nodes_visited == c->path_size-1) {
                entrant->state.type = COMPLETED;
                strcpy(entrant->end_time, cp.time);
            }
            
            break;
    }
 
    entrant->cp_data = cp;
}

void manually_read_data(Event *e) {
    char cp_type;
    int comp_num, cp_num;
    char time[TIME_STRING_SIZE];
    
    printf("Enter the type of check point (T/I/A/D/E):\n");
    scanf(" %[TIADE]c", &cp_type);
    
    printf("Enter the competitor number:\n");
    scanf(" %d", &comp_num);
    
    printf("Enter the check point number:\n");
    scanf(" %d", &cp_num);
    
    printf("Enter the time recorded:\n");
    scanf(" %[0-9:]s", time);
    
    add_new_time(e, time, cp_type, comp_num-1, cp_num);
    update_others(e, comp_num);
}

void update_others(Event *evt, int latest){
    List_Node *current_entrant = evt->entrantlist.head;
    List_Node *current_course;
    List_Node *current_track;
    Entrant *e;
    Course *c;
    Track *t;
    
    int i=0, node_a, node_b, found=0;
    
    
    while(current_entrant->next != NULL) {
        e = (Entrant *) current_entrant->data;
        
        current_course = evt->courselist.head;
        current_track = evt->tracklist.head;
        
        if(e->state.type != COMPLETED && e->state.type != NOT_STARTED 
                && latest != e->number) {
            
            /* get current course*/
            while(e->course != c->name && current_course->next != NULL){
                c = (Course*) current_course->data;
                current_course = current_course->next;
            }

            /*get current track numbers*/
            for (i=0; i< c->path_size; i++) {
                if (e->state.location_ref == c->nodes[i]) {
                    node_a = c->nodes[i];
                    node_b = c->nodes[i+1];
                    break;
                }
            }
            
            /*find the track id and set their location to it*/
            found = 0;
            while(!found && current_track->next != NULL) {
                t = (Track*) current_track->data;
                if ((node_a == t->nodea && node_b == t->nodeb)
                        || (node_a == t->nodeb && node_b == t->nodea)) {
                    e->state.location_ref = t->number;
                    e->state.type = ON_TRACK;
                    found = 1;
                }
                current_track = current_track->next;
            }
        }
        
        current_entrant = current_entrant->next;
    }
}

void read_updates(Event *e) {
    char filename[MAX_FILENAME_LENGTH];
    CP_Data cp_data;
    
    printf("Enter name of the checkpoint files:\n");
    scanf(" %s", filename);
    
    FILE *file = fopen(filename, "r");
    
    while (!feof(file)){
        fscanf(file, "%c %d %d %5[0-9:]s", &cp_data.type, &cp_data.node, 
                &cp_data.competitor, cp_data.time);
        add_new_time(e, cp_data.time, cp_data.type, cp_data.competitor-1, cp_data.node);
    }
    update_others(e, cp_data.competitor);
}

int calc_total_time (char *start, char *end) {
    int start_t, end_t, total;

    start_t  = atoi(start);
    end_t  = atoi(end);
   
    total = end_t - start_t;
    
    return total;  
}

void print_results(Event *e){
    int i, total_hours, total_mins;
    Entrant *entrant;

    printf("-------------------------------------------------------------------------------\n");
    printf("|Competitor           |  Course  |  Start Time |   End Time  |     Total      |\n");
    printf("|-----------------------------------------------------------------------------|\n");
    
    for (i=0; i< e->no_of_entrants-1; i++) {
        total_hours = 0;
        total_mins = 0;
    
        entrant = (Entrant*)get_element_data(e->entrantlist.head, i);
        
        if(entrant->state.type == COMPLETED) {
                total_hours = calc_total_time(entrant->start_time, entrant->end_time);
                total_mins = calc_total_time(&entrant->start_time[3], &entrant->start_time[3]);
        }
        
        printf("|%-21s|    %c     |    %s    |    %s    |  %.2dhrs %.2dmins  |\n", 
                entrant->name, entrant->course, 
                entrant->start_time, entrant->end_time, 
                total_hours, total_mins);
    }
    
    printf("-------------------------------------------------------------------------------\n");
}
