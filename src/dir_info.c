#include "inc/dir_info.h"
#include "inc/file_info.h"

char* getDirName(dirInfo_t* pDirInfo_t) {
    if (NULL == pDirInfo_t) {
        return NULL;
    }
    return pDirInfo_t->dirName;
}

int releaseMemoryDir(dirInfo_t* pDirInfo_t) {
    if (NULL == pDirInfo_t) {
        return RETURNCODE_SUCCESS;
    }

    if (RETURNCODE_SUCCESS != (LIST_destroy(pDirInfo_t->filesList))) {
        DEBUG_PRINT("Could not destroy filesList in dirInfo_t's releaseMemory function.");
        pDirInfo_t->filesList = NULL;
    }
    FREE(pDirInfo_t);
    return RETURNCODE_SUCCESS;
}

dirInfo_t* createDirInfo_t(char* name) {
    if (NULL == name) {
        DEBUG_PRINT("File name is NULL. Please try again. Returning NULL.");
        return NULL;
    }

    if (strlen(name) > MAX_PATH_LENGTH) {
        DEBUG_PRINT("Directory name exceeds MAX_PATH_LENGTH in createDirInfo_t. Memory was not allocated. Returning NULL.");
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

int dirInfoCompareFunction(dirInfo_t* pDir1, dirInfo_t* pDir2) {
    return strcmp(pDir1->dirName, pDir2->dirName);
}