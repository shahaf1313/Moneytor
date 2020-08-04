#ifndef MONEYTOR_LIST_H
#define MONEYTOR_LIST_H

#include <stdio.h>
#include <stdlib.h>

// CR: (DC) Do AutoFormat (Ctrl+Alt+L)
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

// CR: (DC) In case of failure you return an appropriate listReturnCode_t value, specifically
// CR: (DC) RETURNCODE_LIST_GETLENGTH_LIST_IS_NULL. This return code is essentially the value 1,
// CR: (DC) which means in case of error, your function returns the value 1, which can be interpreted
// CR: (DC) as if the list's length is 1.
// CR: (DC) This mixup situation is caused because you're mixing two different types as the possible return
// CR: (DC) values: int and listReturnCode_t. Their underlying type is the same (it is int), but we treat
// CR: (DC) them as different types logically.
// CR: (DC) We don't want to mix return types. Hence, we have two options:
// CR: (DC) 1. Change the return type to listReturnCode_t. In case of success, a special return code
// CR: (DC)    will be returned, indicating success (often names RETURNCODE_SUCCESS, and has the value 0)
// CR: (DC)    With this method, we would need LIST_getLength to accept another int* paremeter, in which it
// CR: (DC)    will place the list's length (as we can't return multiple values from a function).
// CR: (DC) 2. Leave the return type as an int, which means we return the list's length. So what do we do
// CR: (DC)    in case of error? We can pick an int value that can't be returned for a valid list, like -1.
// CR: (DC)    There's no way LIST_getLength will return -1 for a valid list.
// CR: (DC)    So, we can return -1 on error. We can go the extra mile and #define that -1 to
// CR: (DC)    a constant named LIST_GET_LENGTH_ERROR for example.
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

// CR: (DC) This function returns a listReturnCode_t, so why did you specify the return type as int?
// CR: (DC) Declare the return type to be listReturnCode_t
// CR: (DC) It will require you to create a new return code, RETURNCODE_SUCCESS, that can hold the value 0
// CR: (DC) We don't need to create a success return value for each function, we can use the same one for all
// CR: (DC) And it is set to 0 as 0 is the convention for returning success (like the function main() returns
// CR: (DC) 0 on success)
// CR: (DC) Apply this CR to all functions that return listReturnCode_t
/**
 * @brief This function returns a pointer to the next data element of a given element in the list.
 * If the element is not on the list, or the element is the last in the list - an appropriate listReturnCode_t is returned.
 * @param list [IN] is a LIST instance to get it's next element.
 * @param pDataCurrent [IN] is a pointer to a data element from which we want to get the next data element.
 * @param pDataNext [OUT] is an address of a pointer to the next data element.
 * @returns On success - 0, else - appropriate listReturnCode_t.
**/
int LIST_getNext(LIST list, void* pDataCurrent, void** pDataNext);

// CR: (DC) Don't tell me this function removes a node, I don't want to even think about nodes as your
// CR: (DC) module's user. As far as I'm concerned, you may implement the list using a dynamic array
// CR: (DC) (often called ArrayList), and not using nodes, and as your user, I don't care one bit how
// CR: (DC) you implement it.
// CR: (DC) Remove the "node" terminology from all documentation that the user can see
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

// CR: (DC) It is bad practice to deallocate memory not created by you. The responsibility for deallocating
// CR: (DC) the dest list lies on the programmer that called LIST_create, and you shouldn't be doing his work.
// CR: (DC) Moreover, it can cause bugs. Consider the following snippet:
// CR: (DC)
// CR: (DC) LIST list1 = LIST_create(...);
// CR: (DC) LIST list2 = list1;
// CR: (DC) LIST_copyList(list2, list1, ...);
// CR: (DC)
// CR: (DC) I allocate a list, and then create another pointer to that list, named list2. I do that for my
// CR: (DC) own reasons, maybe I need two pointers for some reason. Then, I decide I want to copy list1
// CR: (DC) and I want to reuse list2 to point to the new list, so I pass it as a dest.
// CR: (DC) What will happen here is that you'll destroy list2, which means you'll also destroy list1!
// CR: (DC) And then you'll try to copy from the destroyed list1 into list2!! What a mess.
// CR: (DC) To avoid that, as your user, I would need to set list2 to NULL before calling LIST_copy.
// CR: (DC) But, as the developers of the list module, we can say to our users: "we don't care what your
// CR: (DC) dest pointer points to. If you've forgotten to deallocate, it's your problem"
// CR: (DC) This way we have one less thing to worry about, we won't be accused of writing bugs, and it's
// CR: (DC) perfectly fine, as the responsibility for deallocating list1 is not ours! It's our user's.
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
// CR: (DC) Can be renamed to LIST_copy. The last "List" in the name is very redundant.
// CR: (DC) Of course it copies a list, we're in the list module
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
