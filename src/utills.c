#include "inc/utills.h"

void delay(unsigned int ms) {
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
    // CR: (DC) This is very bad, you're grinding the CPU while doing nothing!
    // CR: (DC) Instead, use the sleep() system call that puts your process to sleep and wakes it up
    // CR: (DC) after some time have elapsed.
    // CR: (SE) sleep() doen't work! probably due to SIGINT signal handler :/
}

returnCode_t joinPaths(char* path1, char* path2, char joined[MAX_PATH_LENGTH + 1]) {
    if (NULL == joined) {
        DEBUG_PRINT("Output pointer to joined path is NULL. Please try again.");
        return RETURNCODE_UTILLS_JOINPATHS_OUTPUT_POINTER_NULL;
    }
    if (strlen(path1) + strlen(path2) > MAX_PATH_LENGTH + 1) { //+1 for the '\0' char
        DEBUG_PRINT("Current entry full path is longer than MAX_PATH_LENGTH.");
        return RETURNCODE_UTILLS_JOINPATHS_PATH_LENGTH_EXCEEDS_MAX_PATH_LENGTH;
    }
    (void) strcpy(joined, path1);
    (void) strcat(joined, "/");
    (void) strcat(joined, path2);
    return RETURNCODE_SUCCESS;
}

returnCode_t getEntryType(struct dirent* pd, fileInfoEntryType_t* pFileType) {
    if (NULL == pFileType) {
        DEBUG_PRINT("Pointer to file type output is NULL. Please try again.");
        return RETURNCODE_UTILLS_GETENTRYLASTCHANGED_OUTPUT_POINTER_NULL;
    }
    switch (pd->d_type) {
        case DT_REG:
            *pFileType = FILE_INFO_ENTRY_TYPE_FILE;
            break;
        case DT_DIR:
            *pFileType = FILE_INFO_ENTRY_TYPE_DIRECTORY;
            break;
        default:
            *pFileType = FILE_INFO_ENTRY_TYPE_UNKNOWN;
            break;
    }
    return RETURNCODE_SUCCESS;
}

returnCode_t newDirFoundHandler(char* newDirFullPath, LIST dirList) {
    if ((NULL == newDirFullPath) || (NULL == dirList)) {
        DEBUG_PRINT("Pointer to entry's full path or to dirList are NULL. Please try again.");
        return RETURNCODE_UTILLS_NEWDIRFOUNDHANDLER_PARAMETER_NULL;
    }
    //Check if directory exists in DirsList, and if not - add it!
    node_t* pNodeIterator = NULL;
    for (pNodeIterator = LIST_getFirst(dirList); pNodeIterator != NULL; LIST_getNext(dirList, pNodeIterator, &pNodeIterator)) {
        dirInfo_t* pDirInfo_t = LIST_getData(pNodeIterator);
        if ((NULL != pDirInfo_t) && (0 == strcmp(newDirFullPath, pDirInfo_t->dirName))) {
            break;
        }
    }

    //If we reached to the end of our for loop - we found a new dir! If so, add it to dirList:
    if (pNodeIterator == NULL) {
        dirInfo_t* pDirInfo_t = createDirInfo_t(newDirFullPath);
        if (NULL == pDirInfo_t) {
            DEBUG_PRINT("Could not create dirInfo_t in newDirFoundHandler function. Please try again.");
            return RETURNCODE_UTILLS_NEWDIRFOUNDHANDLER_COULD_NOT_ALLOCATE_MEMORY;
        }
        CHECK_RETURN_CODE(LIST_addElement(dirList, (void*) pDirInfo_t));
    }
    return RETURNCODE_SUCCESS;
}

returnCode_t getLastChangedTime(char* fullPath, time_t* lastChanged) {
    struct stat status = {0};
    if (-1 == stat(fullPath, &status)) {
        DEBUG_PRINT("Could not read status of entry: %s. errno is: %d", fullPath, errno);
        return RETURNCODE_UTILLS_GETENTRYLASTCHANGED_COULD_NOT_GET_FILE_STATUS;
    }
    *lastChanged = status.st_mtim.tv_sec;
    return RETURNCODE_SUCCESS;
}

returnCode_t findDiffElements(LIST originalList, LIST updatedList, const char* strToPrint, bool shouldCheckUpdate) {
    node_t* pIteratorOriginal = NULL;
    node_t* pIteratorUpdated = NULL;
    fileInfo_t* originalFile = NULL;
    fileInfo_t* updatedFile = NULL;

    if ((NULL == originalList) || (NULL == updatedList) || (NULL == strToPrint)) {
        DEBUG_PRINT("One of the patameters to findDiffElements function is NULL. Please try again.");
        return RETURNCODE_UTILLS_FINDDIFFELEMENTS_PARAMETER_NULL;
    }

    for (pIteratorOriginal = LIST_getFirst(originalList); pIteratorOriginal != NULL; LIST_getNext(originalList, pIteratorOriginal, &pIteratorOriginal)) {
        bool foundFile = false;
        CHECK_RETURN_CODE(LIST_find(updatedList, LIST_getData(pIteratorOriginal), fileInfoCompareFunction, &foundFile, &pIteratorUpdated));

        originalFile = (fileInfo_t*) LIST_getData(pIteratorOriginal);
        if (foundFile) {
            updatedFile = (fileInfo_t*) LIST_getData(pIteratorUpdated);
        }

        if (!shouldCheckUpdate && !foundFile) {
            printf("File %s %s!\n", originalFile->fileName, strToPrint);
        } else if (shouldCheckUpdate && foundFile) {
            if ((originalFile->lastChanged - updatedFile->lastChanged) != 0) {
                printf("File %s %s!\n", originalFile->fileName, strToPrint);
            }
        }
    }
    return RETURNCODE_SUCCESS;
}

returnCode_t printDirTree(LIST dirList) {
    DEBUG_PRINT("**********************************************PrintDirTree**********************************************\n\n");
    DEBUG_PRINT("List of all subDirs:\n");
    CHECK_RETURN_CODE(LIST_print(dirList));
    DEBUG_PRINT("=================================\n\n");
    node_t* pNodeIterator;
    for (pNodeIterator = LIST_getFirst(dirList); pNodeIterator != NULL; LIST_getNext(dirList, pNodeIterator, &pNodeIterator)) {
        dirInfo_t* pDirInfo_t = (dirInfo_t*) LIST_getData(pNodeIterator);
        if (NULL != pDirInfo_t) {
            DEBUG_PRINT("File list in folder: %s:", pDirInfo_t->dirName);
            LIST_print(pDirInfo_t->filesList);
        }
    }
    DEBUG_PRINT("-----------------------------------------\n\n");
    if (NULL != pNodeIterator) {
        DEBUG_PRINT("Error iterating over directory list in printDirTree.");
        return RETURNCODE_UTILLS_PRINTDIRTREE_PRINT_FAILED;
    }
    return RETURNCODE_SUCCESS;
}