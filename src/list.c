#include "inc/list.h"
#include "inc/list_private.h"

LIST LIST_create(memoryReleaseFunction_t memoryReleaseFunction, getNameFunction_t getNameFunction)
{
    LIST list = (list_t*)malloc(sizeof(*list));
    node_t* firstDummyNode = (node_t*)malloc(sizeof(*firstDummyNode));
    node_t* lastDummyNode = (node_t*)malloc(sizeof(*lastDummyNode));
    if( list == NULL || firstDummyNode == NULL || lastDummyNode == NULL) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        if(list != NULL) {
            free(list);
        }
        if(firstDummyNode != NULL) {
            free(firstDummyNode);
        }
        if(lastDummyNode != NULL) {
            free(lastDummyNode);
        }
        return NULL;
    }
    // Set list params:
    list->length = 0;
    list->memoryReleaseFunc = memoryReleaseFunction;
    list->getDataNameFunc = getNameFunction;
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
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_GETLENGTH_LIST_IS_NULL;
    }
    return list->length;
}

int LIST_removeElement(LIST list, void* pDataElement) {
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_REMOVEELEMENT_LIST_IS_NULL;
    }

    if(list->length == 0) {
        DEBUG_PRINT("Tried to remove node from an empty list.");
        return RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY;
    }

    // Look for the element in the given list:
    node_t* iterator = list->first->next;
    for(int i = 0; i < list->length; ++i) {
        if (iterator->data == pDataElement) {
            //found element!
            break;
        }
        iterator = iterator->next;
    }

    // Verify that we have found it:
    if(iterator == list->last) {
        DEBUG_PRINT("Given element was not found in the list.");
        return RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND;
    }

    // Change connectivity in the list:
    // CR: (DC) Be consistent with your code style. Use AutoFormat (Ctrl+Alt+L) rigorously
    // CR: (SE) I am :) didn't understood what is wrong here :)
    node_t* pPrev = iterator->prev;
    node_t *pNext = iterator->next;
    pPrev->next = pNext;
    pNext->prev = pPrev;

    --list->length;

    // Remove element allocated memory:
    void* pDataToRemove = iterator->data;
    if(pDataToRemove != NULL && list->memoryReleaseFunc(pDataToRemove) != 0) {
        DEBUG_PRINT("Failed to free data memory with the given memory release function.");
        return RETURNCODE_LIST_REMOVEELEMENT_FAILED_TO_FREE_DATA_MEMORY;
    }

    // Set other nodes that contains current data pointer to NULL:
    node_t* anotherIterator = list->first->next;
    for(int i = 0; i < list->length; ++i) {
        if(anotherIterator->data == pDataToRemove) {
            anotherIterator->data = NULL;
        }
    }

    // CR: (DC) After we free a pointer we set it to NULL, to prevent using an invalid pointer afterwards
    // Don't forget to release the node itself!
    free(iterator);
    // CR: (SE) CLion tells me that this line is useless.. what am I missing?
    iterator = NULL;

    // Success!
    return 0;
}

int LIST_addElement(LIST list, void* pData)
{
    // Check that parameters are not null:
    if (list == NULL || pData == NULL) {
        DEBUG_PRINT("LIST and/or pData given are NULL. Please try again.");
        return RETURNCODE_LIST_ADDELEMENT_LIST_OR_PADTA_NULL;
    }

    // Allocate memory to the new node:
    node_t* newNode = (node_t*)malloc(sizeof(*newNode));
    if(newNode == NULL) {
        DEBUG_PRINT("Memory allocation failed in LIST_addElement function. Please try again after memory has been freed.");
        return RETURNCODE_LIST_ADDELEMENT_MEMORY_ALLOCATION_FAILED;
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
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return -1;
    }

    // Iterate over all valid nodes and remove them:
    while(list->length > 0)
    {
        // Try to remove first node:
        if( LIST_removeElement(list, list->first->next->data) != 0) {
            return RETURNCODE_LIST_DESTROY_FAILED_TO_FREE_DATA_MEMORY;
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
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_PRINT_LIST_IS_NULL;
    }

    // Iterate over the list and print elements:
    DEBUG_PRINT("*******************************LIST_print TIME*******************************");
    node_t* iterator = list->first->next;
    int i = 0;
    while(iterator != list->last) {
        ++i;
        char* dataName = (*list->getDataNameFunc)(iterator->data);
        DEBUG_PRINT("[%d], %s ", i, dataName);
        iterator = iterator->next;
    }
    DEBUG_PRINT("Total number of elements in the list: %d", list->length);

    // Check that we are good:
    if(i != list->length) {
        return RETURNCODE_LIST_PRINT_NOT_ALL_ELEMENTS_PRINTED;
    }

    // Success!
    return 0;
}
