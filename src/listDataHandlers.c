#include "inc/listDataHandlers.h"
#include "inc/list.h"

char* getFileName(void* pFileInfo_t) {
    if (NULL == pFileInfo_t) {
        return NULL;
    }
    return ((fileInfo_t*) pFileInfo_t)->fileName;
}

char* getDirName(void* pDirInfo_t) {
    if (NULL == pDirInfo_t) {
        return NULL;
    }
    return ((dirInfo_t*) pDirInfo_t)->dirName;
}

int releaseMemoryFile(void* pFileInfo_t) {
    FREE(pFileInfo_t);
    return RETURNCODE_SUCCESS;
}

int releaseMemoryDir(void* pDirInfo_t) {
    dirInfo_t* pDirInfo = (dirInfo_t*) pDirInfo_t;
    returnCode_t returnCode = LIST_destroy(pDirInfo->filesList);
    if (RETURNCODE_SUCCESS != returnCode) {
        DEBUG_PRINT("Could not destroy subDirsList in dirInfo_t's releaseMemory function. "
                    "Return code has been returned, and memory of dirInfo_t WAS NOT RELEASED.");
        return returnCode;
    }
    FREE(pDirInfo_t);
    return RETURNCODE_SUCCESS;
}

fileInfo_t* createFileInfo_t(char* name, time_t lastChanged, listDataHandlersEntryType_t fileType) {
    if (strlen(name) > MAX_PATH_LENGTH) {
        DEBUG_PRINT("File name exceeds MAX_PATH_LENGTH in createFileInfo_t. Memory was not allocated. Returning NULL.");
        return NULL;
    }
    fileInfo_t* pFileInfo_t = (fileInfo_t*) malloc(sizeof(*pFileInfo_t));
    if (NULL == pFileInfo_t) {
        DEBUG_PRINT("Memory allocation failed in createFileInfo_t function. Free memory and try again.\n");
        goto exit;
    }
    strcpy(pFileInfo_t->fileName, name);
    pFileInfo_t->fileType = fileType;
    pFileInfo_t->lastChanged = lastChanged;
exit:
    return pFileInfo_t;
}

dirInfo_t* createDirInfo_t(char* name) {
    if (strlen(name) > MAX_PATH_LENGTH) {
        DEBUG_PRINT(
                "Directory name exceeds MAX_PATH_LENGTH in createDirInfo_t. Memory was not allocated. Returning NULL.");
        return NULL;
    }
    dirInfo_t* pDirInfo_t = (dirInfo_t*) malloc(sizeof(*pDirInfo_t));
    if (NULL == pDirInfo_t) {
        DEBUG_PRINT("Memory allocation failed in createDirInfo_t function. Free memory and try again.");
        goto cleanup;
    }
    strcpy(pDirInfo_t->dirName, name);
    pDirInfo_t->filesList = LIST_create(releaseMemoryFile, getFileName);
    if (NULL == pDirInfo_t->filesList) {
        DEBUG_PRINT("LIST_create failed in createDirInfo_t function. Free memory and try again.");
        goto cleanup;
    }
    goto  exit;
cleanup:
    // We don't need to check if fileList has created, because there is no use case that we will reach this line
    // and that will be the case. So we just need to FREE the dirInfo_t.
    FREE(pDirInfo_t);
exit:
    return pDirInfo_t;
}

void* fileInfoCopyFunction(void* pVoidFileInfo) {
    fileInfo_t* pFileInfo = (fileInfo_t*) pVoidFileInfo;
    if (NULL == pFileInfo) {
        return NULL;
    }
    fileInfo_t* pNewFileInfo = (fileInfo_t*) malloc(sizeof(*pNewFileInfo));
    if (NULL == pNewFileInfo) {
        DEBUG_PRINT("Memory allocation failed in fileInfoCopyFunction.");
        goto exit;
    }
    strcpy(pNewFileInfo->fileName, pFileInfo->fileName);
    pNewFileInfo->fileType = pFileInfo->fileType;
    pNewFileInfo->lastChanged = pFileInfo->lastChanged;
exit:
    return (void*) pNewFileInfo;
}