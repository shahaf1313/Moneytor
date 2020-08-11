extern "C" {
    #include <stdio.h>
    #include <stdlib.h>
    #include "inc/file_info.h"
    #include "inc/dir_info.h"
}

#include "mock.hpp"
#include "hook.hpp"
#include <iostream>

using namespace testing;

TEST(Sample, dataHandlers_test) {
    //create fileInfo:
    fileInfo_t* fileInfo = createFileInfo_t("fileHakitsy", 0, FILE_INFO_ENTRY_TYPE_FILE);
    EXPECT_NE(fileInfo, nullptr);

    //create dirInfo:
    dirInfo_t* dirInfo = createDirInfo_t("dirHakitsy");
    EXPECT_NE(dirInfo, nullptr);

    //print file and dir name:
    char* fileName = getFileName(fileInfo);
    char* dirName = getDirName(dirInfo);

    EXPECT_NE(fileName, nullptr);
    EXPECT_NE(dirName, nullptr);

    std::cout << "\nFile name: " << fileName << "\n";
    std::cout << "Dir name: " << dirName << "\n";

    void* copiedFileInfo = fileInfoCopyFunction(fileInfo);
    fileInfo_t* copiedFileInfoPtr = (fileInfo_t*)copiedFileInfo;
    EXPECT_EQ(strcmp(copiedFileInfoPtr->fileName, fileInfo->fileName), 0);
    EXPECT_EQ(copiedFileInfoPtr->lastChanged, fileInfo->lastChanged);
    EXPECT_EQ(copiedFileInfoPtr->fileType, fileInfo->fileType);

    EXPECT_EQ(releaseMemoryFile(fileInfo), 0);
    EXPECT_EQ(releaseMemoryDir(dirInfo), 0);
    EXPECT_EQ(releaseMemoryFile(copiedFileInfoPtr), 0);

}
