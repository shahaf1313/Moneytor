#include "inc/utills.h"
/**
 * @brief This function returns the last changed time_t of an spicified entry.
 * @param entryFullPath [IN] entry's full path.
 * @return entry's last changed time in time_t format.
 */
static time_t getEntryLastChanged(char* entryFullPath);

/**
 * @brief This function create a full path to an entry using it's directory and name.
 * @param currentWorkingDirectory [IN] directory full path.
 * @param entryName [IN] entry's name.
 * @param entryFullPath [OUT] entry's full path.
 */
static void getEntryFullPath(char* currentWorkingDirectory, char* entryName, char* entryFullPath);

/**
 * @brief This function gets path to an entry and returns it's type (file/dir/unknown) using the
 * listDataHandlersEntryType_t enum. if the function finds out that current entry is a directory - it calls
 * newDirFoundHandler for ferther proccesing.
 * @param pd [IN] entry's Dirent struct (after opened via opendir() and iterated via readdir()).
 * @param entryFullPath [IN] entry's full path.
 * @param dirList [IN] directory list in case of encountering new directory.
 * @return entry's type via listDataHandlersEntryType_t enum.
 */
static listDataHandlersEntryType_t getEntryType(struct dirent* pd, char* entryFullPath, LIST dirList);

/**
 * @brief This function handles encountering a new directory while iterating the current directory.
 * If directory doesn't exists in dirList, the function adds it to dirList.
 * @param newDirFullPath [IN] directory's full path.
 * @param dirList [IN,OUT] directory list to search in.
 */
static void newDirFoundHandler(char* newDirFullPath, LIST dirList);

void delay(unsigned int ms) {
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

int getFileList(DIR* pDir, char* currentWorkingDirectory, LIST fileList, LIST dirList) {
    struct dirent* pDirent;
    time_t lastChanged;
    listDataHandlersEntryType_t fileType;
    char entryFullPath[MAX_PATH_LENGTH];
    int i = 0;

    rewinddir(pDir);
    while (NULL != (pDirent = readdir(pDir))) {
        //Skip self and father folder:
        if (0 == (strcmp(pDirent->d_name, "..")) || (0 == strcmp(pDirent->d_name, "."))) {
            continue;
        }

        // Get entry's properties:
        getEntryFullPath(currentWorkingDirectory, pDirent->d_name, entryFullPath);
        fileType = getEntryType(pDirent, entryFullPath, dirList);
        lastChanged = getEntryLastChanged(entryFullPath);

        // Add entry to current fileList:
        fileInfo_t* pFileInfo_t = createFileInfo_t(entryFullPath, lastChanged, fileType);
        LIST_addElement(fileList, (void*)pFileInfo_t);
        ++i;
    }
    return i;
}

static void getEntryFullPath(char* currentWorkingDirectory, char* entryName, char* entryFullPath) {
    strcpy(entryFullPath, currentWorkingDirectory);
    if(strlen(entryFullPath) > 1 && entryFullPath[strlen(entryFullPath)-1] != '/' && entryFullPath[strlen(entryFullPath)-1] != '\\') {
        strcat(entryFullPath, "/");
    }
    strcat(entryFullPath, entryName);
}

listDataHandlersEntryType_t getEntryType(struct dirent* pd, char* entryFullPath, LIST dirList) {
    listDataHandlersEntryType_t fileType;
    switch (pd->d_type) {
        case DT_REG:
            fileType = LIST_DATA_HANDLERS_ENTRY_TYPE_FILE;
            break;
        case DT_DIR:
            fileType = LIST_DATA_HANDLERS_ENTRY_TYPE_DIRECTORY;
            newDirFoundHandler(entryFullPath, dirList);
            break;
        default:
            fileType = LIST_DATA_HANDLERS_ENTRY_TYPE_UNKNNOWN;
            break;
    }
    return fileType;
}

static void newDirFoundHandler(char* newDirFullPath, LIST dirList) {
    //Check if directory exists in DirsList, and if not - add it!
    void* pDirIt = LIST_getFirst(dirList);
    int dirListLength, i;
    LIST_getLength(dirList, &dirListLength);
    for (i = 0; i < dirListLength; ++i)
    {
        if (0 == strcmp(newDirFullPath, ((dirInfo_t*)pDirIt)->dirName)) {
            break;
        }
        LIST_getNext(dirList, pDirIt, &pDirIt);
    }

    //If we reached to the end of our for loop - we found a new dir! If so, add it to dirList:
    if (dirListLength > 0 && i == dirListLength) {
        dirInfo_t* pDirInfo_t = createDirInfo_t(newDirFullPath);
        LIST_addElement(dirList, (void*)pDirInfo_t);
    }
}

static time_t getEntryLastChanged(char* entryFullPath) {
    struct stat status;
    if (-1 == stat(entryFullPath, &status)) {
        DEBUG_PRINT("Could not read status of entry: %s. Please check permitions and try again.", entryFullPath);
        return 0;
    }
    return status.st_atim.tv_sec;
}

void findDiffNodes(LIST original, LIST updated, char* strToPrint, int updateCheck) {
    int foundFile;
    void* originalData = LIST_getFirst(original);
    void* updatedData;
    int originalLength, updatedLength;
    LIST_getLength(original, &originalLength);
    LIST_getLength(updated, &updatedLength);
    for (int i = 0; i < originalLength; ++i) {
        foundFile = FALSE;
        updatedData = LIST_getFirst(updated);
        for (int j = 0; j < updatedLength; ++j) {
            if (0 == strcmp( ((fileInfo_t*)originalData)->fileName, ((fileInfo_t*)updatedData)->fileName)) {
                foundFile = TRUE;
                break;
            }
            LIST_getNext(updated, updatedData, &updatedData);
        }

        if (!updateCheck && !foundFile) {
            DEBUG_PRINT("File %s %s!", ((fileInfo_t*)originalData)->fileName, strToPrint);
        } else if (updateCheck && foundFile) {
            if (((fileInfo_t*)originalData)->lastChanged - ((fileInfo_t*)updatedData)->lastChanged != 0)
            DEBUG_PRINT("File %s %s!", ((fileInfo_t*)originalData)->fileName, strToPrint);
        }
        LIST_getNext(original, originalData, &originalData);
    }
}

void printDirTree(LIST dirList) {
    DEBUG_PRINT("**********************************************PrintDirTree**********************************************\n\n");
    DEBUG_PRINT("List of all subDirs:\n");
    LIST_print(dirList);
    DEBUG_PRINT("=================================\n\n");
    int dirListLength, returnCode = RETURNCODE_SUCCESS;
    LIST_getLength(dirList, &dirListLength);
    void* pVoidDirInfo_t = LIST_getFirst(dirList);
    for (int i = 0; i < dirListLength; ++i)
    {
        dirInfo_t* pDirInfo_t = (dirInfo_t*)pVoidDirInfo_t;
        // Check if element exists (could have been deleted and hence NULL):
        if (NULL != pDirInfo_t) {
            DEBUG_PRINT("File list in folder: %s:", pDirInfo_t->dirName);
            LIST_print(pDirInfo_t->filesList);
        }
        returnCode = LIST_getNext(dirList, pVoidDirInfo_t, &pVoidDirInfo_t);
        if (i < dirListLength-1 && returnCode != 0) {
            break;
        }
    }
    if (returnCode != RETURNCODE_LIST_GETNEXT_LAST_ELEMENT) {
        DEBUG_PRINT("Error iterating over directory list in printDirTree.");
        return;
    }
    DEBUG_PRINT("-----------------------------------------\n\n");
}

