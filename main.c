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
#include "fileio.h"
#include "structures.h"

char * convert_type_status(enum type_status type, char * output) {
    switch(type) {
        case NOT_STARTED:
            strcpy(output, "Not yet started!");
            break;
    }
    return output;
}

void queryCompetitor(List_Node *el) {

}

void check_started_competitors(Entrant *el){
    
}

int main(int argc, char** argv) {
    int option;
    Event *e = malloc(sizeof(Event));
    
    e->courselist = NULL;
    e->entrantlist = NULL;
    e->nodelist = NULL;
    e->tracklist = NULL;
    
    read_file_data(e);
    
    puts("Event data loaded!");
    
    do {
        printf("Enter an Option:\n"
                "0 - Exit\n"
                "1 - Query Competitor\n");
        
        scanf("%d", &option);
        switch(option) {
            case 1:
                queryCompetitor(e->entrantlist);
                break;
        }
    } while (option != 0);
    
    free(e);
    return (EXIT_SUCCESS);
}
