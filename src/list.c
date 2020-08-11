#include "inc/list.h"

// Private structs and definitions:
struct node_s {
    node_t* prev;
    node_t* next;
    void* data;
};

struct list {
    memoryReleaseFunction_t memoryReleaseFunc;
    getNameFunction_t getDataNameFunc;
    node_t* first;
    node_t* last;
    int length;
};

//Functions:

/**
 * @brief This private function iterates over a list and searches for a specific node that contains the input data
 * element. if not found, it just returns a pointer to the last dummy node (list->last).
 * @param list [IN] list to iterate.
 * @param data [OUT] pointer to the element we are searching for.
 * @returns pointer to the node in the list (if not found, then a pointer to list->last).
 */
static node_t* list_getNode(LIST list, void* data);

LIST LIST_create(memoryReleaseFunction_t memoryReleaseFunction, getNameFunction_t getNameFunction) {

    if ((NULL == memoryReleaseFunction) || (NULL == getNameFunction)) {
        DEBUG_PRINT("Memory release function or get name function are NULL. Please try again.");
        return NULL;
    }

    bool isMemoryAllocatedSuccessfully = false;
    LIST list = NULL;
    list = (list_t*) malloc(sizeof(*list));
    if (NULL == list) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        goto cleanup;
    }

    list->first = (node_t*) malloc(sizeof(*list->first));
    if (NULL == list->first) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        goto cleanup;
    }

    list->last = (node_t*) malloc(sizeof(*list->last));
    if (NULL == list->last) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        goto cleanup;
    }

    isMemoryAllocatedSuccessfully = true;
    list->length = 0;
    list->memoryReleaseFunc = memoryReleaseFunction;
    list->getDataNameFunc = getNameFunction;

    // Set first dummy node:
    list->first->prev = NULL;
    list->first->next = list->last;
    list->first->data = NULL;

    // Set last dummy node:
    list->last->prev = list->first;
    list->last->next = NULL;
    list->last->data = NULL;

cleanup:
    if (!isMemoryAllocatedSuccessfully) {
        LIST_destroy(list);
    }
    return list;
}

returnCode_t LIST_getLength(LIST list, int* length) {
    if ((NULL == list) || (NULL == length)) {
        DEBUG_PRINT("LIST or pointer to length given are NULL. Please try again.");
        return RETURNCODE_LIST_GETLENGTH_LIST_OR_LENGTH_POINTER_ARE_NULL;
    }
    *length = list->length;
    return RETURNCODE_SUCCESS;
}

node_t* LIST_getFirst(LIST list) {
    if (NULL == list) {
        return NULL;
    }
    if (0 == list->length) {
        return NULL;
    }
    return list->first->next;
}

node_t* LIST_getLast(LIST list) {
    if (NULL == list) {
        return NULL;
    }
    if (0 == list->length) {
        return NULL;
    }
    return list->last->prev;
}

returnCode_t LIST_find(LIST list, void* data, compareFunction_t compareFunction, bool* dataFound, node_t** nodeOfFoundData) {
    if ((NULL == list) || (NULL == compareFunction) || (NULL == dataFound) || (NULL == nodeOfFoundData)) {
        DEBUG_PRINT("LIST or one of the other parameters given are NULL. Please try again.");
        return RETURNCODE_LIST_FIND_PARAMETERS_NULL;
    }
    *dataFound = false;
    *nodeOfFoundData = NULL;
    node_t* pNodeIterator = NULL;
    for (pNodeIterator = list->first->next; pNodeIterator != list->last; pNodeIterator = pNodeIterator->next) {
        if (0 == compareFunction(data, pNodeIterator->data)) {
            *dataFound = true;
            *nodeOfFoundData = pNodeIterator;
            break;
        }
    }
    return RETURNCODE_SUCCESS;
}

returnCode_t LIST_getNext(LIST list, node_t* currentNode, node_t** nextNode) {
    if ((NULL == list) || (NULL == currentNode) || (NULL == nextNode)) {
        DEBUG_PRINT("LIST or pointer to one of the nodes given are NULL. Please try again.");
        return RETURNCODE_LIST_GETNEXT_LIST_OR_NODE_NULL;
    }

    // Check if the element is last in the list (and thus has no next element):
    if (currentNode->next == list->last) {
        *nextNode = NULL;
        return RETURNCODE_LIST_GETNEXT_LAST_ELEMENT;
    }

    *nextNode = currentNode->next;
    return RETURNCODE_SUCCESS;
}

returnCode_t LIST_getPrev(LIST list, node_t* currentNode, node_t** prevNode) {
    if ((NULL == list) || (NULL == currentNode) || (NULL == prevNode)) {
        DEBUG_PRINT("LIST or pointer to one of the nodes given are NULL. Please try again.");
        return RETURNCODE_LIST_GETPREV_LIST_OR_NODE_NULL;
    }

    // Check if the element is first in the list (and thus has no previous element):
    if (currentNode->prev == list->first) {
        *prevNode = NULL;
        return RETURNCODE_LIST_GETPREV_FIRST_ELEMENT;
    }

    *prevNode = currentNode->prev;
    return RETURNCODE_SUCCESS;
}

void* LIST_getData(node_t* node) {
    if (NULL == node) {
        return NULL;
    }
    return  node->data;
}

returnCode_t LIST_removeElement(LIST list, void* pDataElement) {
    if (NULL == list) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_REMOVEELEMENT_LIST_IS_NULL;
    }

    if (0 == list->length) {
        DEBUG_PRINT("Tried to remove node from an empty list.");
        return RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY;
    }

    // Look for the element in the given list:
    node_t* nodeToRemove = list_getNode(list, pDataElement);

    // Verify that we have found it:
    if (nodeToRemove == list->last) {
        DEBUG_PRINT("Given element was not found in the list.");
        return RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND;
    }

    // Change connectivity in the list:
    nodeToRemove->prev->next = nodeToRemove->next;
    nodeToRemove->next->prev = nodeToRemove->prev;

    --list->length;

    // Remove element allocated memory:
    void* pDataToRemove = nodeToRemove->data;
    if (0 != list->memoryReleaseFunc(pDataToRemove)) {
        DEBUG_PRINT("Failed to free data memory with the given memory release function.");
        return RETURNCODE_LIST_REMOVEELEMENT_FAILED_TO_FREE_DATA_MEMORY;
    }
    FREE(nodeToRemove);
    return RETURNCODE_SUCCESS;
}

returnCode_t LIST_addElement(LIST list, void* pData) {
    // Check that parameters are not null:
    if (NULL == list) {
        DEBUG_PRINT("LIST and/or pData given are NULL. Please try again.");
        return RETURNCODE_LIST_ADDELEMENT_LIST_NULL;
    }

    // Allocate memory to the new node:
    node_t* newNode = (node_t*) malloc(sizeof(*newNode));
    if (NULL == newNode) {
        DEBUG_PRINT(
                "Memory allocation failed in LIST_addElement function. Please try again after memory has been freed.");
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
    ++list->length;

    // Success!
    return RETURNCODE_SUCCESS;
}

returnCode_t LIST_copy(LIST* dest, LIST src, copyFunction_t copyFunction) {
    if((NULL == copyFunction) || (NULL == src) || (NULL == dest)) {
        DEBUG_PRINT("One of the parameters in LIST_copy is NULL. They should all be valid pointers.");
        return RETURNCODE_LIST_COPY_PARAMETER_NULL;
    }
    *dest = LIST_create(src->memoryReleaseFunc, src->getDataNameFunc);
    if (NULL == *dest) {
        DEBUG_PRINT("Could not create new LIST instance in LIST_copyList.");
        return RETURNCODE_LIST_COPY_FAILED_CREATE_NEW_LIST;
    }
    node_t* srcListIterator;
    for (srcListIterator = src->first->next; srcListIterator != src->last; srcListIterator = srcListIterator->next) {
        void* pNewElement = copyFunction(srcListIterator->data);

        if (NULL == pNewElement && NULL != srcListIterator->data) {
            DEBUG_PRINT("Could not copy data element via copyFunction supplied. Not all elements copied!");
            return RETURNCODE_LIST_COPY_FAILED_TO_COPY_ELEMENT;
        }

        if (RETURNCODE_SUCCESS != LIST_addElement(*dest, pNewElement)) {
            DEBUG_PRINT("Adding new element to the new list failed. Not all elements copied!");
            return RETURNCODE_LIST_COPY_FAILED_TO_ADD_ELEMENT;
        }
    }

    // Success!
    return RETURNCODE_SUCCESS;
}

returnCode_t LIST_destroy(LIST list) {
    if (NULL == list) {
        return RETURNCODE_SUCCESS;
    }

    // Iterate over all valid nodes and remove them:
    while (list->length > 0) {
        // Try to remove first node:
        if (0 != LIST_removeElement(list, list->first->next->data)) {
            return RETURNCODE_LIST_DESTROY_FAILED_TO_FREE_DATA_MEMORY;
        }
    }

    // Now, free dummy nodes and list memory allocations:
    FREE(list->first);
    FREE(list->last);
    FREE(list);

    // Success!
    return RETURNCODE_SUCCESS;
}

returnCode_t LIST_print(LIST list) {
    if (NULL == list) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_PRINT_LIST_IS_NULL;
    }

    // Iterate over the list and print elements:
    DEBUG_PRINT("*******************************LIST_print TIME*******************************");
    node_t* iterator = list->first->next;
    int i = 0;
    while (iterator != list->last) {
        ++i;
        char* dataName = (*list->getDataNameFunc)(iterator->data);
        DEBUG_PRINT("[%d], %s ", i, dataName);
        iterator = iterator->next;
    }
    DEBUG_PRINT("Total number of elements in the list: %d", list->length);

    // Check that we are good:
    if (i != list->length) {
        return RETURNCODE_LIST_PRINT_NOT_ALL_ELEMENTS_PRINTED;
    }

    // Success!
    return RETURNCODE_SUCCESS;
}

static node_t* list_getNode(LIST list, void* data) {
    node_t* iterator;
    for (iterator = list->first->next; iterator != list->last; iterator = iterator->next) {
        if (iterator->data == data) {
            break;
        }
    }
    return iterator;
}