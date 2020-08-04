#ifndef MONEYTOR_LIST_H
#define MONEYTOR_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "inc/common.h"

typedef struct list list_t;
typedef list_t* LIST;

typedef int (*memoryReleaseFunction_t)(void*);

typedef char* (*getNameFunction_t)(void*);

typedef void* (*copyFunction_t)(void*);

/**
 * @brief This function creates a list.
 * It allocates memory for its own usage. When the user finishes using the list, he MUST call LIST_destroy(LIST list)
 * in order to free all allocated memory.
 * ALL data stored in the list must be of the same type!
 * @param memoryReleaseFunc [IN] a pointer to function that releases allocated memory for each of the data elements.
 * This function MUST return 0 when it has released memory successfully, OW another integer.
 * @param getDataNameFunc [IN] a pointer to a function that returns a pointer to a string that contains a name to each
 * for the data elements stored in the list.
 * @returns On success - a valid LIST instance, On failure - NULL.
**/
LIST LIST_create(memoryReleaseFunction_t memoryReleaseFunction, getNameFunction_t getNameFunction);

/**
 * @brief This function returns the length of the given list.
 * @param list [IN] is a LIST instance to get it's length.
 * @param length [OUT] is a pointer to an integer where list's length will be sotred.
 * @returns Appropriate listReturnCode_t.
**/
returnCode_t LIST_getLength(LIST list, int* length);

/**
 * @brief This function returns a pointer to the first data element in the list.
 * @param list [IN] is a LIST instance to get it's first element.
 * @returns pointer to the first element. If list is NULL or that the list is empty - NULL is returned.
**/
void* LIST_getFirst(LIST list);

/**
 * @brief This function returns a pointer to the last data element in the list.
 * @param list [IN] is a LIST instance to get it's last element.
 * @returns pointer to the last element. If list is NULL or that the list is empty - NULL is returned.
**/
void* LIST_getLast(LIST list);

/**
 * @brief This function returns a pointer to the next data element of a given element in the list.
 * If the element is not on the list, or the element is the last in the list - an appropriate listReturnCode_t is returned.
 * @param list [IN] is a LIST instance to get it's next element.
 * @param pDataCurrent [IN] is a pointer to a data element from which we want to get the next data element.
 * @param pDataNext [OUT] is an address of a pointer to the next data element.
 * If the element is either not found or the last element - NULL is set to this pointer's address.
 * @returns Appropriate listReturnCode_t.
**/
returnCode_t LIST_getNext(LIST list, void* pDataCurrent, void** pDataNext);

/**
 * @brief This function removes a specified element from the list.
 * It releases the memory allocated for the data of the specified element.
 * @param list [IN,OUT] is a LIST instance to remove from.
 * @param pDataElement [IN] is a pointer to the data element that we want to remove from the list.
 * @returns Appropriate listReturnCode_t.
**/
returnCode_t LIST_removeElement(LIST list, void* pDataElement);

/**
 * @brief This function adds an element to the end of the list.
 * It allocates memory for the specified element.
 * @param list [IN,OUT] is a LIST instance whom we add to.
 * @param pData [IN] is a pointer to the data we want to store in the list.
 * @returns Appropriate listReturnCode_t.
**/
returnCode_t LIST_addElement(LIST list, void* pData);

/**
 * @brief This function copies a list from dest to src.
 * This function assumes that dest is a parameter for function use and data stored inside of it will be lost.
 * All elements in the src list will be copied and not shared with dest list.
 * @param dest [OUT] is a pointer to LIST instance whom we copy to. Could not be NULL!
 * @param src [IN] is a valid LIST instance whom we copy from, or LIST that is NULL.
 * @param copyFunction [IN] a pointer to a function that gets pointer to data element and returns a pointer to a NEW COPY
 * of this element. This parameter could not be NULL!
 * @returns Appropriate listReturnCode_t.
**/
returnCode_t LIST_copy(LIST* dest, LIST src, copyFunction_t copyFunction);

/**
 * @brief This function destroys the list and frees all its memory, including that of the data.
 * @param list [IN] is a LIST instance to destroy.
 * @param Appropriate listReturnCode_t.
**/
returnCode_t LIST_destroy(LIST list);

/**
 * @brief This function prints a list to stdout in the format:
 * For each element in the list -
 * [ElementNumber] , [DataPointerName]
 * Summery line -
 * Total number of elements in the list: [listLength]
 * @param list [IN] is a LIST instance to print.
 * @returns Appropriate listReturnCode_t.
**/
returnCode_t LIST_print(LIST list);

#endif //MONEYTOR_LIST_H
