/* 
 * File:   main.c
 * Author: Samuel Jackson
 *
 * Created on 11 November 2012, 14:31
 */

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

/*
 * 
 */
int main(int argc, char** argv) {

    read_file_names();
    return (EXIT_SUCCESS);
}

void read_file_names(){
    char event_details_file[50];
    char event_nodes_file[MAX_FILENAME_LENGTH];
    char event_tracks_file[MAX_FILENAME_LENGTH];
    char event_course_file[MAX_FILENAME_LENGTH];
    char event_entrants_file[MAX_FILENAME_LENGTH];
    
    printf("Enter name of event details file:");
    scanf(" %s", event_details_file);
    
    /*
    scanf("\nEnter name of event nodes file:\n", event_nodes_file);
    scanf("\nEnter name of event tracks file:\n", event_tracks_file);
    scanf("\nEnter name of event courses file:\n", event_course_file);
    scanf("\nEnter name of event entrants file:\n", event_entrants_file);*/
    
    read_file(event_details_file, &read_event_details);
    
}

void read_file(char name[MAX_FILENAME_LENGTH], void (*read_file_func) (FILE *)){
   
    FILE *file;
    file = fopen(name, "r");
    if(file != NULL) {
        /*file opened successfully, read it.*/
        read_file_func(file);
    } else {
        puts("Error opening file!");
    }
    fclose(file);
}

void read_event_details(FILE *file){
    Event e;

    /*read in the details for the event*/
    fgets(e.name, sizeof(e.name), file);
    fgets(e.date, sizeof(e.date), file);
    fgets(e.start_time, sizeof(e.start_time), file);
    
    printf("%s", e.name);
}

