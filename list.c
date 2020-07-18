#include "list.h"

/* This function initializes the list.
 * on success returns 0, otherwise a another number */
list_t* initList(void)
{
    list_t *list = (list_t*)malloc(sizeof(list_t));
    if( list == NULL)
    {
        printf("Memory allocation failed. Plase try again after memory has been freed.\n");
    }
    return list;
}

/* This function removes a specified entry from the list.
 * on success returns 0, otherwise a another number */
int removeElement(list_t *list, node_t* pElement)
{
    if(list->counter == 0)
    {
        printf("Tried to remove entry from a list with 0 elements.\n");
        return -1;
    }
    else if( list->counter == 1)
    {
        free(pElement);
        list->last = NULL;
        list->first = NULL;
    }
    else if(pElement == list->first)
    {
        node_t *newFirst = pElement->next;
        list->first = newFirst;
        newFirst->prev = NULL;
        free(pElement);
    }
    else if(pElement == list->last)
    {
        node_t *newLast = pElement->prev;
        list->last = newLast;
        newLast->next = NULL;
        free(pElement);
    }
    else
    {
        node_t *pPrev = pElement->prev;
        node_t *pNext = pElement->next;
        pPrev->next = pNext;
        pNext->prev = pPrev;
        free(pElement);
    }
    --(list->counter);
    return 0;
}

/* This function adds an entry to the end of the list.
 * on success returns 0, otherwise a another number */
int addElement(list_t *list, char* name, time_t lastChanged)
{
    node_t *newNode = (node_t*)malloc(sizeof(node_t));
    if(newNode == NULL) { return -1;}
    newNode->lastChanged = lastChanged;
    strcpy(newNode->entryName, name);
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
        addElement(pNewList, srcIt->entryName, srcIt->lastChanged);
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

/* This function prints a list to stdout */
void printList(list_t *list)
{
    node_t *it = list->first;
    int i = 0;
    while(it != NULL)
    {
        ++i;
        char* prev = (it->prev == NULL) ? "NULL" : it->prev->entryName;
        char* next = (it->next == NULL) ? "NULL" : it->next->entryName;
        printf("[%d] %s, prev = %s, next = %s\n", i, it->entryName, prev, next);
        it = it->next;
    }
    printf("Total number of elements in the list: %d\n", list->counter);
}