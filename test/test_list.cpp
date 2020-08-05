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
    if (NULL == pDataVoid) {
        return 0;
    }
    free(pDataVoid);
    pDataVoid = NULL;
    return 0;
}

void* copyFunction(void* ptr) {
    if (ptr == NULL) {
        return NULL;
    }
    data_t* pData = (data_t*) ptr;
    data_t* pNewData = (data_t*) malloc(sizeof(*pNewData));
    if (pNewData == NULL) {
        return NULL;
    }
    strcpy(pNewData->name, pData->name);
    pNewData->num = pData->num;
    return (void*) pNewData;
}

char* getName(void* pDataVoid) {
    return ((data_t*) pDataVoid)->name;
}

int dummyReleaseFunction(void* param) {
    return 0;
}

char* dummyNameFunction(void* element) {
    return "Dummy";
}

void* nullFunction(void* ptr) {
    return nullptr;
}

char* nullNameFunction(void* ptr) {
    return nullptr;
}


using namespace testing;


TEST(Sample, test_list_dor) {
    // Create a new test that does the following:
    // 1. Create a new list, and for the release function, pass a function that returns 0 always
    // 2. Add a new element to the list with the value ((void*) 10) (it's a list of ints)
    // 3. Remove the element with value ((void*) 10)
    // Run this test, make sure it works. Then run it with valgrind. Were there any leaks?
    LIST list = LIST_create(dummyReleaseFunction, dummyNameFunction);
    EXPECT_NE(nullptr, list);
    EXPECT_EQ(LIST_addElement(list, (void*) 10), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_removeElement(list, (void*) 10), RETURNCODE_SUCCESS);

    // Create a new test that calls LIST_copy with dest=NULL
    EXPECT_EQ(LIST_copy(nullptr, list, copyFunction), RETURNCODE_LIST_COPY_PARAMETER_NULL);

    // Create a new test that calls LIST_copy with a copy function that returns NULL
    // Run the test with valgrind. Were there any leaks?
    LIST copiedList;
    EXPECT_EQ(LIST_addElement(list, (void*) nullptr), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_copy(&copiedList, list, nullFunction), RETURNCODE_SUCCESS);

    // Create a new test that does the following:
    // 1. Create a new list, and for the getName function, pass a function that returns NULL
    // 2. Add the element ((void*) 10) to the list
    // 3. Print the list (using LIST_print)
    // Run this test, make sure it works. Then run it with valgrind. Were there any leaks?
    EXPECT_EQ(LIST_destroy(list), RETURNCODE_SUCCESS);
    list = LIST_create(dummyReleaseFunction, nullNameFunction);
    EXPECT_EQ(LIST_addElement(list, (void*) 10), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_print(list), RETURNCODE_SUCCESS);
    //Destroy lists:
    EXPECT_EQ(LIST_destroy(list), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_destroy(copiedList), RETURNCODE_SUCCESS);


}

// CR: (DC) Create a test that calls LIST_getLength(list, NULL)

// CR: (DC) Create a test that does the following:
// CR: (DC) 1. Create a list
// CR: (DC) 2. Insert the value ((void*) 10)
// CR: (DC) 3. Call LIST_getNext(my_list, ((void*) 10), NULL)

TEST(Sample, test_list_shahaf) {
    int i, listLength;
    char ch, snum[2];
    snum[1] = '\0';
    void* dummyPtr;

    //create list and check:
    LIST list = LIST_create(relMem, getName);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(LIST_getLength(list, &listLength), RETURNCODE_SUCCESS);
    EXPECT_EQ(listLength, 0);

    //try to remove from empty list:
    EXPECT_EQ(LIST_removeElement(list, nullptr), RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY);

    //get first and last from empty list:
    EXPECT_EQ(LIST_getFirst(list), nullptr);
    EXPECT_EQ(LIST_getLast(list), nullptr);

    //get next element from empty list:
    EXPECT_EQ(LIST_getNext(list, snum, &dummyPtr), RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND);

    //try to add null element:
    EXPECT_EQ(LIST_addElement(list, nullptr), RETURNCODE_SUCCESS);
    //remove null element:
    EXPECT_EQ(LIST_removeElement(list, nullptr), RETURNCODE_SUCCESS);

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
        EXPECT_EQ(LIST_addElement(list, pDataArr[i]), RETURNCODE_SUCCESS);
    }

    //try to print list:
    EXPECT_EQ(LIST_print(list), RETURNCODE_SUCCESS);

    //copy list and check:
    LIST copiedList = NULL;
    EXPECT_EQ(LIST_copy(&copiedList, list, copyFunction), RETURNCODE_SUCCESS);
    std::cout << "\nCOPYLIST check - copied list:\n";
    LIST_print(copiedList);

    //try to iterate over the list:
    dummyPtr = LIST_getFirst(list);
    LIST_getLength(list, &listLength);
    EXPECT_NE(dummyPtr, nullptr);
    for (int j = 0; j < listLength; ++j) {
        int returnCode = LIST_getNext(list, dummyPtr, &dummyPtr);
        if (j < listLength - 1) {
            EXPECT_EQ(returnCode, RETURNCODE_SUCCESS);
        } else {
            EXPECT_EQ(returnCode, RETURNCODE_LIST_GETNEXT_LAST_ELEMENT);
        }
    }

    //remove non existing element;
    EXPECT_EQ(LIST_removeElement(list, (void*) pDataArr[NODES_NUM - 1]), RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND);
    EXPECT_EQ(LIST_getLength(list, &listLength), RETURNCODE_SUCCESS);
    EXPECT_EQ(listLength, NODES_NUM - 1);

    //remove all elements BUT one:
    for (i = 1; i < NODES_NUM - 1; i++) {
        EXPECT_EQ(LIST_removeElement(list, (void*) pDataArr[i]), RETURNCODE_SUCCESS);
    }

    //print list again:
    EXPECT_EQ(LIST_print(list), RETURNCODE_SUCCESS);

    //get current length:
    EXPECT_EQ(LIST_getLength(list, &listLength), RETURNCODE_SUCCESS);
    EXPECT_EQ(listLength, 1);

    //destroy list:
    EXPECT_EQ(LIST_destroy(list), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_destroy(copiedList), RETURNCODE_SUCCESS);

    relMem((void*) pDataArr[NODES_NUM - 1]);

}
