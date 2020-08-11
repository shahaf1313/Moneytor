#include "inc/main.h"

static volatile bool keepRunning = true;

// This function catches ctrl + c and causes to program termination
void stopRunning(int dummy) {
    keepRunning = false;
}

/**
 * @brief This function initializes directory tree on program start.
 * @param dirList [IN,OUT] Directory list that contains only the input folder.
 * @return Appropriate returncode_t.
 */
returnCode_t initializeDirectoryList(LIST* dirList) {
    if (NULL == dirList) {
        return RETURNCODE_MAIN_POINTER_TO_DIRLIST_IS_NULL;
    }
    DIR* pDir = NULL;
    node_t* pNodeDirIterator = LIST_getFirst(*dirList);
    for (LIST_getNext(*dirList, pNodeDirIterator, &pNodeDirIterator); pNodeDirIterator != NULL; LIST_getNext(*dirList, pNodeDirIterator, &pNodeDirIterator)) {
        dirInfo_t* pCurrentDir = (dirInfo_t*) LIST_getData(pNodeDirIterator);
        if (NULL == pCurrentDir) {
            DEBUG_PRINT("Found NULL directory. Fatal error.");
            return RETURNCODE_MAIN_FOUND_NULL_DIRECTORY;
        }
        pDir = opendir(pCurrentDir->dirName);
        if (NULL != pDir) {
            if (RETURNCODE_SUCCESS != getFileList(pDir, pCurrentDir->dirName, pCurrentDir->filesList, *dirList)) {
                closedir(pDir);
                DEBUG_PRINT("Failed to read file list from dir %s.", pCurrentDir->dirName);
                return RETURNCODE_MAIN_COULD_NOT_READ_FILE_LIST;
            }
            closedir(pDir);
        } else {
            DEBUG_PRINT("On initialization, could not print folder to read. FATAL ERROR!");
            return RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
        }
    }
    return RETURNCODE_SUCCESS;
}

int main(int argc, char** argv) {
    char* pFolderPath = NULL;
    LIST updatedFileList = LIST_create(releaseMemoryFile, getFileName);
    LIST dirList = LIST_create(releaseMemoryDir, getDirName);
    if ((NULL == updatedFileList) || (NULL == dirList)) {
        DEBUG_PRINT("Unable to init directory list or file list. Please try again.");
        goto exit;
    }
    returnCode_t returnCodeMain = RETURNCODE_MAIN_UNINITIALIZED;

    //Attach SIGINT to the termination handler:
    signal(SIGINT, stopRunning);

    if (2 != argc) {
        DEBUG_PRINT("Incompatible number of parameters. Usage: Moneytor <folder_path>");
        returnCodeMain = RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER;
        goto exit;
    }
    // Add target directory to dirList:
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);
    if (NULL == pDir) {
        DEBUG_PRINT("Could not open specified directory. Usage: Monytor <dir_path>.");
        returnCodeMain = RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
        goto exit;
    }
    dirInfo_t* pBaseDirInfo = createDirInfo_t(pFolderPath);
    if (NULL == pBaseDirInfo) {
        DEBUG_PRINT("Memory allocation failed. Free some memory and try again.");
        returnCodeMain = RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED;
        goto exit;
    }
    CHECK_RETURN_CODE_MAIN(LIST_addElement(dirList, (void*) pBaseDirInfo));
    CHECK_RETURN_CODE_MAIN(getFileList(pDir, pBaseDirInfo->dirName, pBaseDirInfo->filesList, dirList));
    closedir(pDir);
    CHECK_RETURN_CODE_MAIN(initializeDirectoryList(&dirList));

    //Start guarding the folder :)
    while (keepRunning) {
        DEBUG_PRINT("\n====================...Scanning...====================");

        //Iterate over all directories in the monitored folder:
        for (node_t* pNodeDirIterator = LIST_getFirst(dirList); pNodeDirIterator != NULL; LIST_getNext(dirList, pNodeDirIterator, &pNodeDirIterator)) {
            dirInfo_t* pCurrentDir = (dirInfo_t*) LIST_getData(pNodeDirIterator);
            pDir = opendir(pCurrentDir->dirName);
            if (NULL != pDir) {
                CHECK_RETURN_CODE_MAIN(getFileList(pDir, pCurrentDir->dirName, updatedFileList, dirList));
                closedir(pDir);

                //Print changes:
                CHECK_RETURN_CODE_MAIN(findDiffElements(pCurrentDir->filesList, updatedFileList, "Deleted", false));
                CHECK_RETURN_CODE_MAIN(findDiffElements(updatedFileList, pCurrentDir->filesList, "Added", false));
                CHECK_RETURN_CODE_MAIN(findDiffElements(pCurrentDir->filesList, updatedFileList, "Updated", true));

                //Swap lists:
                CHECK_RETURN_CODE_MAIN(LIST_destroy(pCurrentDir->filesList));
                CHECK_RETURN_CODE_MAIN(LIST_copy(&pCurrentDir->filesList, updatedFileList, fileInfoCopyFunction));
                CHECK_RETURN_CODE_MAIN(LIST_destroy(updatedFileList));
                updatedFileList = LIST_create(releaseMemoryFile, getFileName);
                CHECK_NULL_POINTER_MAIN(updatedFileList);
            } else {
                //Directory has been deleted. Remove it from dirList and continue to next Directory
                CHECK_RETURN_CODE_MAIN(LIST_removeElement(dirList, pCurrentDir));
            }
        }

        delay(SLEEP_TIME_SEC*1000);
    }
    printf("\n\nThanks for choosing Moneytor! Seeya again soon :)\n\n");

    //Exit properly: clean memory mainly -
exit:
    LIST_destroy(dirList);
    LIST_destroy(updatedFileList);
    return returnCodeMain;
}
