extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include "inc/utills.h"
#include "inc/file_system_scanner.h"
}

#include "mock.hpp"
#include "hook.hpp"
#include <string>
#include <iostream>

using namespace testing;

int releaseMemoryFileOuter(void* ptr) {
    return releaseMemoryFile((fileInfo_t*)ptr);
}

int releaseMemoryDirOuter(void* ptr) {
    return releaseMemoryDir((dirInfo_t*)ptr);
}

char* getFileNameOuter(void* ptr) {
    return getFileName((fileInfo_t*)ptr);
}

char* getDirNameOuter(void* ptr) {
    return getDirName((dirInfo_t*)ptr);
}

TEST(Sample, utills_test) {
    //dor's test:
    LIST dirListDor = LIST_create(releaseMemoryDirOuter, getDirNameOuter);
    char folderDor[MAX_PATH_LENGTH] = "/mnt/c/Users/Shahaf/CLionProjects/Moneytor/test/testUtillsFolderDor";
    dirInfo_t* pDorDirInfo_t = createDirInfo_t(folderDor);
    // Mocking LIST_addElement to fail :)
    // LIST_addElement(dirListDor, (void*)pDorDirInfo_t);
    DIR* pDirDor = opendir(folderDor);
    returnCode_t returnCodeDor = getFileList(pDirDor, folderDor, pDorDirInfo_t->filesList, dirListDor);
    EXPECT_EQ(returnCodeDor, RETURNCODE_SUCCESS);
    EXPECT_EQ(printDirTree(dirListDor), RETURNCODE_SUCCESS);
    closedir(pDirDor);
    EXPECT_EQ(releaseMemoryDirOuter((void*)pDorDirInfo_t), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_destroy(dirListDor), RETURNCODE_SUCCESS);

    // CR: (SE) After freeing the memory allocated, it all functions as expected :) even though the dirInfo_t was not added to the list :)

    //check delay:
    clock_t tol = CLOCKS_PER_SEC / 1000;
    clock_t before = clock();
    delay(1000);
    clock_t after = clock();
    EXPECT_LE(after - before, tol);
    EXPECT_GE(after - before, -tol);

    //create 3 dirs, each contains 3 files:
    LIST dirList = LIST_create(releaseMemoryDirOuter, getDirNameOuter);
    LIST fileList = LIST_create(releaseMemoryFileOuter, getFileNameOuter);
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
            fileInfo* currFile = createFileInfo_t(tmpName, j, FILE_INFO_ENTRY_TYPE_FILE);
            LIST_addElement(currDir->filesList, (void*)currFile);
        }
    }
    //check printDirTree:
    printDirTree(dirList);
    EXPECT_EQ(LIST_destroy(dirList), RETURNCODE_SUCCESS);
    EXPECT_EQ(LIST_destroy(fileList), RETURNCODE_SUCCESS);

    //check getFileList:
    dirList = LIST_create(releaseMemoryDirOuter, getDirNameOuter);
    char folder[MAX_PATH_LENGTH] = "/mnt/c/Users/Shahaf/CLionProjects/Moneytor/test/testUtillsFolderShahaf";
    dirInfo_t* pDirInfo_t = createDirInfo_t(folder);
    LIST_addElement(dirList, (void*)pDirInfo_t);
    DIR* pDir = opendir(folder);
    returnCode_t returnCode = getFileList(pDir, folder, pDirInfo_t->filesList, dirList);
    EXPECT_EQ(printDirTree(dirList), RETURNCODE_SUCCESS);
    EXPECT_EQ(returnCode, RETURNCODE_SUCCESS);

    //check findDiffElements:
    fileList = LIST_create(releaseMemoryFileOuter, getFileNameOuter);
    //get another copy to fileList:
    returnCode = getFileList(pDir, folder, fileList, dirList);
    EXPECT_EQ(returnCode, RETURNCODE_SUCCESS);

    //remove one element:
    LIST_removeElement(fileList, LIST_getData(LIST_getLast(fileList)));
    //add another:
    fileInfo_t* newHakitsyFile = createFileInfo_t("Hakitsy", 0, FILE_INFO_ENTRY_TYPE_FILE);
    LIST_addElement(fileList, (void*)newHakitsyFile);
    LIST_print(fileList);
    LIST_print(pDirInfo_t->filesList);
    //compare with findDiffs:
    DEBUG_PRINT("\nremoved dir2, added Hakitsy. lets check:");
    findDiffElements(pDirInfo_t->filesList, fileList, "Deleted", false);
    findDiffElements(fileList, pDirInfo_t->filesList, "Added", false);
    //destroy lists and dir:
    closedir(pDir);
    EXPECT_EQ(LIST_destroy(dirList), 0);
    EXPECT_EQ(LIST_destroy(fileList), 0);

}
