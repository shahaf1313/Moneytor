#include "list.h"

// CR: (DC) Do not duplicate documentation. It the function appears in the header file,
// CR: (DC) there's no need to document it in the source file
/* This function initializes the list.
 * on success returns 0, otherwise a another number */
list_t* initList(void)
{
    // CR: (DC) Use sizeof(*list). This way, if we change list's type, it will still work
    list_t *list = (list_t*)malloc(sizeof(list_t));
    if( list == NULL)
    {
        // CR: (DC) What happens if tomorrow we want to use perror() to print the debug message?
        // CR: (DC) Create a macro DEBUG_PRINT that prints a debug message
        printf("Memory allocation failed. Plase try again after memory has been freed.\n");
        return NULL;
    }
    // CR: (DC) Note the warnings here. Why are they here?
    list->counter = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

/* This function removes a specified entry from the list.
 * on success returns 0, otherwise a another number */
int removeElement(list_t *list, node_t* pElement)
{
    // CR: (DC) Notice how hard it is for you to remove or add elements, and it stems from the fact
    // CR: (DC) that the removed element may be the first or the last (or both)
    // CR: (DC) A common practice to make maintaining the list easier is to use dummy nodes
    // CR: (DC) So we create two dummy nodes that will be the first and last nodes
    // CR: (DC) This way, the removed node will always be in the middle of the list, and we will have only
    // CR: (DC) one case to deal with.
    if(list->counter == 0)
    {
        printf("Tried to remove entry from a list with 0 elements.\n");
        return -1;
    }
    else if( list->counter == 1)
    {
        list->last = NULL;
        list->first = NULL;
    }
    else if(pElement == list->first)
    {
        node_t *newFirst = pElement->next;
        list->first = newFirst;
        newFirst->prev = NULL;
    }
    else if(pElement == list->last)
    {
        node_t *newLast = pElement->prev;
        list->last = newLast;
        newLast->next = NULL;
    }
    else
    {
        node_t *pPrev = pElement->prev;
        node_t *pNext = pElement->next;
        pPrev->next = pNext;
        pNext->prev = pPrev;
    }
    /*Remove element allocated memory:*/
    if(pElement->pSubDirList != NULL)
    {
        deleteList(pElement->pSubDirList);
    }
    free(pElement);
    --(list->counter);
    return 0;
}

/* This function adds an entry to the end of the list.
 * Sub directory will not be initialized with files in subDirList, but with NULL instead.
 * on success returns 0, otherwise a another number */
int addElement(list_t *list, char* name, time_t lastChanged, char *fileType)
{
    node_t *newNode = (node_t*)malloc(sizeof(node_t));
    if(newNode == NULL) { return -1;}
    newNode->lastChanged = lastChanged;
    newNode->pSubDirList = NULL;
    strcpy(newNode->entryName, name);
    strcpy(newNode->fileType, fileType);
    if(list->counter == 0)
    {
        list->first = newNode;
        list->last = newNode;
        newNode->prev = NULL;
        newNode->next = NULL;
    }
    else
    {
        node_t* tmp = list->last;
        newNode->prev = tmp;
        newNode->next = NULL;
        tmp->next = newNode;
        list->last = newNode;
    }
    ++(list->counter);
    return 0;
}

/* This function copies a list from src and returns it's new copy via a pointer.
 * on success returns a valid pointer, otherwise NULL */
list_t* copyList(list_t *src)
{
    list_t *pNewList = initList();
    if(pNewList == NULL) { return NULL; }
    node_t *srcIt = src->first;
    while(srcIt != NULL)
    {
        addElement(pNewList, srcIt->entryName, srcIt->lastChanged, srcIt->fileType);
        srcIt = srcIt->next;
    }
    return pNewList;
}

/* This function deletes list and frees all its memory.
 * on success returns 0, otherwise a another number */
int deleteList(list_t *list)
{
    while(list->first != NULL)
    {
        // CR: (DC) What happens if removing the first node constantly fails?
        removeElement(list, list->first);

    }
    if(list->counter!=0)
    {
        printf("Error. Could not free all elements in the list.\n");
        return -1;
    }
    free(list);
    return 0;
}

/* This function prints a list to stdout. On success - returns 0. */
int printList(list_t *list)
{
    node_t *it = list->first;
    int i = 0;
    while(it != NULL)
    {
        ++i;
        printf("[%d] %s, %s \n", i, it->entryName, it->fileType);
        it = it->next;
    }
    printf("Total number of elements in the list: %d\n", list->counter);
    if(i != list->counter)
    {
        return -1;
    }

    return 0;
}