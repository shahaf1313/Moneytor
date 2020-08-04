#include "inc/list.h"

// Private structs and definitions:

// CR: (SE) This line is duplicated from moneytorDefinitions.h in order to allow separate debugging printer of each
// CR: (SE) module (list is not a part of moneytor and vice versa).
// CR: (DC) You can move this into a file we usually call common.h, that holds common macros and definitions,
// CR: (DC) such as this. Then, if you would need to take list module into a new project, you would simply
// CR: (DC) copy the common.h file as well.
#define DEBUG_PRINT(...) {printf(__VA_ARGS__); printf("\n");}

typedef struct node node_t;

struct node {
    // CR: (DC) You can use note_t here instead of struct node
    struct node* prev;
    struct node* next;
    void* data;
};

struct list {
    // CR: (DC) Use your function typedefs
    int (*memoryReleaseFunc)(void*);
    char* (*getDataNameFunc)(void*);
    node_t* first;
    // CR: (DC) You can use note_t here instead of struct node
    struct node* last;
    int length;
};

//Functions:

LIST LIST_create(memoryReleaseFunction_t memoryReleaseFunction, getNameFunction_t getNameFunction) {
    // CR: (DC) This is a very ugly way to manage your memory allocations
    // CR: (DC) Read in the conventions doc:
    // CR: (DC) https://drive.google.com/file/d/1f9L1B73Hfx96owAKfRbIWIrNkcpvXFXg/view?usp=sharing
    // CR: (DC) What is the preferred way to manage memory inside a function.
    LIST list = (list_t*) malloc(sizeof(*list));
    node_t* firstDummyNode = (node_t*) malloc(sizeof(*firstDummyNode));
    node_t* lastDummyNode = (node_t*) malloc(sizeof(*lastDummyNode));
    if (list == NULL || firstDummyNode == NULL || lastDummyNode == NULL) {
        DEBUG_PRINT("Memory allocation failed in LIST_create function. Please try again after memory has been freed.");
        if (list != NULL) {
            free(list);
        }
        if (firstDummyNode != NULL) {
            free(firstDummyNode);

        }
        if (lastDummyNode != NULL) {
            free(lastDummyNode);
        }
        list = firstDummyNode = lastDummyNode = NULL;
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
    // CR: (DC) Comment is so unnecessary. Any programmer can tell you're checking that list is not NULL
    // Check if list is not null:
    // CR: (DC) Flip the if (use Yoda style), like that: if (NULL == list)
    // CR: (DC) Do you understand why Yoda style is important in C?
    // CR: (DC) If not, consult me
    // CR: (DC) Do this for all if conditions
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_GETLENGTH_LIST_IS_NULL;
    }
    return list->length;
}

void* LIST_getFirst(LIST list) {
    if (list == NULL) {
        return NULL;
    }
    else if (list->length == 0) {
        return NULL;
    }
    return list->first->next->data;
}

void* LIST_getLast(LIST list) {
    if (list == NULL) {
        return NULL;
    }
    else if (list->length == 0) {
        return NULL;
    }
    return list->last->prev->data;
}

int LIST_getNext(LIST list, void* pDataCurrent, void** pDataNext) {
    // CR: (DC) Again, comment is unnecessary. Remember we are all developers, don't explain the obvious
    // Check that parameters are not null:
    // CR: (DC) Put each condition in its own parentheses, to avoid confusion regarding the order in which
    // CR: (DC) operators are evaluated
    if (list == NULL || pDataCurrent == NULL) {
        DEBUG_PRINT("LIST and/or pDataCurrent given are NULL. Please try again.");
        // CR: (DC) This line does absolutely nothing, and it's extremely important you understand why.
        // CR: (DC) It may seem petty, but it has a great meaning in understanding how C functions work
        pDataNext = NULL;
        return RETURNCODE_LIST_GETNEXT_LIST_OR_PDATA_NULL;
    }

    // Iterate over the list and search for pDataCurrent:
    node_t* iterator = list->first->next;
    // CR: (DC) You can omit the variable i and stop looping when iterator == list->last
    // CR: (DC) When I said you can convert to a for loop, I didn't mean to introduce a new index variable
    // CR: (DC) I meant simply to change to a for loop. For example:
    // CR: (DC) for (node_t* iterator = list->first->next; iterator != list->last; iterator = iterator->next)
    // CR: (DC) The iterator variable of a for loop doesn't have to be int
    for (int i = 0; i < list->length; ++i){
        if (iterator->data == pDataCurrent) {
            break;
        }
        iterator = iterator->next;
    }

    // Check if we found element:
    if (iterator == list->last) {
        // CR: (DC) Usually, in case of error, like this, we prefer not to alter the pointers given to us
        // CR: (DC) as parameters. We will alter them only if there's no choice, and it will be documented.
        *pDataNext = NULL;
        return RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND;
    }

    // Check if the element is last in the list (and thus has no next element):
    if (iterator->next == list->last) {
        *pDataNext = NULL;
        return RETURNCODE_LIST_GETNEXT_LAST_ELEMENT;
    }

    // Set pDataNext to the correct data element pointer:
    *pDataNext = iterator->next->data;
    return 0;
}

int LIST_removeElement(LIST list, void* pDataElement) {
    // CR: (DC) Thanks, captain obvious
    // Check if list is not null:
    if (list == NULL) {
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return RETURNCODE_LIST_REMOVEELEMENT_LIST_IS_NULL;
    }

    if (list->length == 0) {
        DEBUG_PRINT("Tried to remove node from an empty list.");
        return RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY;
    }

    // CR: (DC) Notice the code duplication between this snippet and the very similar snippet in
    // CR: (DC) LIST_getNext. In both cases, you're searching for the node holding a specific data element.
    // CR: (DC) As we really dislike code duplication, extract it to a function that returns the node
    // CR: (DC) pointing to a specific data element. This new function may be private (static, in C).
    // CR: (DC) Private function don't appear in the header file and are marked with lowercase module letters
    // CR: (DC) For example: static node_t* list_getNode(LIST list, void* data)
    // Look for the element in the given list:
    node_t* iterator = list->first->next;
    for (int i = 0; i < list->length; ++i) {
        if (iterator->data == pDataElement) {
            //found element!
            break;
        }
        iterator = iterator->next;
    }

    // Verify that we have found it:
    if (iterator == list->last) {
        DEBUG_PRINT("Given element was not found in the list.");
        return RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND;
    }

    // Change connectivity in the list:
    // CR: (DC) You can inline these and do: iterator->prev->next = iterator->next, for example
    // CR: (DC) If you don't want to, that's okay. In that case, pPrev and pNext can be const
    // CR: (DC) For example, const int x = 5;
    node_t* pPrev = iterator->prev;
    node_t* pNext = iterator->next;
    pPrev->next = pNext;
    pNext->prev = pPrev;

    --list->length;

    // Remove element allocated memory:
    // CR: (DC) If the user called LIST_addElement(list1, NULL), then the user had a good reason to add NULL
    // CR: (DC) to the list, for some reason he wants to store the value NULL.
    // CR: (DC) As the user called LIST_addElement once, he expects the release function to be called once.
    // CR: (DC) Generally, if LIST_addElement was called X times, the release function should be called X
    // CR: (DC) times. By testing the data for NULL, you break this contract.
    // CR: (DC) Besides, if the user inserted NULL to the list, it's his responsibility to provide a
    // CR: (DC) release function that can handle NULL. This is true for all values he inserts to the list,
    // CR: (DC) he must provide a release function that can handle anything he inserts.
    // CR: (DC) Don't check the data to remove for NULL, just call the removal function.
    void* pDataToRemove = iterator->data;
    // CR: (DC) As we can see, if the release function returns 0 it's considered an error
    // CR: (DC) This is OK, but should be documented in the function that accepts this release function.
    if (pDataToRemove != NULL && list->memoryReleaseFunc(pDataToRemove) != 0) {
        DEBUG_PRINT("Failed to free data memory with the given memory release function.");
        return RETURNCODE_LIST_REMOVEELEMENT_FAILED_TO_FREE_DATA_MEMORY;
    }

    // CR: (DC) Don't do that! Don't decide for the user what should happen with his other data!
    // CR: (DC) Remember you're only a storage module.
    // CR: (DC) Also, consider that maybe the stored data isn't really allocated, maybe I'm using your list
    // CR: (DC) to store ints (I'm casting them to void*) and I provided a release function that prints
    // CR: (DC) the (so-called) "released" integer. I expect every integer I added to list to be printed,
    // CR: (DC) and you break this expectation. Bottom line, don't assume anything on your user's data.
    // Set other nodes that contains current data pointer to NULL:
    node_t* anotherIterator = list->first->next;
    for (int i = 0; i < list->length; ++i) {
        if (anotherIterator->data == pDataToRemove) {
            anotherIterator->data = NULL;
        }
    }

    // Don't forget to release the node itself!
    free(iterator);
    iterator = NULL;

    // Success!
    return 0;
}

int LIST_addElement(LIST list, void* pData) {
    // Check that parameters are not null:
    if (list == NULL || pData == NULL) {
        DEBUG_PRINT("LIST and/or pData given are NULL. Please try again.");
        return RETURNCODE_LIST_ADDELEMENT_LIST_OR_PADTA_NULL;
    }

    // Allocate memory to the new node:
    node_t* newNode = (node_t*) malloc(sizeof(*newNode));
    if (newNode == NULL) {
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
    ++(list->length);

    // Success!
    return 0;
}

int LIST_copyList(LIST* dest, LIST src, copyFunction_t copyFunction) {
    if (src == NULL) {
        DEBUG_PRINT("Parameter src in LIST_copyList is NULL.");
        return RETURNCODE_LIST_COPYLIST_SRC_IS_NULL;
    }
    if (*dest != NULL) {
        int returnCode = LIST_destroy(*dest);
        if (returnCode != 0) {
            DEBUG_PRINT("Could not destroy dest list (which is not NULL) in LIST_copyList. Proper return code has been "
                        "returned.");
            return returnCode;
        }
    }
    *dest = LIST_create(src->memoryReleaseFunc, src->getDataNameFunc);
    if (*dest == NULL) {
        DEBUG_PRINT("Could not create new LIST instance in LIST_copyList.");
        return RETURNCODE_LIST_COPYLIST_FAILED_CREATE_NEW_LIST;
    }
    node_t* srcListIterator = src->first->next;
    for (int i = 0; i < src->length; ++i) {
        void* pNewElement = copyFunction(srcListIterator->data);
        if(pNewElement == NULL && srcListIterator->data != NULL) {
            DEBUG_PRINT("Could not copy data element via copyFunction supplied. Not all elements copied!");
            return RETURNCODE_LIST_COPYLIST_FAILED_TO_COPY_ELEMENT;
        }
        // CR: (DC) You don't check the return value?!
        LIST_addElement(*dest, pNewElement);
        srcListIterator = srcListIterator->next;
    }

    // Success!
    return 0;
}

int LIST_destroy(LIST list) {
    // Check if list is not null:
    if (list == NULL) {
        // CR: (DC) This shouldn't be considered an error. We can support passing NULL to LIST_destroy,
        // CR: (DC) and simply do nothing.
        DEBUG_PRINT("LIST given is NULL. Please try again.");
        return -1;
    }

    // Iterate over all valid nodes and remove them:
    while (list->length > 0) {
        // Try to remove first node:
        if (LIST_removeElement(list, list->first->next->data) != 0) {
            return RETURNCODE_LIST_DESTROY_FAILED_TO_FREE_DATA_MEMORY;
        }
    }

    // Now, free dummy nodes and list memory allocations:
    free(list->first);
    free(list->last);
    free(list);
    list = NULL;

    // Success!
    return 0;
}

int LIST_print(LIST list) {
    // CR: (DC) Captain obvious to the rescue
    // Check if list is not null:
    if (list == NULL) {
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
    return 0;
}
