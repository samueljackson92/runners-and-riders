/* 
 * File:   main.c
 * Author: Samuel Jackson
 *
 * Created on 11 November 2012, 14:31
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "fileio.h"
#include "structures.h"

int main(int argc, char** argv) {
    Event *e = malloc(sizeof(Event));
    read_file_data(e);
    free(e);
    return (EXIT_SUCCESS);
}