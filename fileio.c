#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileio.h"
#include "structures.h"
#include "linked_list.h"

void read_file_data(Event *e){
    char filename[MAX_FILENAME_LENGTH];
    
    /*
    printf("Enter name of event details file:\n");
    scanf(" %s", filename);*/
    read_file("name.txt", &read_event_details, e);
    
    /*printf("Enter name of the nodes file:\n");
    scanf(" %s", filename);*/
    read_file("nodes.txt", &read_nodes, e);
    
    /*printf("Enter name of the tracks file:\n");
    scanf(" %s", filename);*/
    read_file("tracks.txt", &read_tracks, e);
    
    /*
    printf("Enter name of the courses file:\n");
    scanf(" %s", filename);*/
    read_file("courses.txt", &read_courses, e);
    
    /*printf("Enter name of the entrants file:\n");
    scanf(" %s", filename);*/
    read_file("entrants.txt", &read_entrants, e);
    
    /*printf("Enter name of the checkpoint files:\n");
    scanf(" %s", filename);
    read_file("", &read_checkpoint_data);*/
    
}

void read_file(char name[MAX_FILENAME_LENGTH], void (*read_file_func) (FILE *, Event *), Event *e){
    FILE *file;
    char filepath[FILE_PATH_SIZE];
    strcpy(filepath, "res2/");
    strcat(filepath, name);
    
    file = fopen(filepath, "r");
    if(file != NULL) {
        /*file opened successfully, read it.*/
        read_file_func(file, e);
    } else {
        /*couldn't open file, output error*/
        puts("Error opening file!");
        puts(filepath);
    }
    fclose(file);
}

void read_event_details(FILE *file, Event *e){
    /*read in the details for the event*/
    fscanf(file, " %[A-Za-z0-9- ]s", e->name);
    fscanf(file, " %[a-zA-Z0-9 ]s", e->date);
    fscanf(file, " %[0-9:]s", e->start_time);
}

void read_nodes(FILE *file, Event *e){
    int num;
    Node *node; /*new node to store data */
    List_Node *new; /*new list element to wrap node*/
    char cp[3];
    
    while (!feof(file)) {
        node = malloc(sizeof(Node));
        new = malloc(sizeof(List_Node));
        
        fscanf(file, "%d %s", &num, cp);
        node->num = num;
        node->type = CP;

        new->data = node;
        new->next = NULL;

        add_element(&e->nodelist, new);
    }
}

Track * findTrack(Linked_List list, int node_a, int node_b) {
    List_Node * current = list.head;
    Track * current_track;
    int found =0;
    
    while (!found && current->next != NULL) {
        current_track = (Track*) current->data;
        if ((node_a == current_track->nodea && node_b == current_track->nodeb)
            || (node_a == current_track->nodeb && node_b == current_track->nodea)) {
            found = 1;
        }
        current = current->next;
    }
    
    return current_track;
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
                track = findTrack(e->tracklist, course->nodes[i], course->nodes[i+1]);
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

Course * findCourse(Linked_List *list, char c) {
    List_Node *current = list->head;
    Course * current_course;
    int found = 0;
    
    while (!found && current->next){
        current_course = (Course *) current->data;
        if(c == current_course->name){
            found = 1;
        }
        current = current->next;
    }
    
    return current_course;
}

void read_entrants(FILE *file, Event *e) {
    Entrant *entrant;
    List_Node *new;
    Course *course;
    
    while (!feof(file)) {
        entrant = malloc(sizeof(Entrant));
        new = malloc(sizeof(List_Node));
        fscanf(file, " %d %c %[a-zA-Z ]s", &entrant->number,
                &entrant->course, entrant->name);
        
        entrant->state.type = NOT_STARTED;
        strcpy(entrant->start_time, "00:00");
        strcpy(entrant->end_time, "00:00");
        
        course = findCourse(&e->courselist, entrant->course);
        entrant->current_track = course->tracks.head;
        
        new->data = entrant;
        new->next = NULL;
        add_element(&e->entrantlist, new);
    } 
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
