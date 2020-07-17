#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define SLEEP_TIME_SEC 10
#define MAX_PATH_LENGTH 256

/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms);

/* This function receives an entry in a folder and returns it's type */
static char* getEntryType(struct dirent *pd);

#endif //MONEYTOR_MAIN_H
