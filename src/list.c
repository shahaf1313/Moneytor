#include "inc/list.h"

// Private structs and definitions:

typedef struct node node_t;

struct node {
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
    LIST list = NULL;
    // CR: (DC) You have two semicolons at the end
    node_t* firstDummyNode = NULL;;
    node_t* lastDummyNode = NULL;;

    // CR: (DC) In the beginning of any *public* function we MUST make parameter verifications (and you did,
    // CR: (DC) in most functions). Here you should add verifications that the function pointers
    // CR: (DC) received are not NULL. You can verify both in a single if block.

    list = (list_t*) malloc(sizeof(*list));
    if (NULL == list) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        goto cleanup;
    }

    // CR: (DC) You can delete this variable and store the allocated memory directly in list->first
    firstDummyNode = (node_t*) malloc(sizeof(*firstDummyNode));
    if (NULL == firstDummyNode) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        goto cleanup;
    }

    // CR: (DC) You can delete this variable and store the allocated memory directly in list->last
    lastDummyNode = (node_t*) malloc(sizeof(*lastDummyNode));
    if (NULL == lastDummyNode) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        goto cleanup;
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

    // CR: (DC) As the conventions document says, there can only be one return statement in a function
    // CR: (DC) that has a cleanup label, and that return statement must be at the bottom, after the cleanup
    // CR: (DC) section.
    // CR: (DC) This return statement should be deleted.
    // Success!
    return list;

    cleanup:
    // CR: (DC) Remember that after freeing a pointer we want to set it back to NULL, to prevent using
    // CR: (DC) a dangling pointer.
    // CR: (DC) Also, notice the code duplication between these three deallocations.
    // CR: (DC) As we highly dislike code duplication, extract a macro, called FREE(p) that does the following:
    // CR: (DC) 1. Test if the pointer is NULL
    // CR: (DC) 2. If so, calls free() on it
    // CR: (DC) 3. Sets the pointer to NULL
    // CR: (DC) Although a function is preferrable over a macro, this FREE(p) macro can't be written as a
    // CR: (DC) function, as it won't be able to set the pointer to NULL.
    if (NULL != list) {
        free(list);
    }
    if (NULL != firstDummyNode) {
        free(firstDummyNode);
    }
    if (NULL != lastDummyNode) {
        free(lastDummyNode);
    }
    return NULL;

}

returnCode_t LIST_getLength(LIST list, int* length) {
    if (NULL == list) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_GETLENGTH_LIST_IS_NULL;
    }
    *length = list->length;
    return RETURNCODE_SUCCESS;
}

void* LIST_getFirst(LIST list) {
    if (NULL == list) {
        return NULL;
    // CR: (DC) No need for the else, as you make a return in the first if block
    // CR: (DC) Can be restructured:
    // CR: (DC) if (NULL == list) {
    // CR: (DC)     return NULL;
    // CR: (DC) }
    // CR: (DC) if (0 == list->length) { ...
    // CR: (DC) This technique is also called "early return" or "early exit"
    } else if (0 == list->length) {
        return NULL;
    }
    return list->first->next->data;
}

void* LIST_getLast(LIST list) {
    if (NULL == list) {
        return NULL;
    } else if (0 == list->length) {
        return NULL;
    }
    return list->last->prev->data;
}

returnCode_t LIST_getNext(LIST list, void* pDataCurrent, void** pDataNext) {
    if (NULL == list) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_GETNEXT_LIST_NULL;
    }

    // CR: (DC) Comment is not needed. If a function call has to be commented, it means the function name
    // CR: (DC) isn't informative enough.
    // CR: (DC) I want to explain why I always tell you to remove comments. Some developers think comments
    // CR: (DC) are good practice, and that it will make the next developer's life easier, by explaining
    // CR: (DC) the code for him. That's all true, comments do help the next developer understand the code
    // CR: (DC) better. However, comments have a single property that make them very dangerous. Yes, you
    // CR: (DC) read it right: DANGEROUS. I bet you think "What?! How comments can be dangerous? They
    // CR: (DC) don't even compile!". And THAT is the same thing that makes them dangerous:
    // CR: (DC) They don't compile. The preprocessor deletes all comments before compiling. The outcome
    // CR: (DC) of this fact is that *comments rot*. Like food in the fridge that gets forgotten, comments
    // CR: (DC) rot over time. Why is that you ask? Because code is dynamic, it changes, we add a feature,
    // CR: (DC) fix a bug, refactor this, rename that. Just look at your list module, how much it have
    // CR: (DC) changed over just a few days. Whenever we change code, for example, rename a function,
    // CR: (DC) every use of that function *won't work* unless we rename the usages to use the new name.
    // CR: (DC) Why is that? Because the compiler (and the IDE) CARES about code, and checks that it's used
    // CR: (DC) correctly. While code changes are maintained by the compiler and the IDE, COMMENTS DON'T.
    // CR: (DC) It's very common to find a comment like that:
    // CR: (DC)     // Print the list
    // CR: (DC)     // LIST_copy(list1, list2)
    // CR: (DC) This was probably caused by a developer changing the call from LIST_print to LIST_copy,
    // CR: (DC) because of a bug or a new feature. However, because the comment is not checked by the compiler
    // CR: (DC) the developer simply overlook the comment and didn't change it along with the code. Nobody
    // CR: (DC) cares. It's so easy to overlook comments as the IDE paints them grey.
    // CR: (DC) The next developer that comes along won't understand how LIST_copy prints the list, and it will
    // CR: (DC) only make him more confused.
    // CR: (DC) Thus, comments are good at explaining your code, but it's a double edged sword.
    // CR: (DC) A better way to make your code more readable is by naming your functions with good,
    // CR: (DC) informative names, naming your variables with informative names, and structuring your code
    // CR: (DC) in a way that it will be readable. Comments should be used to explain code only when the code
    // CR: (DC) is really complicated and requires a delicate understanding of how the code was designed,
    // CR: (DC) and why certain choices were made when writing the code. Most of the time, this is not the case.
    // Iterate over the list and search for pDataCurrent:
    // CR: (DC) nodeSearcher can be const
    node_t* nodeSearcher = list_getNode(list, pDataCurrent);

    // Check if we found element:
    if (nodeSearcher == list->last) {
        *pDataNext = NULL;
        return RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND;
    }

    // Check if the element is last in the list (and thus has no next element):
    if (nodeSearcher->next == list->last) {
        *pDataNext = NULL;
        return RETURNCODE_LIST_GETNEXT_LAST_ELEMENT;
    }

    // Set pDataNext to the correct data element pointer:
    *pDataNext = nodeSearcher->next->data;
    return RETURNCODE_SUCCESS;
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

    // Don't forget to release the node itself!
    free(nodeToRemove);
    nodeToRemove = NULL;

    // Success!
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
    free(list->first);
    free(list->last);
    free(list);
    list = NULL;

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