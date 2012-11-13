#include <stdlib.h>
#include <stdio.h>
#include "fileio.h"
#include "structures.h"

void read_file_names(){
    char filename[MAX_FILENAME_LENGTH];
    
    /*
    printf("Enter name of event details file:\n");
    scanf(" %s", filename);
    read_file(filename, &read_event_details);*/
    
    
    /*printf("Enter name of the nodes file:\n");
    scanf(" %s", filename);
    read_file("nodes.txt", &read_nodes);*/
    
    /*printf("Enter name of the tracks file:\n");
    scanf(" %s", filename);
    read_file(filename, &read_tracks);*/
    
    /*
    printf("Enter name of the courses file:\n");
    scanf(" %s", filename);
    read_file(filename, &read_courses);*/
    
    /*printf("Enter name of the entrants file:\n");
    scanf(" %s", filename);
    read_file(filename, &read_entrants);*/
    
    printf("Enter name of the checkpoint files:\n");
    scanf(" %s", filename);
    read_file(filename, &read_checkpoint_data);
    /*
    scanf("\nEnter name of event nodes file:\n", event_nodes_file);
    scanf("\nEnter name of event tracks file:\n", event_tracks_file);
    scanf("\nEnter name of event courses file:\n", event_course_file);
    scanf("\nEnter name of event entrants file:\n", event_entrants_file);*/
    
}

void read_file(char name[MAX_FILENAME_LENGTH], void (*read_file_func) (FILE *)){
    FILE *file;
    char filepath[FILE_PATH_SIZE];
    strcpy(filepath, "res/");
    strcat(filepath, name);
    
    file = fopen(filepath, "r");
    if(file != NULL) {
        /*file opened successfully, read it.*/
        read_file_func(file);
    } else {
        /*couldn't open file, output error*/
        puts("Error opening file!");
        puts(filepath);
    }
    fclose(file);
}

void read_event_details(FILE *file){
    Event e;

    /*read in the details for the event*/
    fscanf(file, "%[A-Za-z0-9: ]s", e.name);
    fscanf(file, "%s", e.date);
    fscanf(file, "%s", e.start_time);
    
    printf("%s\n", e.name);
}

void read_nodes(FILE *file){
    int line_count = count_lines(file);
    int status, i=0;
    char cp[3];
    
    Node *nodes = malloc(line_count * sizeof(Node));
    
    if(nodes == 0) {
        puts("Cannot build node list! Out of memory!");
    }
    
    do {
        status = fscanf(file, "%d %s", &nodes[i].num, cp);
        if(status != EOF) {
            printf("%d\n", nodes[i].num);
            printf("%s\n", cp);
        }
        i++;
    } while (status != EOF);
}

void read_tracks(FILE *file) {
    int status, i =0;
    int line_count = count_lines(file);
    
    Track *tracks = malloc(line_count * sizeof(Track));
    
    if (tracks == 0) {
        puts("Cannot build track list! Out of memory!");
    }
    
    do {
        status = fscanf(file, "%d %d %d %d", 
                &tracks[i].number, &tracks[i].nodea, 
                &tracks[i].nodeb, &tracks[i].time );
        
        if(status != EOF) {
            printf("%d\n", tracks[i].number);
        }
        
        i++;
    } while (status != EOF);
}

void read_courses (FILE *file) {
    int status, i=0, j=0;
    int line_count = count_lines(file);
    
    Course *courses = malloc(line_count * sizeof(Course));
    if(courses == 0) {
        puts("Cannot build courses list! Out of memory!");
    }
   
    do {
        status = fscanf(file, " %c", &courses[i].name);
        if(status != EOF) {
            fscanf(file, " %d", &courses[i].path_size);
            courses[i].nodes = malloc(courses[i].path_size * sizeof(int));
            for (j=0; j<courses[i].path_size; j++) {
                fscanf(file, " %d", &courses[i].nodes[j]);
            }
        }
        i++;
    } while (status != EOF);

}

void read_entrants(FILE *file) {
    int status, i=0;
    int line_count = count_lines(file);
    Entrant *competitors = malloc(line_count * sizeof(Entrant));
    
    do {
        competitors[i].excluded = FALSE;
        status = fscanf(file, " %d %c %[a-zA-Z ]s", &competitors[i].number,
                &competitors[i].course, competitors[i].name);
        
        if(status != EOF) {
            /* do stuff*/
            printf("%s %d %c\n", competitors[i].name, competitors[i].number, competitors[i].course);
        }
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
        if(status != EOF) {
            printf("%c %d %d %s\n", data[i].type, data[i].node, data[i].competitor, data[i].time);
        }
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
