#include "inc/main.h"

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char** argv) {
    //Variables declaration:
    char* pFolderPath;
    LIST updatedFileList = LIST_create(releaseMemoryFile, getFileName);
    LIST dirList = LIST_create(releaseMemoryDir, getDirName);
    int returnCodeMain = 0;

    //Attach SIGINT to the termination handler:
    signal(SIGINT, intHandler);

    //Parse arguments form usr and open the folder's path
    if (argc != 2) {
        DEBUG_PRINT("Incompatible number of parameters. Usage: Moneytor <folder_path>");
        returnCodeMain = RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER;
        goto exit;
    }
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);

    //Open folder and add folder to DirsList:
    if (pDir == NULL) {
        DEBUG_PRINT("Could not open specified directory. Usage: Monytor <dir_path>.");
        returnCodeMain = RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
        goto exit;
    }
    dirInfo_t* pBaseDirInfo = createDirInfo_t(pFolderPath);
    if (pBaseDirInfo == NULL) {
        DEBUG_PRINT("Memory allocation falied. Free some memory and try again.");
        returnCodeMain = RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED;
        goto exit;
    }
    LIST_addElement(dirList, (void*)pBaseDirInfo);

    //Initialize file list for the first time, including all subdir if exists:
    getFileList(pDir, pBaseDirInfo->dirName, pBaseDirInfo->filesList, dirList);
    closedir(pDir);

    //Set iterator to the second element (if exists) and start scanning dirList:
    void* pDirInfoIt;
    LIST_getNext(dirList, pBaseDirInfo, &pDirInfoIt);
    while (pDirInfoIt != NULL) {
        dirInfo_t* pCurrentDir = (dirInfo_t*)pDirInfoIt;
        pDir = opendir(pCurrentDir->dirName);
        if (pDir != NULL) {
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
            if (pDir != NULL) {
                //Whoohoo! Directory exists. Scan files and sub directories:
                getFileList(pDir, pCurrentDir->dirName, updatedFileList, dirList);
                closedir(pDir);

                //Print changes:
                findDiffNodes(pCurrentDir->filesList, updatedFileList, "Deleted", FALSE);
                findDiffNodes(updatedFileList, pCurrentDir->filesList, "Added", FALSE);
                findDiffNodes(pCurrentDir->filesList, updatedFileList, "Updated", TRUE);

                //Swap lists:
                LIST_copyList(&pCurrentDir->filesList, updatedFileList, fileInfoCopyFunction);
                LIST_destroy(updatedFileList);
                updatedFileList = LIST_create(releaseMemoryFile, getFileName);

                //Go to next directory:
                LIST_getNext(dirList, pDirInfoIterator, &pDirInfoIterator);
            } else {
                //Directory has been deleted. Remove it from dirList and continue to next Directory
                void* tmpPtrToDeletedDir = pDirInfoIterator;
                LIST_getNext(dirList, pDirInfoIterator, &pDirInfoIterator);
                LIST_removeElement(dirList, tmpPtrToDeletedDir);
            }
        }

        //Wait till next iteration:
        delay(SLEEP_TIME_SEC * 1000);
    }

    //Exit properly: clean memory mainly -
    exit:
    LIST_destroy(dirList);
    LIST_destroy(updatedFileList);
    DEBUG_PRINT("\n\nThanks for choosing Moneytor! Seeya again soon :)\n");
    return returnCodeMain;
}
