#include "inc/utills.h"
/**
 * @brief This function returns the last changed time_t of an spicified entry.
 * @param entryFullPath [IN] entry's full path.
 * @param lastChanged [OUT] valid pointer to time_t where last changed time will be stored.
 * @returns Appropriate returnCode_t.
 */
static returnCode_t getEntryLastChanged(char* entryFullPath, time_t* lastChanged);

/**
 * @brief This function create a full path to an entry using it's directory and name.
 * @param currentWorkingDirectory [IN] directory full path.
 * @param entryName [IN] entry's name.
 * @param entryFullPath [OUT] entry's full path.
 * @returns Appropriate returnCode_t.
 */
static returnCode_t getEntryFullPath(char* currentWorkingDirectory, char* entryName, char* entryFullPath) ;

/**
 * @brief This function gets path to an entry and returns it's type (file/dir/unknown) using the
 * listDataHandlersEntryType_t enum. if the function finds out that current entry is a directory - it calls
 * newDirFoundHandler for ferther proccesing.
 * @param pd [IN] entry's Dirent struct (after opened via opendir() and iterated via readdir()).
 * @param entryFullPath [IN] entry's full path.
 * @param pFileType [OUT] pointer to entry's type via listDataHandlersEntryType_t enum. Pointer mustn't be NULL.
 * @param dirList [IN, OUT] directory list in case of encountering new directory.
 * @returns Appropriate returnCode_t.
 */
static returnCode_t getEntryType(struct dirent* pd, char* entryFullPath, listDataHandlersEntryType_t* pFileType, LIST dirList);

/**
 * @brief This function handles encountering a new directory while iterating the current directory.
 * If directory doesn't exists in dirList, the function adds it to dirList.
 * @param newDirFullPath [IN] directory's full path.
 * @param dirList [IN,OUT] directory list to search in.
 * @returns Appropriate returnCode_t.
 */
static returnCode_t newDirFoundHandler(char* newDirFullPath, LIST dirList);

void delay(unsigned int ms) {
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

returnCode_t getFileList(DIR* pDir, char* currentWorkingDirectory, LIST fileList, LIST dirList) {
    struct dirent* pDirent;
    time_t lastChanged;
    listDataHandlersEntryType_t fileType;
    char entryFullPath[MAX_PATH_LENGTH];
    if ((NULL == pDir) || (NULL == currentWorkingDirectory) || (NULL == fileList) || (NULL == dirList)) {
        DEBUG_PRINT("One of the patameters to getFileList function is NULL. Please try again.");
        return RETURNCODE_LIST_UTILLS_GETFILELIST_PARAMETER_NULL;
    }
    rewinddir(pDir);
    while (NULL != (pDirent = readdir(pDir))) {
        returnCode_t currentReturnCode;

        //Skip self and father folder:
        if (0 == (strcmp(pDirent->d_name, "..")) || (0 == strcmp(pDirent->d_name, "."))) {
            continue;
        }

        // Get entry's properties:
        CHECK_RETURN_CODE(getEntryFullPath(currentWorkingDirectory, pDirent->d_name, entryFullPath))
        CHECK_RETURN_CODE(getEntryType(pDirent, entryFullPath, &fileType, dirList))
        CHECK_RETURN_CODE(getEntryLastChanged(entryFullPath, &lastChanged))

        // Add entry to current fileList:
        fileInfo_t* pFileInfo_t = createFileInfo_t(entryFullPath, lastChanged, fileType);
        if (NULL == pFileInfo_t) {
            DEBUG_PRINT("Could not create fileInfo_t in getFileList function. Please try again.");
            return RETURNCODE_LIST_UTILLS_GETFILELIST_COULD_NOT_ALLOCATE_MEMORY;
        }
        CHECK_RETURN_CODE(LIST_addElement(fileList, (void*)pFileInfo_t))
    }
    return RETURNCODE_SUCCESS;
}

static returnCode_t getEntryFullPath(char* currentWorkingDirectory, char* entryName, char* entryFullPath) {
    if (NULL == entryFullPath) {
        DEBUG_PRINT("Output pointer to the entry's full path is NULL. Please try again.");
        return RETURNCODE_LIST_UTILLS_GETENTRYFULLPATH_OUTPUT_POINTER_NULL;
    }
    if (strlen(currentWorkingDirectory) + strlen(entryName) + 1 > MAX_PATH_LENGTH) { //+1 for the '\' char
        DEBUG_PRINT("Current entry full path is longer than MAX_PATH_LENGTH.");
        return RETURNCODE_LIST_UTILLS_GETENTRYFULLPATH_PATH_LENGTH_EXCEEDS_MAX_PATH_LENGTH;
    }
    strcpy(entryFullPath, currentWorkingDirectory);
    if(strlen(entryFullPath) > 1 && entryFullPath[strlen(entryFullPath)-1] != '/' && entryFullPath[strlen(entryFullPath)-1] != '\\') {
        strcat(entryFullPath, "/");
    }
    strcat(entryFullPath, entryName);
    return RETURNCODE_SUCCESS;
}

static returnCode_t getEntryType(struct dirent* pd, char* entryFullPath, listDataHandlersEntryType_t* pFileType, LIST dirList) {
    if (NULL == pFileType) {
        DEBUG_PRINT("Pointer to file type output is NULL. Please try again.");
        return RETURNCODE_LIST_UTILLS_GETENTRYLASTCHANGED_OUTPUT_POINTER_NULL;
    }
    returnCode_t getEntryTypeReturnCode = RETURNCODE_SUCCESS;
    switch (pd->d_type) {
        case DT_REG:
            *pFileType = LIST_DATA_HANDLERS_ENTRY_TYPE_FILE;
            break;
        case DT_DIR:
            *pFileType = LIST_DATA_HANDLERS_ENTRY_TYPE_DIRECTORY;
            getEntryTypeReturnCode = newDirFoundHandler(entryFullPath, dirList);
            break;
        default:
            *pFileType = LIST_DATA_HANDLERS_ENTRY_TYPE_UNKNNOWN;
            break;
    }
    return getEntryTypeReturnCode;
}

static returnCode_t newDirFoundHandler(char* newDirFullPath, LIST dirList) {
    if ((NULL == newDirFullPath) || (NULL == dirList)) {
        DEBUG_PRINT("Pointer to entry's full path or to dirList are NULL. Please try again.");
        return RETURNCODE_LIST_UTILLS_NEWDIRFOUNDHANDLER_PARAMETER_NULL;
    }
    //Check if directory exists in DirsList, and if not - add it!
    void* pDirIt;
    for (pDirIt = LIST_getFirst(dirList); pDirIt != NULL; LIST_getNext(dirList, pDirIt, &pDirIt)) {
        if (0 == strcmp(newDirFullPath, ((dirInfo_t*)pDirIt)->dirName)) {
            break;
        }
    }

    //If we reached to the end of our for loop - we found a new dir! If so, add it to dirList:
    if (pDirIt == NULL) {
        dirInfo_t* pDirInfo_t = createDirInfo_t(newDirFullPath);
        if (NULL == pDirInfo_t) {
            DEBUG_PRINT("Could not create dirInfo_t in newDirFoundHandler function. Please try again.");
            return RETURNCODE_LIST_UTILLS_NEWDIRFOUNDHANDLER_COULD_NOT_ALLOCATE_MEMORY;
        }
        CHECK_RETURN_CODE(LIST_addElement(dirList, (void*)pDirInfo_t))
    }
    return RETURNCODE_SUCCESS;
}

static returnCode_t getEntryLastChanged(char* entryFullPath, time_t* lastChanged) {
    struct stat status;
    if (-1 == stat(entryFullPath, &status)) {
        DEBUG_PRINT("Could not read status of entry: %s. Please check permitions and try again.", entryFullPath);
        return RETURNCODE_LIST_UTILLS_GETENTRYLASTCHANGED_COULD_NOT_GET_FILE_STATUS;
    }
    *lastChanged = status.st_atim.tv_sec;
    return RETURNCODE_SUCCESS;
}

returnCode_t findDiffElements(LIST original, LIST updated, char* strToPrint, int updateCheck) {
    bool foundFile;
    void* originalData;
    void* updatedData;
    if ((NULL == original) || (NULL == updated) || (NULL == strToPrint)) {
        DEBUG_PRINT("One of the patameters to findDiffElements function is NULL. Please try again.");
        return RETURNCODE_LIST_UTILLS_FINDDIFFELEMENTS_PARAMETER_NULL;
    }
    for (originalData = LIST_getFirst(original); originalData != NULL; LIST_getNext(original, originalData, &originalData)) {
        foundFile = false;
        for (updatedData = LIST_getFirst(updated); updatedData != NULL; LIST_getNext(updated, updatedData, &updatedData)) {
            if (0 == strcmp( ((fileInfo_t*)originalData)->fileName, ((fileInfo_t*)updatedData)->fileName)) {
                foundFile = true;
                break;
            }
        }
        if (!updateCheck && !foundFile) {
            printf("File %s %s!", ((fileInfo_t*)originalData)->fileName, strToPrint);
        } else if (updateCheck && foundFile) {
            if (((fileInfo_t*)originalData)->lastChanged - ((fileInfo_t*)updatedData)->lastChanged != 0)
            printf("File %s %s!", ((fileInfo_t*)originalData)->fileName, strToPrint);
        }
    }
    return RETURNCODE_SUCCESS;
}

returnCode_t printDirTree(LIST dirList) {
    DEBUG_PRINT("**********************************************PrintDirTree**********************************************\n\n");
    DEBUG_PRINT("List of all subDirs:\n");
    CHECK_RETURN_CODE(LIST_print(dirList));
    DEBUG_PRINT("=================================\n\n");
    void* pVoidDirInfo_t;
    for (pVoidDirInfo_t = LIST_getFirst(dirList); pVoidDirInfo_t != NULL; LIST_getNext(dirList, pVoidDirInfo_t, &pVoidDirInfo_t)) {
        dirInfo_t* pDirInfo_t = (dirInfo_t*)pVoidDirInfo_t;
        // Check if element exists (could have been deleted and hence NULL):
        if (NULL != pDirInfo_t) {
            DEBUG_PRINT("File list in folder: %s:", pDirInfo_t->dirName);
            LIST_print(pDirInfo_t->filesList);
        }
    }
    DEBUG_PRINT("-----------------------------------------\n\n");
    if (NULL != pVoidDirInfo_t) {
        DEBUG_PRINT("Error iterating over directory list in printDirTree.")
        return RETURNCODE_LIST_UTILLS_PRINTDIRTREE_PRINT_FAILED;
    }
    return RETURNCODE_SUCCESS;
}