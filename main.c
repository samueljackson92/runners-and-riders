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


char * convert_type_status(enum type_status type, char * output) {
    switch(type) {
        case NOT_STARTED:
            strcpy(output, "Not yet started.");
            break;
        case MC_CHECKPOINT:
            strcpy(output, "At medical checkpoint.");
            break;
        case ON_TRACK:
            strcpy(output, "Out on course.");
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

void queryCompetitor(Linked_List *el) {
    char output[35];
    int id;
    List_Node *competitor;
    Entrant *entrant;
    
    printf("Enter id for the competitor:\n");
    scanf("%d", &id);
    
    competitor = get_element(el->head, id);
    entrant = (Entrant*) competitor->data;
    
    
    printf("COMPETITOR %d:\n", id);
    printf("Name: %s\n", entrant->name);
    printf("Status: %s\n", convert_type_status(entrant->state.type, output));
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
            entrant->state.type = TIME_CHECKPOINT;
            break;
        case 'I':
            entrant->state.type = EXCLUDED_IR;
            break;
        case 'A':
            entrant->state.type = MC_CHECKPOINT;
            break;
        case 'D':
            entrant->state.type = ON_TRACK;
            break;
        case 'E':
            entrant->state.type = EXCLUDED_MC;
            break;
    }
 
    if(c->nodes[c->path_size-1] == checkpoint_num) {
        entrant->state.type = COMPLETED;
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
    
    add_new_time(e, time, cp_type, comp_num, cp_num);
    update_others(e->entrantlist.head, comp_num);
}

void update_others(List_Node *current, int latest){
    Entrant *e;
    
    while(current->next != NULL) {
        e = (Entrant *) current->data;
        if (latest != e->number-1) {
            e->state.type = ON_TRACK;
        }
        current = current->next;
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
        add_new_time(e, cp_data.time, cp_data.type, cp_data.competitor, cp_data.node);
    }
    update_others(e->entrantlist.head, cp_data.competitor);
}

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
                "6 - Read in a file of updates\n");
        
        scanf("%d", &option);
        switch(option) {
            case 1:
                queryCompetitor(&e->entrantlist);
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
        }
    } while (option != 0);
    
    free(e);
    return (EXIT_SUCCESS);
}
