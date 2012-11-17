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
    strcpy(filepath, "res/");
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
    int status, num;
    Node *node; /*new node to store data */
    List_Node *new; /*new list element to wrap node*/
    char cp[3];
    
    do {
        /*assign a new batch of memory to the next node/list node*/
        node = malloc(sizeof(Node));
        new = malloc(sizeof(List_Node));
        
        status = fscanf(file, "%d %s", &num, cp);
        if(status != EOF){
            node->num = num;
            node->type = CP;
            
            new->data = node;
            new->next = NULL;
            
            add_element(&e->nodelist, new);
        }
    } while (status != EOF);
}
void read_courses (FILE *file, Event *e) {
    int status, i=0;
    char name;
    int path_size, *nodes;
   
    Course *course;
    List_Node *node;
    
    do {
        course = malloc(sizeof(Course));
        node = malloc(sizeof(List_Node));
        
        status = fscanf(file, " %c", &name);
        if(status != EOF) {
            fscanf(file, " %d", &path_size);
            nodes = malloc(path_size * sizeof(int));
            for (i=0; i<path_size; i++) {
                fscanf(file, " %d", &nodes[i]);
            }
            
            course->name = name;
            course->path_size = path_size;
            course->nodes = nodes;
            node->data = course;
            add_element(&e->courselist, node);
        }
    } while (status != EOF);
}

void read_tracks(FILE *file, Event *e) {
    int status;
    Track *track;
    List_Node *new;
    
    do {
        track = malloc(sizeof(Track));
        new = malloc(sizeof(List_Node));
        status = fscanf(file, "%d %d %d %d", &track->number, 
                &track->nodea, &track->nodeb, &track->time);
        
        new->data = track;

        add_element(&e->tracklist, new);
    } while (status != EOF);
}

void read_entrants(FILE *file, Event *e) {
    int status;
    Entrant *entrant;
    List_Node *new;
    
    do {
        entrant = malloc(sizeof(Entrant));
        new = malloc(sizeof(List_Node));
        status = fscanf(file, " %d %c %[a-zA-Z ]s", &entrant->number,
                &entrant->course, entrant->name);
        
        entrant->state.type = NOT_STARTED;
        new->data = entrant;
        new->next = NULL;
        
        add_element(&e->entrantlist, new);
        
    } while(status != EOF);
}

void read_checkpoint_data(FILE * file) {
    int status;
    Linked_List cp_data_list;
    List_Node *node;
    CP_Data *data;
    do {
        data = malloc(sizeof(CP_Data));
        node = malloc(sizeof(Node));
        status = fscanf(file, " %c %d %d %5[0-9:]s", &data->type,
                &data->node, &data->competitor, data->time);
        
        node->data = data;
        add_element(&cp_data_list, node);
    } while(status != EOF);
}

int count_lines(FILE *file){
    char line[1024];
    int count = 0;
    
    while(fgets(line, sizeof(line), file) != NULL){
        count++;
    }
    
    rewind(file);
    return count;
}
