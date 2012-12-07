#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileio.h"
#include "structures.h"
#include "linked_list.h"
#include "util.h"

/* 
 * File:   fileio.c
 * Author: Samuel Jackson
 *
 * File contains functions used to read in the data files for the application.
 * 
 * Created on 13 November 2012, 14:34
 */

/* Read in all of the data files for this event */
void read_file_data(event *e){
    char filename[MAX_FILEPATH_LENGTH];
    int success = 0;
    
    do {
        printf("Enter location and name of event details file:\n");
        scanf(" %100s", filename);
        success = read_file(filename, &read_event_details, e);
    } while (!success);
    
    
    do {
        printf("Enter location and name of the nodes file:\n");
        scanf(" %100s", filename);
        success = read_file(filename, &read_nodes, e);
    } while(!success);
    
    do {
        printf("Enter location and name of the tracks file:\n");
        scanf(" %100s", filename);
        success = read_file(filename, &read_tracks, e);
    } while(!success);
    
    do {
        printf("Enter location and name of the courses file:\n");
        scanf(" %100s", filename);
        success = read_file(filename, &read_courses, e);
    } while(!success);
    
    do {
        printf("Enter location and name of the entrants file:\n");
        scanf(" %100s", filename);
        success = read_file(filename, &read_entrants, e);
    } while(!success);
    
}

/* Generic read file function  which takes a file specific processing function as a parameter */
int read_file(char filename[MAX_FILEPATH_LENGTH], void (*read_file_func) (FILE *, event *), event *e){
    FILE *file = NULL;
    int status = 0;
    
    file = fopen(filename, "r");
    if(file != NULL) {
        /*file opened successfully, read it.*/
        read_file_func(file, e);
        status = 1;
        fclose(file);
    } else {
        /*couldn't open file, output error*/
        printf("Error opening file:\n");
        printf("%s\n", filename);
    }
    
    return status;
}

/* Function to read the details of this event */
void read_event_details(FILE *file, event *e){
    /*read in the details for the event*/
    fscanf(file, " %[A-Za-z0-9- ]\n", e->name);
    fscanf(file, " %[a-zA-Z0-9 ]\n", e->date);
    fscanf(file, " %[0-9:]\n", e->start_time);
}

/* Function to read in the nodes file*/
void read_nodes(FILE *file, event *e){
    int num;
    node *node_data; /*new node to store data */
    list_node *new_element; /*new list element to wrap node*/
    char type[3];
    
    while (!feof(file)) {
        node_data = malloc(sizeof(node));
        new_element = malloc(sizeof(list_node));
        
        fscanf(file, "%d %2s", &num, type);
        node_data->num = num;
        node_data->type = convert_node_type(type);

        new_element->data = node_data;
        new_element->next = NULL;

        add_element(&e->nodelist, new_element);
    }
}

/* Function to read in a file of courses */
void read_courses (FILE *file, event *e) {
    int status, i;
   
    course *course_data;
    list_node *node, *track_node;
    track *track;
    
    while (!feof(file)) {
        course_data = malloc(sizeof(course));
        node = malloc(sizeof(list_node));
        
        status = fscanf(file, " %c", &course_data->name);
        if(status != EOF) {
            /*read in the number of nodes in the course*/
            fscanf(file, " %d", &course_data->path_size);

            /*setup an array of tracks*/
            course_data->nodes = malloc(course_data->path_size * sizeof(int));
            course_data->tracks.head = NULL;
            course_data->tracks.tail = NULL;
            
            /* Read in an array of all the nodes in this course. */
            for (i=0; i<course_data->path_size; i++) {
                fscanf(file, " %d", &course_data->nodes[i]);
            } 
            
            /*build a list of tracks that are part of this course*/
            for(i=0;i<course_data->path_size-1;i++) {
                track_node = (list_node *) malloc(sizeof(list_node));
                track = find_track(e->tracklist, course_data->nodes[i], course_data->nodes[i+1]);
                track_node->data = track;
                add_element(&course_data->tracks, track_node);
            }

            node->data = course_data;
            add_element(&e->courselist, node);
        }
    }
}

/*Function to read in a file of tracks */
void read_tracks(FILE *file, event *e) {
    track *track_data;
    list_node *new_element;
    
    while (!feof(file)){
        track_data = malloc(sizeof(track));
        new_element = malloc(sizeof(list_node));
        fscanf(file, "%d %d %d %d", &track_data->number, 
                &track_data->nodea, &track_data->nodeb, &track_data->time);
        
        new_element->data = track_data;
        add_element(&e->tracklist, new_element);
    }
}

/* Function to read in a file of entrants */
void read_entrants(FILE *file, event *e) {
    entrant *entrant_data;
    list_node *new_element;
    course *course;
    int status;
    
    while (!feof(file)) {
        entrant_data = malloc(sizeof(entrant));
 
        new_element = malloc(sizeof(list_node));
        status = fscanf(file, "%d %c %[a-zA-Z ]\n", &entrant_data->number,
                &entrant_data->course, entrant_data->name);
        
        if(status != EOF) {
            entrant_data->state.type = NOT_STARTED;
            entrant_data->start_time = 0;
            entrant_data->end_time = 0;

            course = find_course(e->courselist, entrant_data->course);
            entrant_data->current_track = course->tracks.head;

            entrant_data->state.nodes_visited = 0;
            entrant_data->state.location_ref = 0;
            entrant_data->state.type = NOT_STARTED;
            entrant_data->state.late = 0;
            entrant_data->mc_time_delay = 0;
            
            new_element->data = entrant_data;
            new_element->next = NULL;
            add_element(&e->entrantlist, new_element);
        }
    } 
}
