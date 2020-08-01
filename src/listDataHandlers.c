#include "inc/listDataHandlers.h"
#include "inc/list.h"

char* getFileName(void* pFileInfo_t) {
    if (pFileInfo_t == NULL) {
        return  NULL;
    }
    return ((fileInfo_t*)pFileInfo_t)->fileName;
}

char* getDirName(void* pDirInfo_t) {
    if (pDirInfo_t == NULL) {
        return NULL;
    }
    return ((dirInfo_t*)pDirInfo_t)->dirName;
}

int releaseMemoryFile(void* pFileInfo_t) {
    free(pFileInfo_t);
    pFileInfo_t = NULL;
    return 0;
}

int releaseMemoryDir(void* pDirInfo_t) {
    dirInfo_t* pDirInfo = (dirInfo_t*)pDirInfo_t;
    int returnCode = LIST_destroy(pDirInfo->filesList);
    if ( returnCode != 0) {
        DEBUG_PRINT("Could not destroy subDirsList in dirInfo_t's releaseMemory function. "
                    "Return code has been returned, and memory of dirInfo_t WAS NOT RELEASED.");
        return returnCode;
    }
    free(pDirInfo);
    pDirInfo_t = pDirInfo = NULL;
    return 0;
}

fileInfo_t* createFileInfo_t(char* name, time_t lastChanged, listDataHandlersEntryType_t fileType) {
    fileInfo_t* pFileInfo_t = (fileInfo_t*)malloc(sizeof(*pFileInfo_t));
    if (pFileInfo_t == NULL) {
        printf("Memory allocation failed in createFileInfo_t function. Free memory and try again.\n");
        return NULL;
    }
    //todo: check if name and file type are MAX_PATH_LENGTH. if bigger - return flase.
    strcpy(pFileInfo_t->fileName, name);
    pFileInfo_t->fileType = fileType;
    pFileInfo_t->lastChanged = lastChanged;
    return pFileInfo_t;
}

dirInfo_t* createDirInfo_t(char* name) {
    dirInfo_t* pDirInfo_t = (dirInfo_t*)malloc(sizeof(*pDirInfo_t));
    if (pDirInfo_t == NULL) {
        DEBUG_PRINT("Memory allocation failed in createDirInfo_t function. Free memory and try again.");
        return NULL;
    }
    //todo: check if name is MAX_PATH_LENGTH. if bigger - return flase.
    strcpy(pDirInfo_t->dirName, name);
    pDirInfo_t->filesList = LIST_create(releaseMemoryFile, getFileName);
    if (pDirInfo_t->filesList == NULL) {
        DEBUG_PRINT("LIST_create failed in createDirInfo_t function. Free memory and try again.");
        return NULL;
    }
    return pDirInfo_t;
}