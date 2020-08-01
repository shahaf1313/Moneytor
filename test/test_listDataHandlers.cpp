extern "C" {
    #include <stdio.h>
    #include <stdlib.h>
    #include "inc/listDataHandlers.h"
}

#include "mock.hpp"
#include "hook.hpp"
#include <iostream>

using namespace testing;

TEST(Sample, dataHandlers_test) {
    //create fileInfo:
    fileInfo_t* fileInfo = createFileInfo_t("fileHakitsy", 0, LIST_DATA_HANDLERS_ENTRY_TYPE_FILE);
    EXPECT_NE(fileInfo, nullptr);

    //create dirInfo:
    dirInfo_t* dirInfo = createDirInfo_t("dirHakitsy");
    EXPECT_NE(dirInfo, nullptr);

    //print file and dir name:
    char* fileName = getFileName((void*)fileInfo);
    char* dirName = getDirName((void*)dirInfo);

    EXPECT_NE(fileName, nullptr);
    EXPECT_NE(dirName, nullptr);

    std::cout << "\nFile name: " << fileName << "\n";
    std::cout << "Dir name: " << dirName << "\n";

    void* copiedFileInfo = fileInfoCopyFunction((void*)fileInfo);
    fileInfo_t* copiedFileInfoPtr = (fileInfo_t*)copiedFileInfo;
    EXPECT_EQ(strcmp(copiedFileInfoPtr->fileName, fileInfo->fileName), 0);
    EXPECT_EQ(copiedFileInfoPtr->lastChanged, fileInfo->lastChanged);
    EXPECT_EQ(copiedFileInfoPtr->fileType, fileInfo->fileType);

    EXPECT_EQ(releaseMemoryFile((void*)fileInfo), 0);
    EXPECT_EQ(releaseMemoryDir((void*)dirInfo), 0);
    EXPECT_EQ(releaseMemoryFile((void*)copiedFileInfoPtr), 0);

}
