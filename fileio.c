#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileio.h"
#include "structures.h"
#include "linked_list.h"
#include "util.h"

void read_file_data(Event *e){
    char filename[MAX_FILEPATH_LENGTH];

    /*printf("Enter name of event details file:\n");
    scanf(" %100s", filename);*/
    read_file("name.txt", &read_event_details, e);
    
    /*printf("Enter name of the nodes file:\n");
    scanf(" %100s", filename);*/
    read_file("nodes.txt", &read_nodes, e);
    
    /*printf("Enter name of the tracks file:\n");
    scanf(" %100s", filename);*/
    read_file("tracks.txt", &read_tracks, e);
    
    /*printf("Enter name of the courses file:\n");
    scanf(" %100s", filename);*/
    read_file("courses.txt", &read_courses, e);
    
    /*printf("Enter name of the entrants file:\n");
    scanf(" %100s", filename);*/
    read_file("entrants.txt", &read_entrants, e);
    
}

void read_file(char filename[MAX_FILEPATH_LENGTH], void (*read_file_func) (FILE *, Event *), Event *e){
    FILE *file;
    
    file = fopen(filename, "r");
    if(file != NULL) {
        /*file opened successfully, read it.*/
        read_file_func(file, e);
    } else {
        /*couldn't open file, output error*/
        printf("Error opening file!\n");
        printf("%s\n", filename);
    }
    fclose(file);
}

void read_event_details(FILE *file, Event *e){
    /*read in the details for the event*/
    fscanf(file, " %[A-Za-z0-9- ]s", e->name);
    fscanf(file, " %[a-zA-Z0-9 ]s", e->date);
    fscanf(file, " %[0-9:]s", e->start_time);
}

enum check_point convert_node_type(char type_string[3]){
    enum check_point type;
    if(!strcmp(type_string, "CP")) {
        type = CP;
    } else if (!strcmp(type_string, "JN")) {
        type = JN;
    }
    return type;
}

void read_nodes(FILE *file, Event *e){
    int num;
    Node *node; /*new node to store data */
    List_Node *new; /*new list element to wrap node*/
    char type[3];
    
    while (!feof(file)) {
        node = malloc(sizeof(Node));
        new = malloc(sizeof(List_Node));
        
        fscanf(file, "%d %s", &num, type);
        node->num = num;
        node->type = convert_node_type(type);

        new->data = node;
        new->next = NULL;

        add_element(&e->nodelist, new);
    }
}

void read_courses (FILE *file, Event *e) {
    int status, i;
    char name;
    int path_size;
   
    Course *course;
    List_Node *node, *track_node;
    Track *track;
    
    while (!feof(file)) {
        path_size = 0;
        course = malloc(sizeof(Course));
        node = malloc(sizeof(List_Node));
        
        status = fscanf(file, " %c", &name);
        if(status != EOF) {
            fscanf(file, " %d", &path_size);

            course->nodes = malloc(path_size * sizeof(int));
            
            for (i=0; i<path_size; i++) {
                fscanf(file, " %d", &course->nodes[i]);
            }
            
            course->name = name;
            course->path_size = path_size;
            
            
            /*build a list of tracks that are part of this course*/
            for(i=0;i<path_size-1;i++) {
                track_node = (List_Node *) malloc(sizeof(List_Node));
                track = find_track(e->tracklist, course->nodes[i], course->nodes[i+1]);
                track_node->data = track;
                add_element(&course->tracks, track_node);
            }

            node->data = course;
            add_element(&e->courselist, node);
        }
    }
}

void read_tracks(FILE *file, Event *e) {
    Track *track;
    List_Node *new;
    
    while (!feof(file)){
        track = malloc(sizeof(Track));
        new = malloc(sizeof(List_Node));
        fscanf(file, "%d %d %d %d", &track->number, 
                &track->nodea, &track->nodeb, &track->time);
        
        new->data = track;
        add_element(&e->tracklist, new);
    }
}

void read_entrants(FILE *file, Event *e) {
    Entrant *entrant;
    List_Node *new;
    Course *course;
    int status;
    
    while (!feof(file)) {
        entrant = malloc(sizeof(Entrant));
        new = malloc(sizeof(List_Node));
        status = fscanf(file, " %d %c %[a-zA-Z ]s", &entrant->number,
                &entrant->course, entrant->name);
        
        if(status != EOF) {
            entrant->state.type = NOT_STARTED;
            strcpy(entrant->start_time, "00:00");
            strcpy(entrant->end_time, "00:00");

            course = find_course(e->courselist, entrant->course);
            entrant->current_track = course->tracks.head;

            new->data = entrant;
            new->next = NULL;
            add_element(&e->entrantlist, new);
        }
    } 
}
