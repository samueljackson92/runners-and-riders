/* 
 * File:   fileio.h
 * Author: samuel
 *
 * Created on 13 November 2012, 09:41
 */

#ifndef FILEIO_H
#define	FILEIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "structures.h"
    
/*
 * macro for the maximum filepath length
 * set to 101 so as to leave room for the /0
 */
#define MAX_FILEPATH_LENGTH 101

/*read in the name of each of the data files from the user.*/
void read_file_data(event *e);

/*open a file for reading with the given name and a function to read the contents.*/
int read_file(char name[MAX_FILEPATH_LENGTH], void (*read_file_func) (FILE *file, event *e), event *e);

/*function to read event details from a file.*/
void read_event_details(FILE *file, event *e);

/*function to read in a data file of nodes*/
void read_nodes(FILE *file, event *e);

/*function to read in a data file of tracks*/
void read_tracks(FILE *file, event *e);

/*function to read in a data file of courses*/
void read_courses(FILE * file, event *e);

/*function to read in a data file of entrants*/
void read_entrants(FILE *file, event *e);

#ifdef	__cplusplus
}
#endif

#endif	/* FILEIO_H */

