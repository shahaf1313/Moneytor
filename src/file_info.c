#include "inc/file_info.h"

char* getFileName(fileInfo_t* pFileInfo_t) {
    if (NULL == pFileInfo_t) {
        return NULL;
    }
    return pFileInfo_t->fileName;
}

int releaseMemoryFile(fileInfo_t* pFileInfo_t) {
    FREE(pFileInfo_t);
    return RETURNCODE_SUCCESS;
}

fileInfo_t* createFileInfo_t(char* name, time_t lastChanged, fileInfoEntryType_t fileType) {
    if (NULL == name) {
        DEBUG_PRINT("File name is NULL. Please try again. Returning NULL.");
        return NULL;
    }
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

fileInfo_t* fileInfoCopyFunction(fileInfo_t* pFileInfo_t) {
    if (NULL == pFileInfo_t) {
        return NULL;
    }
    fileInfo_t* pNewFileInfo = (fileInfo_t*) malloc(sizeof(*pNewFileInfo));
    if (NULL == pNewFileInfo) {
        DEBUG_PRINT("Memory allocation failed in fileInfoCopyFunction.");
        goto exit;
    }
    strcpy(pNewFileInfo->fileName, pFileInfo_t->fileName);
    pNewFileInfo->fileType = pFileInfo_t->fileType;
    pNewFileInfo->lastChanged = pFileInfo_t->lastChanged;
exit:
    return pNewFileInfo;
}

int fileInfoCompareFunction(fileInfo_t* pFile1, fileInfo_t* pFile2) {
    return strcmp(pFile1->fileName, pFile2->fileName);
}