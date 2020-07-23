#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

// CR: (DC) Many of the files you committed should not be committed.
// CR: (DC) Here is a list of the files and/or directories that should not be committed
// CR: (DC) and an explanation what each one contains.
// CR: (DC) I encourage you to open each one in your favorite text editor (hopefully, Sublime)
// CR: (DC) and see what I'm talking about.
// CR: (DC) The list of files and directories:
// CR: (DC) .idea/deployment.xml - Holds the ID of your WSL virtual machine, it is machine-specific
// CR: (DC) .idea/workspace.xml - Holds the current configuration of your CLion editor tabs
// CR: (DC) cmake-build-debug - This is the build directory, and should not be committed
// CR: (DC) Please remove all the wrongfully committed files and directories,
// CR: (DC) tell Git to forget them (stop tracking them) and add them to .gitignore to prevent GitKraken
// CR: (DC) from monitoring them.

// CR: (DC) In our shared Google Drive directory, there are two conventions documents
// CR: (DC) Link: https://drive.google.com/drive/folders/1zCWN1LOd_9gxki_tvbB1LSZtc1eEglKV
// CR: (DC) Please read them and fix the code accordingly
// CR: (DC) Note #1: I saw you chose the camelCase naming convention, which is perfectly fine.
// CR: (DC) DO NOT convert your code to snake_case now, we will just keep going with camelCase.
// CR: (DC) Note #2: I saw you chose to name your structs list_t, while our convention is t_list
// CR: (DC) DO NOT change your convention to t_list now, we will keep using list_t.
// CR: (DC) Note #3: If you have any futher questions about conventions, feel free to ask

// CR: (DC) The list module has a lot of CRs and is going to change. We want to able to make small changes
// CR: (DC) and after each small change, test that our change didn't break the code
// CR: (DC) The best way to do it is with Unit Tests
// CR: (DC) Read about the Cutie library: https://github.com/mrdor44/Cutie
// CR: (DC) It is a library for C Unit Testing based on the popular GoogleTest
// CR: (DC) So first, before fixing any other CR, add Cutie as a subrepo (search google for
// CR: (DC) GitKraken how to add subrepo) and create UT for your list module.
// CR: (DC) Create at least one test for each function in the list module, so that after each change you
// CR: (DC) can simply run the UT and see if it works.
// CR: (DC) If you have doubts whether your UT are comprehensive enough before tackling the CRs, contact me

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
#define WINDOWS 0
#define LINUX 1


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
