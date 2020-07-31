#include "inc/main.h"
#include "inc/utills.h"

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char** argv) {
    return 0;
}
    /*
    //Variables declaration:
    char* pFolderPath;
    LIST fileList = LIST_create(releaseMemoryFile, getFileName);
    LIST dirList = LIST_create(releaseMemoryDir, getDirName);

    //Attach SIGINT to the termination handler:
    signal(SIGINT, intHandler);

    //Parse arguments form usr and open the folder's path
    if (argc != 2) {
        printf("Incompatible number of parameters. Usage: Moneytor <folder_path>\n");
        return -1;
    }
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);

    //Open folder and add folder to DirsList:
    if (pDir == NULL) {
        printf("Could not open specified directory. Usage: Monytor <dir_path>.\n");
        return -2;
    }
    dirInfo_t* pDirInfo_t = createDirInfo_t(pFolderPath);
    if (pDirInfo_t == NULL) {
        printf("Memory allocation falied. Free some memory and try again.\n");
        return 0;
    }
    LIST_addElement(dirList, (void*)pDirInfo_t);

    //Initialize file list for the first time, including all subdir if exists:
    pDirsList->first->pSubDirList = initList();
    getFileList(pDir, pDirsList->first->entryName, pDirsList->first->pSubDirList, pDirsList);
    closedir(pDir);
    node_t* pDirIt = pDirsList->first->next;
    while (pDirIt != NULL) {
        pDir = opendir(pDirIt->entryName);
        if (pDir != NULL) {
            //Whoohoo! Directory exists. Scan files and sub directories:
            //Read new file list:
            getFileList(pDir, pDirIt->entryName, pDirIt->pSubDirList, pDirsList);
            //Close directory:
            closedir(pDir);
        } else {
            printf("On initialization, could not print folder to read. FATAL ERROR!\n");
            return -1;
        }
        pDirIt = pDirIt->next;
    }
    //DBG: printDirTree(pDirsList);

    //Start guarding the folder :)
    while (keepRunning) {
        printf("\n====================...Scanning...====================\n");

        //Iterate over all directories in the monitored folder:
        node_t* pDirIt = pDirsList->first;
        while (pDirIt != NULL) {
            pDir = opendir(pDirIt->entryName);
            if (pDir != NULL) {
                //Whoohoo! Directory exists. Scan files and sub directories:
                //Read new file list:
                getFileList(pDir, pDirIt->entryName, pNewFileList, pDirsList);
                //Close directory:
                closedir(pDir);

                //Print changes:
                findDiffNodes(pDirIt->pSubDirList, pNewFileList, "Deleted", FALSE);
                findDiffNodes(pNewFileList, pDirIt->pSubDirList, "Added", FALSE);
                findDiffNodes(pDirIt->pSubDirList, pNewFileList, "Updated", TRUE);

                //Swap lists:
                deleteList(pDirIt->pSubDirList);
                pDirIt->pSubDirList = pNewFileList;
                pNewFileList = initList();
                pDirIt = pDirIt->next;
            } else {
                //Directory has been deleted. Remove it from dirList and continue to next Directory
                node_t* tmp = pDirIt->next;
                removeElement(pDirsList, pDirIt);
                pDirIt = tmp;
            }
        }

        //Wait till next iteration:
        delay(SLEEP_TIME_SEC * 1000);
    }

    //Exit properly: close dir, clean memory, etc:
    LIST_destroy(dirList);
    LIST_destroy(fileList);
    printf("\n\nThanks for choosing Moneytor! Seeya again soon :)\n\n");
    return 0;
}

*/