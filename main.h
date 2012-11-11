/* 
 * File:   main.h
 * Author: samuel
 *
 * Created on 11 November 2012, 15:01
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_FILENAME_LENGTH 50
#define FILE_PATH_SIZE 54
#define MAX_EVENT_NAME_SIZE 50
#define MAX_DATE_SIZE 20
#define TIME_STRING_SIZE 5

/*read in the name of each of the data files from the user.*/
void read_file_names();

/*open a file for reading with the given name and a function to read the contents.*/
void read_file(char name[MAX_FILENAME_LENGTH], void (*read_file_func) (FILE *));

/*function to read event details from a file.*/
void read_event_details(FILE *);

/*structure to hold details about a single event*/
typedef struct event {
    char name[MAX_EVENT_NAME_SIZE];
    char date[MAX_DATE_SIZE];
    char start_time[TIME_STRING_SIZE];
} Event;

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

