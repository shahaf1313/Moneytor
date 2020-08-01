#ifndef MONEYTOR_UTILLS_H
#define MONEYTOR_UTILLS_H

#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "inc/moneytorDefinitions.h"
#include "inc/listDataHandlers.h"

#define TRUE 1
#define FALSE 0

//todo: document functions!

void delay(unsigned int ms);

int getFileList(DIR* pDir, char* currentWorkingDirectory, LIST fileList, LIST dirList);

void findDiffNodes(LIST original, LIST updated, char* strToPrint, int updateCheck);

void printDirTree(LIST dirList);

#endif //MONEYTOR_UTILLS_H