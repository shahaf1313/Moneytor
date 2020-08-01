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

    //Attach SIGINT to the termination handler:
    signal(SIGINT, intHandler);

    //Parse arguments form usr and open the folder's path
    if (argc != 2) {
        printf("Incompatible number of parameters. Usage: Moneytor <folder_path>\n");
        return RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER;
    }
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);

    //Open folder and add folder to DirsList:
    if (pDir == NULL) {
        printf("Could not open specified directory. Usage: Monytor <dir_path>.\n");
        return RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
    }
    dirInfo_t* pBaseDirInfo = createDirInfo_t(pFolderPath);
    if (pBaseDirInfo == NULL) {
        printf("Memory allocation falied. Free some memory and try again.\n");
        return RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED;
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
            //Read new file list:
            getFileList(pDir, pCurrentDir->dirName, pCurrentDir->filesList, dirList);
            //Close directory:
            closedir(pDir);
        } else {
            printf("On initialization, could not print folder to read. FATAL ERROR!\n");
            return RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER;
        }
        LIST_getNext(dirList, pDirInfoIt, &pDirInfoIt);
    }
    // Check initialization:
    printDirTree(dirList);

    //Start guarding the folder :)
    while (keepRunning) {
        printf("\n====================...Scanning...====================\n");

        //Iterate over all directories in the monitored folder:
        void* pDirInfoIterator = LIST_getFirst(dirList);
        while (pDirInfoIterator != NULL) {
            dirInfo_t* pCurrentDir = (dirInfo_t*)pDirInfoIterator;
            pDir = opendir(pCurrentDir->dirName);
            if (pDir != NULL) {
                //Whoohoo! Directory exists. Scan files and sub directories:
                //Read new file list:
                getFileList(pDir, pCurrentDir->dirName, updatedFileList, dirList);
                //Close directory:
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

    //Exit properly: close dir, clean memory, etc:
    LIST_destroy(dirList);
    LIST_destroy(updatedFileList);
    printf("\n\nThanks for choosing Moneytor! Seeya again soon :)\n\n");
    return 0;
}
