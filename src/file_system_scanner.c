#include "inc/file_system_scanner.h"

returnCode_t getFileList(DIR* pDir, char* currentWorkingDirectory, LIST fileList, LIST dirList) {
    struct dirent* pDirent = NULL;
    time_t lastChanged = 0;
    fileInfoEntryType_t fileType = FILE_INFO_ENTRY_TYPE_FILE;
    char entryFullPath[MAX_PATH_LENGTH + 1] = "\0";
    if ((NULL == pDir) || (NULL == currentWorkingDirectory) || (NULL == fileList) || (NULL == dirList)) {
        DEBUG_PRINT("One of the patameters to getFileList function is NULL. Please try again.");
        return RETURNCODE_UTILLS_GETFILELIST_PARAMETER_NULL;
    }

    rewinddir(pDir);
    for (pDirent = readdir(pDir); NULL != pDirent; pDirent = readdir(pDir)) {
        //Skip self and father folder:
        if (0 == (strcmp(pDirent->d_name, "..")) || (0 == strcmp(pDirent->d_name, "."))) {
            continue;
        }

        CHECK_RETURN_CODE(joinPaths(currentWorkingDirectory, pDirent->d_name, entryFullPath));
        CHECK_RETURN_CODE(getEntryType(pDirent, &fileType));
        if (fileType == FILE_INFO_ENTRY_TYPE_DIRECTORY) {
            CHECK_RETURN_CODE(newDirFoundHandler(entryFullPath, dirList));
        }
        CHECK_RETURN_CODE(getLastChangedTime(entryFullPath, &lastChanged));

        // Add entry to current fileList:
        fileInfo_t* pFileInfo_t = createFileInfo_t(entryFullPath, lastChanged, fileType);
        if (NULL == pFileInfo_t) {
            DEBUG_PRINT("Could not create fileInfo_t in getFileList function. Please try again.");
            return RETURNCODE_UTILLS_GETFILELIST_COULD_NOT_ALLOCATE_MEMORY;
        }
        CHECK_RETURN_CODE(LIST_addElement(fileList, (void*) pFileInfo_t));
    }
    return RETURNCODE_SUCCESS;
}