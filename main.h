#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "list.h"

#define SLEEP_TIME_SEC 10
#define TRUE 1
#define FALSE 0


/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms);

/* This function receives pointer to a directory and stores each entry in a list */
static int getFileList(DIR* pDir, list_t* pFileList);

/* This function receives an entry in a folder and returns it's type */
static char* getEntryType(struct dirent *pd);

/* This function gets two arrays lists of strings and prints all the strings that differ from the first
 * list to the second, with the strToPrint (Added/Deleted) */
static void findDiffNodes(list_t *original, list_t *updated, char *strToPrint, int updateCheck);

/* This function catches ctrl + c and causes to program termination */
void intHandler(int dummy);

#endif //MONEYTOR_MAIN_H
