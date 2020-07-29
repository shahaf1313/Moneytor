extern "C" {
    #include <stdio.h>
    #include <stdarg.h>
    #include <stdlib.h>
    #include <string.h>
    // CR: (DC) Use #include "list.h" (see CRs in CMakeLists.txt)
    #include "../list.h"
    // CR: (DC) Everything below here can be moved outside of the extern "C"
    // CR: (DC) as it is valid C++ code
    #define NODES_NUM 9
    typedef struct data {
        char name[100];
        int num;
    } data_t;

    int relMem(void* pDataVoid) {
        data_t* pData = (data_t*)pDataVoid;
        free(pData);
        return 0;
    }

    char* getName(void* pDataVoid) {
        return ((data_t*)pDataVoid)->name;
    }
}

#include "mock.hpp"
#include "hook.hpp"
#include <iostream>
// CR: (DC) We don't use an entire namespace, as it pollutes our namespace
// CR: (DC) Add specific using directives based on the specific things you need
using namespace std;
using namespace testing;


// CR: (DC) Rename test
TEST(Sample, Test)
{
    int i;
    char ch, snum[2];
    snum[1] = '\0';

    //create list and check:
    // CR: (DC) Can't you just pass free?
    LIST list = LIST_create(relMem, getName);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(LIST_getLength(list), 0);

    //try to remove from empty list:
    EXPECT_EQ(LIST_removeElement(list, nullptr), -2);

    //try to add null element:
    EXPECT_EQ(LIST_addElement(list, nullptr), -1);


    //create data to store:
    data_t* pDataArr[NODES_NUM];
    for(i=0; i<NODES_NUM; i++)
    {
        EXPECT_NE(pDataArr[i] = (data_t*)malloc(sizeof(data_t)), nullptr);
        ch = '0' + i;
        snum[0] = ch;
        strcpy(pDataArr[i]->name, "HelloDataStruct");
        strcat(pDataArr[i]->name, snum);
        pDataArr[i]->num = i;
        // CR: (DC) Delete commented out code
        //cout << "node name is " << pDataArr[i]->name << " and num " << pDataArr[i]->num ;
        //cout << ", pointer to data - " << pDataArr[i] << "\n";
    }

    //add all elements BUT the last:
    for(i=0; i<NODES_NUM-1; i++)
    {
        EXPECT_EQ(LIST_addElement(list, pDataArr[i]), 0);
    }

    //try to print list:
    EXPECT_EQ(LIST_print(list), 0);

    //remove non existing element;
    EXPECT_EQ(LIST_removeElement(list, (void*)pDataArr[NODES_NUM-1]), -3);
    EXPECT_EQ(LIST_getLength(list), NODES_NUM-1);

    // CR: (DC) Can be moved to a different test
    //try to add element to null list:
    EXPECT_EQ(LIST_addElement(nullptr, pDataArr[0]), -1);

    //try to add existing element:
    EXPECT_EQ(LIST_addElement(list, pDataArr[0]), -2);

    //remove all elements BUT one:
    for(i=1; i<NODES_NUM-1; i++) {
        EXPECT_EQ(LIST_removeElement(list, (void*)pDataArr[i]), 0);
    }

    //print list again:
    EXPECT_EQ(LIST_print(list), 0);

    //get current length:
    EXPECT_EQ(LIST_getLength(list), 1);

    //destroy list:
    EXPECT_EQ(LIST_destroy(list), 0);

    // CR: (DC) Why not just use free?
    relMem((void*)pDataArr[NODES_NUM-1]);


}
