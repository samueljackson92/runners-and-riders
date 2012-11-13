#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fileio.h"
#include "structures.h"

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
    fscanf(file, " %s", e->date);
    fscanf(file, " %s", e->start_time);
}

void read_nodes(FILE *file, Event *e){
    int line_count = count_lines(file);
    int status, i=0;
    char cp[3];
    
    e->nodelist = malloc(line_count * sizeof(Node));
    
    if(e->nodelist == 0) {
        puts("Cannot build node list! Out of memory!");
    }
    
    do {
        status = fscanf(file, "%d %s", &e->nodelist[i].num, cp);
        i++;
    } while (status != EOF);
}

void read_tracks(FILE *file, Event *e) {
    int status, i =0;
    int line_count = count_lines(file);
    
    e->tracklist = malloc(line_count * sizeof(Track));
    
    if (e->tracklist == 0) {
        puts("Cannot build track list! Out of memory!");
    }
    
    do {
        status = fscanf(file, "%d %d %d %d", 
                &e->tracklist[i].number, &e->tracklist[i].nodea, 
                &e->tracklist[i].nodeb, &e->tracklist[i].time );
        i++;
    } while (status != EOF);
}

void read_courses (FILE *file, Event *e) {
    int status, i=0, j=0;
    int line_count = count_lines(file);
    
    e->courselist = malloc(line_count * sizeof(Course));
    if(e->courselist == 0) {
        puts("Cannot build courses list! Out of memory!");
    }
   
    do {
        status = fscanf(file, " %c", &e->courselist[i].name);
        if(status != EOF) {
            fscanf(file, " %d", &e->courselist[i].path_size);
            e->courselist[i].nodes = malloc(e->courselist[i].path_size * sizeof(int));
            for (j=0; j<e->courselist[i].path_size; j++) {
                fscanf(file, " %d", &e->courselist[i].nodes[j]);
            }
        }
        i++;
    } while (status != EOF);

}

void read_entrants(FILE *file, Event *e) {
    int status, i=0;
    int line_count = count_lines(file);
    e->entrantlist = malloc(line_count * sizeof(Entrant));
    
    do {
        status = fscanf(file, " %d %c %[a-zA-Z ]s", &e->entrantlist[i].number,
                &e->entrantlist[i].course, e->entrantlist[i].name);
        i++;
    } while(status != EOF);
}

void read_checkpoint_data(FILE * file) {
    int status, i=0;
    int line_count = count_lines(file);
    CP_Data *data = malloc(line_count * sizeof(CP_Data));
    
    do {
        status = fscanf(file, " %c %d %d %5[0-9:]s", &data[i].type,
                &data[i].node, &data[i].competitor, data[i].time);
        i++;
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
