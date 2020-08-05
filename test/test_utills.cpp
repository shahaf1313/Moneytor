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
            fileInfo* currFile = createFileInfo_t(tmpName, j, LIST_DATA_HANDLERS_ENTRY_TYPE_FILE);
            LIST_addElement(currDir->filesList, (void*)currFile);
        }
    }
    //check printDirTree:
    printDirTree(dirList);
    EXPECT_EQ(LIST_destroy(dirList), 0);
    EXPECT_EQ(LIST_destroy(fileList), 0);

    //check getFileList:
    dirList = LIST_create(releaseMemoryDir, getDirName);
    char folder[MAX_PATH_LENGTH] = "/mnt/c/Users/Shahaf/CLionProjects/Moneytor/test/testUtillsFolder";
    dirInfo_t* pDirInfo_t = createDirInfo_t(folder);
    LIST_addElement(dirList, (void*)pDirInfo_t);
    DIR* pDir = opendir(folder);
    int filesNum = getFileList(pDir, folder, pDirInfo_t->filesList, dirList);
    printDirTree(dirList);
    EXPECT_EQ(filesNum, 8);

    //check findDiffNodes:
    fileList = LIST_create(releaseMemoryFile, getFileName);
    //get another copy to fileList:
    getFileList(pDir, folder, fileList, dirList);
    //remove one element:
    LIST_removeElement(fileList, LIST_getLast(fileList));
    //add another:
    fileInfo_t* newHakitsyFile = createFileInfo_t("Hakitsy", 0, LIST_DATA_HANDLERS_ENTRY_TYPE_FILE);
    LIST_addElement(fileList, (void*)newHakitsyFile);
    LIST_print(fileList);
    LIST_print(pDirInfo_t->filesList);
    //compare with findDiffs:
    DEBUG_PRINT("\nremoved dir2, added Hakitsy. lets check:");
    findDiffNodes(pDirInfo_t->filesList, fileList, "Deleted", false);
    findDiffNodes(fileList, pDirInfo_t->filesList, "Added", false);
    //destroy lists and dir:
    closedir(pDir);
    EXPECT_EQ(LIST_destroy(dirList), 0);
    EXPECT_EQ(LIST_destroy(fileList), 0);
}
