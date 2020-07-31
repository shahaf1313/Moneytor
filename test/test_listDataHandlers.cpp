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
    fileInfo_t* fileInfo = createFileInfo_t("fileHakitsy", 0, "file");
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

    EXPECT_EQ(releaseMemoryFile((void*)fileInfo), 0);
    EXPECT_EQ(releaseMemoryDir((void*)dirInfo), 0);

}
