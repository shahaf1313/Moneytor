#include "list.h"

LIST LIST_create(int (*memoryReleaseFunc)(void*), char* (*getDataNameFunc)(void*))
{
    LIST list = (list_t*)malloc(sizeof(*list));
    node_t* firstDummyNode = (node_t*)malloc(sizeof(*firstDummyNode));
    node_t* lastDummyNode = (node_t*)malloc(sizeof(*lastDummyNode));
    if( list == NULL || firstDummyNode == NULL || lastDummyNode == NULL) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.\n");
        return NULL;
    }
    // Set list params:
    list->length = 0;
    list->memoryReleaseFunc = memoryReleaseFunc;
    list->getDataNameFunc = getDataNameFunc;
    list->first = firstDummyNode;
    list->last = lastDummyNode;

    // Set first dummy node:
    firstDummyNode->prev = NULL;
    firstDummyNode->next = lastDummyNode;
    firstDummyNode->data = NULL;

    // Set last dummy node:
    lastDummyNode->prev = firstDummyNode;
    lastDummyNode->next = NULL;
    lastDummyNode->data = NULL;

    // Success!
    return list;
}

int LIST_getLength(LIST list) {
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.\n");
        return -1;
    }
    return list->length;
}

int LIST_removeElement(LIST list, void* pDataElement) {
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.\n");
        return -1;
    }

    // Check if the list is not empty:
    if(list->length == 0) {
        DEBUG_PRINT("Tried to remove node from an empty list.\n");
        return -2;
    }

    // Look for the element in the given list:
    node_t* iterator = list->first->next;
    while (iterator != list->last) {
        if(iterator->data == pDataElement) {
            //found element!
            break;
        }
        iterator = iterator->next;
    }

    // Verify that we have found it:
    if(iterator == list->last) {
        DEBUG_PRINT("Given element was not found in the list.\n");
        return -3;
    }

    // Change connectivity in the list:
    node_t* pPrev = iterator->prev;
    node_t *pNext = iterator->next;
    pPrev->next = pNext;
    pNext->prev = pPrev;

    // Update list length:
    --(list->length);

    // Remove element allocated memory:
    if( (*list->memoryReleaseFunc)(iterator->data) != 0) {
        DEBUG_PRINT("Failed to free data memory with the given memory release function.\n");
        return -4;
    }

    // Don't forget to release the node itself!
    free(iterator);

    // Success!
    return 0;
}

int LIST_addElement(LIST list, void* pData)
{
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.\n");
        return -1;
    }

    // Check if pData is NULL:
    if (pData == NULL) {
        DEBUG_PRINT("pData given is NULL. Please try again.\n");
        return -1;
    }
    // Check if data element is ALREADY in the list:
    node_t* iterator = list->first->next;
    while (iterator != list->last) {
        if(iterator->data == pData) {
            DEBUG_PRINT("Data element is ALREADY in the list! He was not added again.\n");
            return -2;
        }
        iterator = iterator->next;
    }

    // Allocate memory to the new node:
    node_t* newNode = (node_t*)malloc(sizeof(*newNode));
    if(newNode == NULL) {
        DEBUG_PRINT("Memory allocation failed in LIST_addElement function. Please try again after memory has been freed.\n");
        return -3;
    }

    // Point to the data:
    newNode->data = pData;

    // Set node in the end of the list:
    node_t* lastValidNode = list->last->prev;
    lastValidNode->next = newNode;
    newNode->prev = lastValidNode;
    newNode->next = list->last;
    list->last->prev = newNode;

    // Update list length:
    ++(list->length);

    // Success!
    return 0;
}

int LIST_destroy(LIST list)
{
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.\n");
        return -1;
    }

    // Iterate over all valid nodes and remove them:
    while(list->first->next != list->last)
    {
        // Try to remove first node:
        if( LIST_removeElement(list, list->first->next->data) != 0) {
            return -2;
        }
    }

    // Now, free dummy nodes and list memory allocations:
    free(list->first);
    free(list->last);
    free(list);

    // Success!
    return 0;
}

int LIST_print(LIST list)
{
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.\n");
        return -1;
    }

    // Iterate over the list and print elements:
    DEBUG_PRINT("*******************************LIST_print TIME*******************************\n");
    node_t* iterator = list->first->next;
    int i = 0;
    while(iterator != list->last)
    {
        ++i;
        char* dataName = (*list->getDataNameFunc)(iterator->data);
        DEBUG_PRINT("[%d], %s \n", i, dataName);
        iterator = iterator->next;
    }
    DEBUG_PRINT("Total number of elements in the list: %d\n", list->length);

    // Check that we are good:
    if(i != list->length)
    {
        return -2;
    }

    // Success!
    return 0;
}
