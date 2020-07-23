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

// CR: (DC) Your list is dedicated for storing this type of data
// CR: (DC) If tomorrow I want a list module that can store integers, I can't use your module
// CR: (DC) So, what we're gonna do is make your list generic
// CR: (DC) Generics in C is implemented using void*
// CR: (DC) So, to make your list generic, your node should hold the data as a void*
// CR: (DC) Meaning, the node doesn't have a clue what the data it holds is
struct node {
    node_t *prev, *next;
    char entryName[MAX_PATH_LENGTH];
    time_t lastChanged;
    char fileType[TYPE_LENGTH];
    list_t *pSubDirList;
};

// CR: (DC) We don't want the developer using the list module to know the internals of our struct
// CR: (DC) If the user of our module knows our structure, they might access the internal fields and change them
// CR: (DC) under our feet, which is terrible.
// CR: (DC) So, what we would do is put the struct definition in the .c file, to hide it from the user of the
// CR: (DC) module. So what will remain here for the user to know? The following line:
// CR: (DC) typedef struct list_s* list_t;
// CR: (DC) Note #1: Notice we've named the struct list_s, to identify that it is a struct
// CR: (DC) Note #2: Notice we define list_t as a pointer. The reason is that we want our user to work
// CR: (DC) with list_t in his code, and not with list_t*, to make his life easier.
// CR: (DC) This practice is known as ADT (Abstract Data Type) or "opaque data type".
struct list {
    // CR: (DC) Split each field to a seperate line
    node_t *first, *last;
    // CR: (DC) What does it count? The number of birds in the sky? A more meaningful name would be "length"
    int counter;
};

// CR: (DC) Our convention is that all functions in a module will be named MODULE_function
// CR: (DC) A few examples:
// CR: (DC) LIST_create (conventionally we use create over init)
// CR: (DC) LIST_removeElement
// CR: (DC) LIST_addElement
// CR: (DC) LIST_destroy (instead of deleteList)

// CR: (DC) The function doesn't initialize the list, it creates the list
// CR: (DC) There's a semantic difference between initialization and creation
// CR: (DC) And, the documentation regarding the return value is wrong
// CR: (DC) Also, specify in the docs that LIST_destroy should be called when we're done with the list
/* This function initializes the list.
 * on success returns 0, otherwise a another number */
list_t* initList(void);

/* This function removes a specified entry from the list.
 * on success returns 0, otherwise a another number */
// CR: (DC) You mixed using list_t* and list_t * (note the space between the type and the *)
// CR: (DC) Our convention is for the * to be next to the type
// CR: (DC) You can use AutoFormat (Ctrl+Alt+L) to have CLion format your code
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
