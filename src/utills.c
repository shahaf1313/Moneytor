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
static returnCode_t getEntryFullPath(char* currentWorkingDirectory, char* entryName, char* entryFullPath);

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
static returnCode_t
getEntryType(struct dirent* pd, char* entryFullPath, listDataHandlersEntryType_t* pFileType, LIST dirList);

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
    // CR: (DC) This is very bad, you're grinding the CPU while doing nothing!
    // CR: (DC) Instead, use the sleep() system call that puts your process to sleep and wakes it up
    // CR: (DC) after some time have elapsed.
    while (stopTime > clock());
}

// CR: (DC) "utils" module is just a name for a garbage module you can dump any function into.
// CR: (DC) This function performs filesystem scanning operations
// CR: (DC) Move it to a more properly named module, like file_system_scanner or file_system_walker
returnCode_t getFileList(DIR* pDir, char* currentWorkingDirectory, LIST fileList, LIST dirList) {
    // CR: (DC) NEVER leave variables uninitialized. They will contain the value of whetever garbage
    // CR: (DC) is in the stack right now, which is never a good thing.
    struct dirent* pDirent;
    time_t lastChanged;
    listDataHandlersEntryType_t fileType;
    char entryFullPath[MAX_PATH_LENGTH];
    if ((NULL == pDir) || (NULL == currentWorkingDirectory) || (NULL == fileList) || (NULL == dirList)) {
        DEBUG_PRINT("One of the patameters to getFileList function is NULL. Please try again.");
        return RETURNCODE_LIST_UTILLS_GETFILELIST_PARAMETER_NULL;
    }
    rewinddir(pDir);
    // CR: (DC) Never write ugly code such as that. Split the assignment and the check for NULL into
    // CR: (DC) two statements.
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
        CHECK_RETURN_CODE(LIST_addElement(fileList, (void*) pFileInfo_t))
    }
    return RETURNCODE_SUCCESS;
}

// CR: (DC) This function essentially joins two paths, just like Python's os.path.join
// CR: (DC) There's nothing stopping me from using this function to join two paths that aren't related
// CR: (DC) to a monitored entry. I can join two random paths of my choosing, and it will still work.
// CR: (DC) Thus, this function can use more general terms, and instead of using the terms "entry" and
// CR: (DC) "working directory", it can simply refer to the paths as paths.
// CR: (DC) For example, consider the following signature:
// CR: (DC) static returnCode_t joinPaths(char* path1, char* path2, char joined[MAX_PATH_LENGTH + 1])
// CR: (DC) Then, it is clear that this function is not entry-specific, but is a general function that
// CR: (DC) can join two paths.
// CR: (DC) Note that I defined "joined" as a char[MAX_PATH_LENGTH + 1] instead of a char*
// CR: (DC) To the compiler it doesn't matter (it will still treat "joined" as char*), but it strongly
// CR: (DC) indicates that you require "joined" to be an array of length MAX_PATH_LENGTH + 1, at the least
// CR: (DC) Note that the compiler DOES NOT VERIFY that the passed "joined" array is of length
// CR: (DC) MAX_PATH_LENGTH + 1. The compiler treats "joined" as a char*.
// CR: (DC) Also note the +1 in MAX_PATH_LENGTH + 1, which reserves a byte for the NULL terminator.
static returnCode_t getEntryFullPath(char* currentWorkingDirectory, char* entryName, char* entryFullPath) {
    if (NULL == entryFullPath) {
        DEBUG_PRINT("Output pointer to the entry's full path is NULL. Please try again.");
        // CR: (DC) Why LIST_UTILLS? This is UTILS. Fix everywhere.
        return RETURNCODE_LIST_UTILLS_GETENTRYFULLPATH_OUTPUT_POINTER_NULL;
    }
    // CR: (DC) What if my currentWorkingDirectory already has a slash? Then you don't need to add the 1
    // CR: (DC) to the calculation. I'm saying this because you did care for the case where my
    // CR: (DC) currentWorkingDirectory has a slash in the next if block.
    // CR: (DC) My suggestion is, instead of handling both a currentWorkingDirectory that has a slash and
    // CR: (DC) a currentWorkingDirectory that doesn't have slash, just assume it doesn't have a slash
    // CR: (DC) and add it. In the worst case, your path would look like: /path/to//file, which is awkward
    // CR: (DC) but is still valid.
    if (strlen(currentWorkingDirectory) + strlen(entryName) + 1 > MAX_PATH_LENGTH) { //+1 for the '\' char
        DEBUG_PRINT("Current entry full path is longer than MAX_PATH_LENGTH.");
        return RETURNCODE_LIST_UTILLS_GETENTRYFULLPATH_PATH_LENGTH_EXCEEDS_MAX_PATH_LENGTH;
    }
    // CR: (DC) Cast the return value of these to void, to indicate you don't want to check the return value
    // CR: (DC) For example: (void) strcpy(...)
    strcpy(entryFullPath, currentWorkingDirectory);
    if (strlen(entryFullPath) > 1 && entryFullPath[strlen(entryFullPath) - 1] != '/' &&
        entryFullPath[strlen(entryFullPath) - 1] != '\\') {
        strcat(entryFullPath, "/");
    }
    strcat(entryFullPath, entryName);
    return RETURNCODE_SUCCESS;
}

// CR: (DC) This function is badly designed. It is named getEntryType, which implies that I give it a
// CR: (DC) directory entry, and it will tell me what that entry's type is.
// CR: (DC) However, if you examine the code, you can see that if the type is a directory, it also
// CR: (DC) creates a new directory info structure and inserts it to a list
// CR: (DC) You should separate the logic for retrieving the entry type from the logic of creating a
// CR: (DC) directory info structure and appending it to a list
static returnCode_t getEntryType(struct dirent* pd, char* entryFullPath, listDataHandlersEntryType_t* pFileType, LIST dirList) {
static returnCode_t
getEntryType(struct dirent* pd, char* entryFullPath, listDataHandlersEntryType_t* pFileType, LIST dirList) {
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
    // CR: (DC) Note the subtle difference between a list iterator and list data
    // CR: (DC) pDirIt is the DATA, not an iterator. An iterator is something that can be used to
    // CR: (DC) obtain the data.
    // CR: (DC) So pDirIt should be renamed to pDirInfo
    void* pDirIt;
    // CR: (DC) What's the time complexity of your list iteration? Assume the list has N elements
    // CR: (DC) Is it O(N) or O(N^2)? What do you think should be time complexity?
    // CR: (DC) It stems from the fact that your LIST_getNext function iterates over the list from the
    // CR: (DC) beginning, trying to find the next element based on the data.
    // CR: (DC) This is very inefficient, and you implemented it that way because your LIST_getFirst
    // CR: (DC) and LIST_getNext functions return the DATA, and not an iterator. If I have the data only,
    // CR: (DC) I can't deduce where's the next element. But if I had an iterator, I could deduce where
    // CR: (DC) the next element is in O(1). In the list's case, the iterator is simply a node*
    // CR: (DC) So, we can change the LIST_getFirst, LIST_getNext and LIST_getLast functions to return
    // CR: (DC) an iterator, instead of the actual data.
    // CR: (DC) For instance:
    // CR: (DC)     typedef struct node_s node_t
    // CR: (DC)     node_t* LIST_getFirst(LIST)
    // CR: (DC)     node_t* LIST_getNext(LIST, node_t*)
    // CR: (DC)     node_t* LIST_getLast(LIST)
    // CR: (DC) Then, we can implement each of these function in O(1) time! But wait, how do I read the data?
    // CR: (DC) Because now all I have is an iterator, not the data itself. We can create this function:
    // CR: (DC)     void* LIST_getData(node_t*)
    // CR: (DC) And pass our iterator to it to extract the data.
    for (pDirIt = LIST_getFirst(dirList); pDirIt != NULL; LIST_getNext(dirList, pDirIt, &pDirIt)) {
        if (0 == strcmp(newDirFullPath, ((dirInfo_t*) pDirIt)->dirName)) {
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
        CHECK_RETURN_CODE(LIST_addElement(dirList, (void*) pDirInfo_t))
    }
    return RETURNCODE_SUCCESS;
}

// CR: (DC) Like getEntryFullPath, this function is also using the term "entry", however it is not really
// CR: (DC) specific to directory entries. We can look at it as a more general function that can take
// CR: (DC) a path to a file and tell us when the file was last changed
// CR: (DC) We can change the function's signature to:
// CR: (DC) static returnCode_t getLastChangedTime(char* fullPath, time_t* lastChanged)
static returnCode_t getEntryLastChanged(char* entryFullPath, time_t* lastChanged) {
    // CR: (DC) Don't EVER leave variables uninitialized
    struct stat status;
    if (-1 == stat(entryFullPath, &status)) {
        // CR: (DC) Why do you assume there is a permissions issue in case of failure?
        // CR: (DC) Read the man page for stat (man 3 stat), under the "Errors" section
        // CR: (DC) You can see that stat() may fail due to many reasons, one of the most common ones
        // CR: (DC) is that the file simply DOESN'T EXIST.
        // CR: (DC) What you're doing here is printing an error message that will only CONFUSE the poor
        // CR: (DC) developer trying to debug. Even the file doesn't even exist, you still tell the
        // CR: (DC) developer to check the permissions, sending him to on a futile quest in permission-land.
        // CR: (DC) You know what's worse than no error message? An error message that points me to the
        // CR: (DC) wrong direction and only wastes my valuable time. If you can't help me debug, then just
        // CR: (DC) don't.
        // CR: (DC) When you write error messages you should be very careful to only include data which is
        // CR: (DC) reliable, or point to directions that you're sure that are correct.
        // CR: (DC) If you truly want to help the developer debug the error, you can do something much more
        // CR: (DC) helpful. You can add the value of "errno" to the error message, which will tell the
        // CR: (DC) developer exactly what happened.
        DEBUG_PRINT("Could not read status of entry: %s. Please check permitions and try again.", entryFullPath);
        return RETURNCODE_LIST_UTILLS_GETENTRYLASTCHANGED_COULD_NOT_GET_FILE_STATUS;
    }
    *lastChanged = status.st_atim.tv_sec;
    return RETURNCODE_SUCCESS;
}

// CR: (DC) Why is updateCheck an int if it's only passed true or false? Change to bool
// CR: (DC) Also, name it like a question, for example: bool shouldCheckUpdate
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
        // CR: (DC) Extract a function LIST_find(LIST, void* data, compareFunction_t)
        // CR: (DC) This new function will take a list, a data element and a compare function, and will try
        // CR: (DC) to find the data element in the list, using the compare function given.
        for (updatedData = LIST_getFirst(updated); updatedData != NULL; LIST_getNext(updated, updatedData, &updatedData)) {
            if (0 == strcmp(((fileInfo_t*) originalData)->fileName, ((fileInfo_t*) updatedData)->fileName)) {
                foundFile = true;
                break;
            }
        }
        if (!updateCheck && !foundFile) {
            printf("File %s %s!\n", ((fileInfo_t*) originalData)->fileName, strToPrint);
        } else if (updateCheck && foundFile) {
            if ((((fileInfo_t*) originalData)->lastChanged - ((fileInfo_t*) updatedData)->lastChanged) != 0) {
                printf("File %s %s!\n", ((fileInfo_t*) originalData)->fileName, strToPrint);
            }
        }
    }
    return RETURNCODE_SUCCESS;
}

returnCode_t printDirTree(LIST dirList) {
    DEBUG_PRINT(
            "**********************************************PrintDirTree**********************************************\n\n");
    DEBUG_PRINT("List of all subDirs:\n");
    CHECK_RETURN_CODE(LIST_print(dirList));
    DEBUG_PRINT("=================================\n\n");
    void* pVoidDirInfo_t;
    for (pVoidDirInfo_t = LIST_getFirst(dirList);
         pVoidDirInfo_t != NULL; LIST_getNext(dirList, pVoidDirInfo_t, &pVoidDirInfo_t)) {
        dirInfo_t* pDirInfo_t = (dirInfo_t*) pVoidDirInfo_t;
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