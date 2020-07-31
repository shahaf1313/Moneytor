#ifndef MONEYTOR_UTILLS_H
#define MONEYTOR_UTILLS_H

#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "inc/moneytorDefinitions.h"
#include "inc/list.h"
#include "inc/listDataHandlers.h"

//todo: document functions!

void delay(unsigned int ms);

int getFileList(DIR* pDir, char* fullDirPath, LIST fileList, LIST dirList);

void findDiffNodes(LIST original, LIST updated, char* strToPrint, int updateCheck);

char* getEntryType(struct dirent* pd, LIST dirList, char* entryName);

void printDirTree(LIST dirList);

#endif //MONEYTOR_UTILLS_H