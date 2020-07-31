extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include "inc/utills.h"
#include "inc/listDataHandlers.h"
}

#include "mock.hpp"
#include "hook.hpp"
#include <string>
#include <iostream>

using namespace testing;

TEST(Sample, utills_test) {
    //check delay:
    clock_t tol = CLOCKS_PER_SEC / 1000;
    clock_t before = clock();
    delay(1000);
    clock_t after = clock();
    EXPECT_LE(after-before, CLOCKS_PER_SEC + tol);
    EXPECT_GE(after-before, CLOCKS_PER_SEC - tol);

    //create 3 dirs, each contains 3 files:
    LIST dirList = LIST_create(releaseMemoryDir, getDirName);
    LIST fileList = LIST_create(releaseMemoryFile, getFileName);
    EXPECT_NE(dirList, nullptr);
    EXPECT_NE(fileList, nullptr);
    char tmpName[20];
    for (int i = 0; i < 3; ++i) {
        std::string dirName = "dir" + std::to_string(i);
        strcpy(tmpName, dirName.c_str());
        dirInfo_t* currDir = createDirInfo_t(tmpName);
        LIST_addElement(dirList, (void*)currDir);
        for (int j = 0; j < 3; ++j) {
            std::string fileName = "file" + std::to_string(j) + "dir" + std::to_string(i);
            strcpy(tmpName, fileName.c_str());
            fileInfo* currFile = createFileInfo_t(tmpName, j, "file");
            LIST_addElement(currDir->filesList, (void*)currFile);
        }
    }
    //print dir tree:
    printDirTree(dirList);
    EXPECT_EQ(LIST_destroy(dirList), 0);
    EXPECT_EQ(LIST_destroy(fileList), 0);
}
