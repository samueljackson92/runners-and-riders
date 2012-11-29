/* 
 * File:   main.c
 * Author: Samuel Jackson
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
        
        scanf(" %d", &option);
        clear_screen();
        switch(option) {
            case 1:
                query_competitor(e);
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

void query_competitor(Event *e) {
    int id;
    Track *t;
    Entrant *entrant;
    char buff[OUTPUT_BUFF];
    
    printf("Enter id for the competitor:\n");
    scanf("%d", &id);
    
    clear_screen();
    
    entrant = find_entrant(e->entrantlist, id);
    convert_type_status(entrant->state.type, buff);
    
    printf("COMPETITOR %d:\n", id);
    printf("Name: %s\n", entrant->name);
    printf("Status: %s\n", buff);
    
    switch(entrant->state.type) {
        case ON_TRACK:
            t = (Track *) entrant->current_track->data;
            printf("Last recorded time: %s\n", entrant->cp_data.time);
            printf("Location Reference: %d\n", entrant->state.location_ref);
            printf("Presumed on track between node %d and node %d\n", t->nodea, t->nodeb);
            break;
        default:
            printf("Currently at node: %d\n", entrant->state.location_ref);
            break;
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

void read_updates(Event *e) {
    char filename[MAX_FILENAME_LENGTH];
    CP_Data data;
    
    printf("Enter name of the checkpoint files:\n");
    scanf(" %s", filename);
    
    FILE *file = fopen(filename, "r");
    
    while (!feof(file)){
        fscanf(file, "%c %d %d %5[0-9:]s", &data.type, &data.node, 
                &data.competitor, data.time);
        add_new_time(e, data);
    }
    update_others(e, data);
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