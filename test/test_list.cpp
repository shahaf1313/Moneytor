extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include "inc/list.h"
}

#include "mock.hpp"
#include "hook.hpp"
#include <iostream>

#define NODES_NUM 9

typedef struct data {
    char name[100];
    int num;
} data_t;

int relMem(void* pDataVoid) {
    free(pDataVoid);
    pDataVoid = NULL;
    return 0;
}

void* copyFunction(void* ptr) {
    if (ptr == NULL) {
        return NULL;
    }
    data_t* pData = (data_t*)ptr;
    data_t* pNewData = (data_t*)malloc(sizeof(*pNewData));
    if (pNewData == NULL) {
        return NULL;
    }
    strcpy(pNewData->name, pData->name);
    pNewData->num = pData->num;
    return (void*)pNewData;
}

char* getName(void* pDataVoid) {
    return ((data_t*) pDataVoid)->name;
}

using namespace testing;

// CR: (DC) Create a new test that does the following:
// CR: (DC) 1. Create a new list, and for the release function, pass a function that returns 1
// CR: (DC) 2. Add a new element to the list with the value ((void*) 10) (it's a list of ints)
// CR: (DC) 3. Remove the element with value ((void*) 10)
// CR: (DC) Run this test, make sure it works. Then run it with valgrind. Were there any leaks?

// CR: (DC) Create a new test that calls LIST_copyList with dest=NULL

// CR: (DC) Create a new test that calls LIST_copyList with a copy function that returns NULL
// CR: (DC) Run the test with valgrind. Were there any leaks?

// CR: (DC) Create a new test that does the following:
// CR: (DC) 1. Create a new list, and for the getName function, pass a function that returns NULL
// CR: (DC) 2. Add the element ((void*) 10) to the list
// CR: (DC) 3. Print the list (using LIST_print)
// CR: (DC) Run this test, make sure it works. Then run it with valgrind. Were there any leaks?

TEST(Sample, LIST_test) {
    int i;
    char ch, snum[2];
    snum[1] = '\0';
    void* dummyPtr;

    //create list and check:
    LIST list = LIST_create(relMem, getName);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(LIST_getLength(list), 0);

    //try to remove from empty list:
    EXPECT_EQ(LIST_removeElement(list, nullptr), RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY);

    //get first and last from empty list:
    EXPECT_EQ(LIST_getFirst(list), nullptr);
    EXPECT_EQ(LIST_getLast(list), nullptr);

    //get next element from empty list:
    EXPECT_EQ(LIST_getNext(list, snum, &dummyPtr), RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND);

    //try to add null element:
    EXPECT_EQ(LIST_addElement(list, nullptr), RETURNCODE_LIST_ADDELEMENT_LIST_OR_PADTA_NULL);

    //create data to store:
    data_t* pDataArr[NODES_NUM];
    for (i = 0; i < NODES_NUM; i++) {
        EXPECT_NE(pDataArr[i] = (data_t*) malloc(sizeof(data_t)), nullptr);
        ch = '0' + i;
        snum[0] = ch;
        strcpy(pDataArr[i]->name, "HelloDataStruct");
        strcat(pDataArr[i]->name, snum);
        pDataArr[i]->num = i;
    }

    //add all elements BUT the last:
    for (i = 0; i < NODES_NUM - 1; i++) {
        EXPECT_EQ(LIST_addElement(list, pDataArr[i]), 0);
    }

    //try to print list:
    EXPECT_EQ(LIST_print(list), 0);

    //copy list and check:
    LIST copiedList = NULL;
    EXPECT_EQ(LIST_copyList(&copiedList, list, copyFunction), 0);
    std::cout << "\nCOPYLIST check - copied list:\n";
    LIST_print(copiedList);

    //try to iterate over the list:
    dummyPtr = LIST_getFirst(list);
    EXPECT_NE(dummyPtr, nullptr);
    for (int j = 0; j < LIST_getLength(list); ++j) {
        int returnCode = LIST_getNext(list, dummyPtr, &dummyPtr);
        if (j < LIST_getLength(list)-1) {
            EXPECT_EQ(returnCode, 0);
        } else {
            EXPECT_EQ(returnCode, RETURNCODE_LIST_GETNEXT_LAST_ELEMENT);
        }
    }

    //remove non existing element;
    EXPECT_EQ(LIST_removeElement(list, (void*) pDataArr[NODES_NUM - 1]),
              RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND);
    EXPECT_EQ(LIST_getLength(list), NODES_NUM - 1);

    //try to add existing element:
    EXPECT_EQ(LIST_addElement(list, pDataArr[0]), 0);

    //remove all elements BUT one:
    for (i = 1; i < NODES_NUM - 1; i++) {
        EXPECT_EQ(LIST_removeElement(list, (void*) pDataArr[i]), 0);
    }

    //print list again:
    EXPECT_EQ(LIST_print(list), 0);

    //get current length:
    EXPECT_EQ(LIST_getLength(list), 2);

    //destroy list:
    EXPECT_EQ(LIST_destroy(list), 0);
    EXPECT_EQ(LIST_destroy(copiedList), 0);

    relMem((void*) pDataArr[NODES_NUM - 1]);

}
