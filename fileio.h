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
#define MAX_FILENAME_LENGTH 50
#define FILE_PATH_SIZE 54

/*read in the name of each of the data files from the user.*/
void read_file_names();

/*open a file for reading with the given name and a function to read the contents.*/
void read_file(char name[MAX_FILENAME_LENGTH], void (*read_file_func) (FILE *file));

/*function to read event details from a file.*/
void read_event_details(FILE *file);

/*function to read in a data file of nodes*/
void read_nodes(FILE *file);

/*function to read in a data file of tracks*/
void read_tracks(FILE *file);

/*function to read in a data file of courses*/
void read_courses(FILE * file);

/*function to read in a data file of entrants*/
void read_entrants(FILE *file);

/*function to read in a data file of new checkpoint data*/
void read_checkpoint_data(FILE *file);

/*function count the number of lines in a file*/
int count_lines(FILE *file);

#ifdef	__cplusplus
}
#endif

#endif	/* FILEIO_H */

