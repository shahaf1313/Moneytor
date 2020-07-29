#include "list.h"

// CR: (DC) Create typedefs for these function types
// CR: (DC) For example:
// CR: (DC) typedef int (*memory_release_function_t)(void*)
// CR: (DC) This creates the type memory_release_function_t
LIST LIST_create(int (*memoryReleaseFunc)(void*), char* (*getDataNameFunc)(void*))
{
    LIST list = (list_t*)malloc(sizeof(*list));
    node_t* firstDummyNode = (node_t*)malloc(sizeof(*firstDummyNode));
    node_t* lastDummyNode = (node_t*)malloc(sizeof(*lastDummyNode));
    if( list == NULL || firstDummyNode == NULL || lastDummyNode == NULL) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.\n");
        // CR: (DC) If one allocation succeeded and the rest failed, what happens with the
        // CR: (DC) memory successfully allocated? It is leaked
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
        // CR: (DC) Notice how you always add \n to DEBUG_PRINT. Seems like you are repeating yourself,
        // CR: (DC) and that's not something we like to do
        // CR: (DC) Make DEBUG_PRINT add the \n for us
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

    // CR: (DC) No need for comments that don't add information
    // Check if the list is not empty:
    if(list->length == 0) {
        DEBUG_PRINT("Tried to remove node from an empty list.\n");
        return -2;
    }

    // CR: (DC) Can be converted to a for loop
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
        // CR: (DC) Notice how you're using numbers to indicate errors in the code, which is very
        // CR: (DC) cumbersome. A better technique is to create an enum, called return_code_t,
        // CR: (DC) that will contain constants used to indicate errors
        // CR: (DC) For example, the constant that will be used in the line below will be:
        // CR: (DC) return RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND
        return -3;
    }

    // Change connectivity in the list:
    // CR: (DC) Be consistent with your code style. Use AutoFormat (Ctrl+Alt+L) rigorously
    node_t* pPrev = iterator->prev;
    node_t *pNext = iterator->next;
    pPrev->next = pNext;
    pNext->prev = pPrev;

    // CR: (DC) No need for useless comments and useless parentheses
    // Update list length:
    --(list->length);

    // Remove element allocated memory:
    // CR: (DC) You can omit the * and () around the function pointer
    if( (*list->memoryReleaseFunc)(iterator->data) != 0) {
        DEBUG_PRINT("Failed to free data memory with the given memory release function.\n");
        return -4;
    }

    // CR: (DC) After we free a pointer we set it to NULL, to prevent using an invalid pointer afterwards
    // Don't forget to release the node itself!
    free(iterator);

    // Success!
    return 0;
}

int LIST_addElement(LIST list, void* pData)
{
    // CR: (DC) Unify all parameter checks to a single if block
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
    // CR: (DC) Why is that? Let me add duplicates if I wish to
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
    // CR: (DC) Can use LIST_getLength here
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
