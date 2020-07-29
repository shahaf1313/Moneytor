#ifndef MONEYTOR_LIST_H
#define MONEYTOR_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list_private.h"


#define DEBUG_PRINT printf
#define MAX_PATH_LENGTH 255
#define MAX_TYPE_LENGTH 10

/**
 * This function creates a list.
 * It allocates memory for its own usage. When the user finishes using the list, he MUST call LIST_destroy(LIST list)
 * in order to free all allocated memory.
 * ALL data stored in the list must be of the same type!
 * @memoryReleaseFunc [IN] a pointer to function that releases allocated memory for the data in each node.
 * @getDataNameFunc [IN] a pointer to a function that prints a unique name to each of the data elements stored in the
 * list.
 * @return On success - a valid LIST instance, On failure - NULL.
**/
LIST LIST_create(int (*memoryReleaseFunc)(void *ptr), char* (*getDataNameFunc)(void*));

/**
 * This function returns the length of the given list.
 * @list [IN] is a LIST instance to get it's length.
 * @return length as a positive number on success, -1 when list is NULL.
**/
int LIST_getLength(LIST list);

/**
 * This function removes a specified node from the list.
 * It releases the memory allocated for the data of the specified node.
 * @list [IN,OUT] is a LIST instance to remove from.
 * @pDataElement [IN] is a pointer to the data element that we want to remove from the list.
 * @return 0 on success, -1 when list and/or pData are NULL, -2 when the list is empty, -3 when failed to find node in the list,
 * -4 on memory release failure.
**/
int LIST_removeElement(LIST list, void* pDataElement);

/**
 * This function adds a node to the end of the list.
 * It allocates memory for the specified node.
 * @list [IN,OUT] is a LIST instance whom we add to.
 * @pData [IN] is a pointer to the data that will be contained in the node.
 * @return 0 on success, -1 when list is NULL, -2 when element is already in the list,
 * -3 when failed to allocate memory for the node.
**/
int LIST_addElement(LIST list, void* pData);

/**
 * This function destroys the list and frees all its memory, including that of the data in the nodes.
 * @list [IN] is a LIST instance to destroy.
 * @return 0 on success, -1 when list is NULL, -2 when failed to destroy list.
**/
int LIST_destroy(LIST list);

/**
 * This function prints a list to stdout in the format:
 * For each node -
 * [NodeNumber] , [DataPointerName]
 * Summery line -
 * Total number of elements in the list: [listLength]
 * @list [IN] is a LIST instance to print.
 * @return 0 on success, -1 when list is NULL, -2 on failure.
**/
int LIST_print(LIST list);

#endif //MONEYTOR_LIST_H
