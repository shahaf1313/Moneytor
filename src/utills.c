#include "inc/utills.h"

// This function makes a delay of ms milliseconds
void delay(unsigned int ms) {
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}
/*
// This function receives pointer to a directory and stores each entry in a string array (future - list)
int getFileList(DIR* pDir, char* fullDirPath, LIST fileList, LIST dirList) {
    struct stat status;
    struct dirent* pDirent;
    time_t lastChanged;
    char* pFileType, entryName[MAX_PATH_LENGTH];
    int i = 0;
    rewinddir(pDir);
    while ((pDirent = readdir(pDir)) != NULL) {
        //Skip self and father folder:
        if ((strcmp(pDirent->d_name, "..") == 0) || (strcmp(pDirent->d_name, ".") == 0)) {
            continue;
        }

        //Get entry's full path:
        strcpy(entryName, fullDirPath);
        strcat(entryName, "/");
        strcat(entryName, pDirent->d_name);

        //Get entry's type and full path:
        pFileType = getEntryType(pDirent, pDirList, entryName);

        //Check entry's last changed time:
        if (stat(entryName, &status) == -1) {
            printf("Could not read status of entry: %s. Please check permitions and try again.", pDirent->d_name);
            lastChanged = 0;
        } else {
            lastChanged = status.st_atim.tv_sec;
        }
        addElement(pFileList, entryName, lastChanged, pFileType);
        ++i;

        //dbg: printf("[%s], %s, last changed in %s\n", pDirent->d_name, pFileType,  asctime(gmtime(&lastChanged)));
    }
    return i;
}

// This function gets two arrays lists of strings and prints all the strings that differ from the first
*
list to
the second, with

the strToPrint(Added

/Deleted)

void findDiffNodes(LIST original, LIST updated, char* strToPrint, int updateCheck) {
    int foundFile;
    node_t* pOriginalNode = original->first, * pUpdatedNode;
    while (pOriginalNode != NULL) {
        foundFile = FALSE;
        pUpdatedNode = updated->first;

        while (pUpdatedNode != NULL) {
            if (strcmp(pOriginalNode->entryName, pUpdatedNode->entryName) == 0) {
                foundFile = TRUE;
                break;
            }
            pUpdatedNode = pUpdatedNode->next;
        }

        if (!updateCheck && !foundFile) {
            printf("File %s %s!\n", pOriginalNode->entryName, strToPrint);
        } else if (updateCheck && foundFile) {
            if (pOriginalNode->lastChanged - pUpdatedNode->lastChanged != 0)
                printf("File %s %s!\n", pOriginalNode->entryName, strToPrint);
        }

        pOriginalNode = pOriginalNode->next;
    }
}

// This function receives an entry in a folder and returns it's type and full path.
*
If the
current entry
is a
directory,
the function
searches it
in dirList
and if
it doen
't exists, adds it to the list.

char* getEntryType(struct dirent* pd, LIST dirList, char* entryName) {
    char* pFileType;
    switch (pd->d_type) {
        case DT_REG:
            pFileType = "File";
            break;
        case DT_DIR:
            pFileType = "Directory";
            //Check if directory exists in DirsList, and if not - add it!
            node_t* pDirIt = pDirList->first;
            while (pDirIt != NULL) {
                if (strcmp(entryName, pDirIt->entryName) == 0) {
                    break;
                }
                pDirIt = pDirIt->next;
            }
            //If pDirIt is NULL - you didn't find any directory in DirsList that matches to this dir.
            *Thus, it
            is
                    a
            new
                    sub
            directory.Add
            it
                    to
            DirsList
            !
            if (pDirIt == NULL && (pDirList->counter > 0)) {
                addElement(pDirList, entryName, 0, pFileType);
                pDirList->last->pSubDirList = initList();
            }
            break;
        default:
            pFileType = "Unknown";
            break;
    }
    return pFileType;
}
*/
// This function prints the entire tree of files and subfolders in dirList
void printDirTree(LIST dirList) {
    DEBUG_PRINT("**********************************************PrintDirTree**********************************************\n\n");
    DEBUG_PRINT("List of all subDirs:\n");
    LIST_print(dirList);
    DEBUG_PRINT("=================================\n\n");
    int n = LIST_getLength(dirList), returnCode = 0;
    void* pVoidDirInfo_t = LIST_getFirst(dirList);
    for (int i = 0; i < n; ++i)
    {
        dirInfo_t* pDirInfo_t = (dirInfo_t*)pVoidDirInfo_t;
        // Check if element exists (could have been deleted and hence NULL):
        if (pDirInfo_t != NULL) {
            DEBUG_PRINT("File list in folder: %s:", pDirInfo_t->dirName);
            LIST_print(pDirInfo_t->filesList);
        }
        returnCode = LIST_getNext(dirList, pVoidDirInfo_t, &pVoidDirInfo_t);
        if (i < n-1 && returnCode != 0) {
            break;
        }
    }
    if (returnCode != RETURNCODE_LIST_GETNEXT_LAST_ELEMENT) {
        DEBUG_PRINT("Error iterating over directory list in printDirTree.");
        return;
    }
    DEBUG_PRINT("-----------------------------------------\n\n");
}

