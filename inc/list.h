#ifndef MONEYTOR_LIST_H
#define MONEYTOR_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct list list_t;
typedef  list_t* LIST;
typedef int (*memoryReleaseFunction_t)(void*);
typedef char* (*getNameFunction_t)(void*);
typedef void* (*copyFunction_t)(void*);
typedef enum {
    RETURNCODE_LIST_GETLENGTH_LIST_IS_NULL = 1,
    RETURNCODE_LIST_REMOVEELEMENT_LIST_IS_NULL,
    RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY,
    RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND,
    RETURNCODE_LIST_REMOVEELEMENT_FAILED_TO_FREE_DATA_MEMORY,
    RETURNCODE_LIST_ADDELEMENT_LIST_OR_PADTA_NULL,
    RETURNCODE_LIST_ADDELEMENT_MEMORY_ALLOCATION_FAILED,
    RETURNCODE_LIST_DESTROY_FAILED_TO_FREE_DATA_MEMORY,
    RETURNCODE_LIST_PRINT_LIST_IS_NULL,
    RETURNCODE_LIST_PRINT_NOT_ALL_ELEMENTS_PRINTED,
    RETURNCODE_LIST_GETNEXT_LIST_OR_PDATA_NULL,
    RETURNCODE_LIST_GETNEXT_LAST_ELEMENT,
    RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND,
    RETURNCODE_LIST_COPYLIST_SRC_IS_NULL,
    RETURNCODE_LIST_COPYLIST_FAILED_CREATE_NEW_LIST,
    RETURNCODE_LIST_COPYLIST_FAILED_TO_COPY_ELEMENT
    } listReturnCode_t;

/**
 * @brief This function creates a list.
 * It allocates memory for its own usage. When the user finishes using the list, he MUST call LIST_destroy(LIST list)
 * in order to free all allocated memory.
 * ALL data stored in the list must be of the same type!
 * @param memoryReleaseFunc [IN] a pointer to function that releases allocated memory for the data in each node.
 * @param getDataNameFunc [IN] a pointer to a function that prints a unique name to each of the data elements stored in the
 * list.
 * @returns On success - a valid LIST instance, On failure - NULL.
**/
LIST LIST_create(memoryReleaseFunction_t memoryReleaseFunction, getNameFunction_t getNameFunction);

/**
 * @brief This function returns the length of the given list.
 * @param list [IN] is a LIST instance to get it's length.
 * @returns length as a positive number on success, else - appropriate listReturnCode_t.
**/
int LIST_getLength(LIST list);

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
 * @returns On success - 0, else - appropriate listReturnCode_t.
**/
int LIST_getNext(LIST list, void* pDataCurrent, void** pDataNext);

/**
 * @brief This function removes a specified node from the list.
 * It releases the memory allocated for the data of the specified node.
 * @param list [IN,OUT] is a LIST instance to remove from.
 * @param pDataElement [IN] is a pointer to the data element that we want to remove from the list.
 * @returns 0 on success, else - appropriate listReturnCode_t.
**/
int LIST_removeElement(LIST list, void* pDataElement);

/**
 * @brief This function adds a node to the end of the list.
 * It allocates memory for the specified node.
 * @param list [IN,OUT] is a LIST instance whom we add to.
 * @param pData [IN] is a pointer to the data that will be contained in the node.
 * @returns 0 on success, else - appropriate listReturnCode_t.
**/
int LIST_addElement(LIST list, void* pData);

/**
 * @brief This function copies a list from dest to src.
 * If dest is a valid list, it will be destroyed first.
 * All elements in the dest list will be copied and not shared with src list.
 * @param dest [OUT] is a pointer to LIST instance whom we copy to.
 * @param src [IN] is a valid LIST instance whom we copy from, or LIST that is NULL.
 * @param copyFunction [IN] a pointer to a function that gets pointer to data element and returns a pointer to a NEW COPY
 * of this element.
 * @returns 0 on success, else - appropriate listReturnCode_t.
**/
int LIST_copyList(LIST* dest, LIST src, copyFunction_t copyFunction);

/**
 * @brief This function destroys the list and frees all its memory, including that of the data in the nodes.
 * @param list [IN] is a LIST instance to destroy.
 * @param return 0 on success, else - appropriate listReturnCode_t.
**/
int LIST_destroy(LIST list);

/**
 * @brief This function prints a list to stdout in the format:
 * For each node -
 * [NodeNumber] , [DataPointerName]
 * Summery line -
 * Total number of elements in the list: [listLength]
 * @param list [IN] is a LIST instance to print.
 * @returns 0 on success, else - appropriate listReturnCode_t.
**/
int LIST_print(LIST list);

#endif //MONEYTOR_LIST_H
