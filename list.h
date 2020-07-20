#ifndef MONEYTOR_LIST_H
#define MONEYTOR_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PATH_LENGTH 255
#define TYPE_LENGTH 10

typedef struct list list_t;
typedef struct node node_t;

struct node {
    node_t *prev, *next;
    char entryName[MAX_PATH_LENGTH];
    time_t lastChanged;
    char fileType[TYPE_LENGTH];
    list_t *pSubDirList;
};

struct list {
    node_t *first, *last;
    int counter;
};

/* This function initializes the list.
 * on success returns 0, otherwise a another number */
list_t* initList(void);

/* This function removes a specified entry from the list.
 * on success returns 0, otherwise a another number */
int removeElement(list_t *list, node_t* pElement);

/* This function adds an entry to the end of the list.
 * on success returns 0, otherwise a another number */
int addElement(list_t *list, char* name, time_t lastChanged, char* fileType);

/* This function copies a list from src and returns it's new copy via a pointer.
 * on success returns a valid pointer, otherwise NULL */
list_t* copyList(list_t *src);

/* This function deletes list and frees all its memory.
 * on success returns 0, otherwise a another number */
int deleteList(list_t *list);

/* This function prints a list to stdout */
void printList(list_t *list);

#endif //MONEYTOR_LIST_H
