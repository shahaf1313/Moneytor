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
#define MAX_PATH_LENGTH 255
#define MAX_TYPE_LENGTH 10

typedef struct fileInfo {
    char fileName[MAX_PATH_LENGTH];
    time_t lastChanged;
    char fileType[MAX_TYPE_LENGTH];
} fileInfo_t;

typedef struct dirInfo {
    char dirName[MAX_PATH_LENGTH];
    struct list *subDirList;
} dirInfo_t;




/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms);

/* This function receives pointer to a directory and stores each entry in a list */
static int getFileList(DIR* pDir, char *fullDirPath, list_t* pFileList, list_t *pDirList);

/* This function receives an entry in a folder.
 * If the current entry is a directory, the function searches it in dirList and if it doen't exists, adds it to the list.*/
static char* getEntryType(struct dirent *pd, list_t *pDirList, char *entryName);

/* This function gets two arrays lists of strings and prints all the strings that differ from the first
 * list to the second, with the strToPrint (Added/Deleted) */
static void findDiffNodes(list_t *original, list_t *updated, char *strToPrint, int updateCheck);

/* This function catches ctrl + c and causes to program termination */
void intHandler(int dummy);

/* This function prints the entire tree of files and subfolders in dirList */
static void printDirTree(list_t *pDirList);

#endif //MONEYTOR_MAIN_H
