#include "inc/main.h"

// CR: (DC) Why int? You have bool type.
static volatile int keepRunning = 1;

// CR: (DC) Rename to indicate what this function does, such as stopRunning
void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char** argv) {
    // CR: (DC) Is it an airplane? Is it a starship?! No! It's captain obvious!
    //Variables declaration:
    char* pFolderPath;
    // CR: (DC) What if LIST_create failed?!
    LIST updatedFileList = LIST_create(releaseMemoryFile, getFileName);
    LIST dirList = LIST_create(releaseMemoryDir, getDirName);
    returnCode_t returnCodeMain = RETURNCODE_MAIN_UNINITIALIZED, dirListReturnCode, fileListReturnCode;

    //Attach SIGINT to the termination handler:
    signal(SIGINT, intHandler);

    //Parse arguments form usr and open the folder's path
    if (2 != argc) {
        DEBUG_PRINT("Incompatible number of parameters. Usage: Moneytor <folder_path>");
        returnCodeMain = RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER;
        goto exit;
    }
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);

    if (NULL == pDir) {
        DEBUG_PRINT("Could not open specified directory. Usage: Monytor <dir_path>.");
        returnCodeMain = RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
        goto exit;
    }
    dirInfo_t* pBaseDirInfo = createDirInfo_t(pFolderPath);
    if (NULL == pBaseDirInfo) {
        DEBUG_PRINT("Memory allocation falied. Free some memory and try again.");
        returnCodeMain = RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED;
        goto exit;
    }
    CHECK_RETURN_CODE_MAIN(LIST_addElement(dirList, (void*)pBaseDirInfo));

    //Initialize file list for the first time, including all subdir if exists:
    CHECK_RETURN_CODE_MAIN(getFileList(pDir, pBaseDirInfo->dirName, pBaseDirInfo->filesList, dirList));
    closedir(pDir);

    //Set iterator to the second element (if exists) and start scanning dirList:
    //todo: add iterator function to reduce code reuse!
    void* pDirInfoIt;
    LIST_getNext(dirList, pBaseDirInfo, &pDirInfoIt);
    while (NULL != pDirInfoIt) {
        dirInfo_t* pCurrentDir = (dirInfo_t*)pDirInfoIt;
        pDir = opendir(pCurrentDir->dirName);
        if (NULL != pDir) {
            //Whoohoo! Directory exists. Scan files and sub directories:
            getFileList(pDir, pCurrentDir->dirName, pCurrentDir->filesList, dirList);
            closedir(pDir);
        } else {
            DEBUG_PRINT("On initialization, could not print folder to read. FATAL ERROR!");
            returnCodeMain = RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
            goto exit;
        }
        LIST_getNext(dirList, pDirInfoIt, &pDirInfoIt);
    }

    //Start guarding the folder :)
    while (keepRunning) {
        DEBUG_PRINT("\n====================...Scanning...====================");

        //Iterate over all directories in the monitored folder:
        void* pDirInfoIterator = LIST_getFirst(dirList);
        while (pDirInfoIterator != NULL) {
            dirInfo_t* pCurrentDir = (dirInfo_t*)pDirInfoIterator;
            pDir = opendir(pCurrentDir->dirName);
            if (NULL != pDir) {
                //Whoohoo! Directory exists. Scan files and sub directories:
                CHECK_RETURN_CODE_MAIN(getFileList(pDir, pCurrentDir->dirName, updatedFileList, dirList));
                closedir(pDir);

                // CR: (DC) You are consistently ignoring your own functions return values. What if the fail?!
                // CR: (DC) You shouldn't continue!
                // CR: (DC) Check the return values!
                // CR: (DC) It will mean this function will get much bigger. Think of a way to split it into
                // CR: (DC) multiple functions, but really think what should be the best split to do.
                // CR: (DC) If you can't figure it out, lets discuss it.
                //Print changes:
                CHECK_RETURN_CODE_MAIN(findDiffElements(pCurrentDir->filesList, updatedFileList, "Deleted", false));
                CHECK_RETURN_CODE_MAIN(findDiffElements(updatedFileList, pCurrentDir->filesList, "Added", false));
                //fixme: fix bug in updated files :)
                CHECK_RETURN_CODE_MAIN(findDiffElements(pCurrentDir->filesList, updatedFileList, "Updated", true));

                //Swap lists:
                CHECK_RETURN_CODE_MAIN(LIST_destroy(pCurrentDir->filesList));
                CHECK_RETURN_CODE_MAIN(LIST_copy(&pCurrentDir->filesList, updatedFileList, fileInfoCopyFunction));
                CHECK_RETURN_CODE_MAIN(LIST_destroy(updatedFileList));
                updatedFileList = LIST_create(releaseMemoryFile, getFileName);

                //Go to next directory:
                LIST_getNext(dirList, pDirInfoIterator, &pDirInfoIterator);
            } else {
                //Directory has been deleted. Remove it from dirList and continue to next Directory
                void* tmpPtrToDeletedDir = pDirInfoIterator;
                LIST_getNext(dirList, pDirInfoIterator, &pDirInfoIterator);
                CHECK_RETURN_CODE_MAIN(LIST_removeElement(dirList, tmpPtrToDeletedDir));
            }
        }

        //Wait till next iteration:
        delay(SLEEP_TIME_SEC * 1000);
    }

    // Bye!
    printf("\n\nThanks for choosing Moneytor! Seeya again soon :)\n\n");

    //Exit properly: clean memory mainly -
    // CR: (DC) Use AutoFormat regularly (Ctrl+Alt+L)
    exit:
    // CR: (DC) Ignore return code in cleanup
    dirListReturnCode = LIST_destroy(dirList);
    if( RETURNCODE_SUCCESS != dirListReturnCode) {
        DEBUG_PRINT("Destroying directory list has Failed. Return code is: %d", dirListReturnCode);
    }

    fileListReturnCode = LIST_destroy(updatedFileList);
    if( RETURNCODE_SUCCESS != fileListReturnCode) {
        DEBUG_PRINT("Destroying files list has Failed. Return code is: %d", dirListReturnCode);
    }

    return returnCodeMain;
}
