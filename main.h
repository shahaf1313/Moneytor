#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define SLEEP_TIME_SEC 10
#define MAX_PATH_LENGTH 256
#define TRUE 1
#define FALSE 0
#define MAX_FILES_NUM 30

/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms);

/* This function receives pointer to a directory and stors each entry in a string array (future - list) */
static int getFileList(DIR* pDir, char **pFileList);

/* This function receives an entry in a folder and returns it's type */
static char* getEntryType(struct dirent *pd);

/* This function gets two arrays (future - lists) of strings and prints all the strings that differ from the first
 * array to the second, with the strToPrint (Added/Deleted) */
static void findDiffStrings(char **firstArr, int n1, char **secondArr, int n2, char *strToPrint);

void copyStringArr(char **dest, char **src, int row);

void zeroStringArray(char **arr, int row, int col);

#endif //MONEYTOR_MAIN_H
