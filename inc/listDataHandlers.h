#ifndef MONEYTOR_LISTDATAHANDLERS_H
#define MONEYTOR_LISTDATAHANDLERS_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "inc/common.h"
#include "inc/list.h"

#define MAX_PATH_LENGTH 255

typedef enum {
    LIST_DATA_HANDLERS_ENTRY_TYPE_FILE = 1,
    LIST_DATA_HANDLERS_ENTRY_TYPE_DIRECTORY,
    LIST_DATA_HANDLERS_ENTRY_TYPE_UNKNNOWN
} listDataHandlersEntryType_t;

typedef struct fileInfo {
    char fileName[MAX_PATH_LENGTH];
    time_t lastChanged;
    listDataHandlersEntryType_t fileType;
} fileInfo_t;

typedef struct dirInfo {
    char dirName[MAX_PATH_LENGTH];
    LIST filesList;
} dirInfo_t;

/**
 * @brief This function gets fileInfo_t's name.
 * @param pFileInfo_t [IN] pointer to fileInfo_t.
 * @returns On success - pointer to the name string, on failure - NULL.
**/
char* getFileName(void* pFileInfo_t);

/**
 * @brief This function gets fileInfo_t's name.
 * @param pDirInfo_t [IN] pointer to dirInfo_t.
 * @returns On success - pointer to the name string, on failure - NULL.
**/
char* getDirName(void* pDirInfo_t);

/**
 * @brief This function releases allocated memory of fileInfo_t.
 * @param pFileInfo_t [IN] pointer to fileInfo_t.
 * @returns On success - 0,
**/
int releaseMemoryFile(void* pFileInfo_t);

/**
* @brief This function releases allocated memory of dirInfo_t.
* @param pDirInfo_t [IN] pointer to dirInfo_t.
* @returns On success - 0, on failure -  appropriate listReturnCode_t defined in list.h file.
**/
int releaseMemoryDir(void* pDirInfo_t);

/**
 * @brief This function creates fileInfo_t.
 * It allocates memory for its own usage. When the user finishes using the struct, he MUST call releaseMemoryFile
 * in order to free all allocated memory.
 * @param name [IN] a pointer to a string that contains file's name. MUST be smaller or equal length to MAX_PATH_LENGTH.
 * @param lastChanged [IN] time_t carries the last time that the file changed.
 * @param fileType [IN] listDataHandlersEntryType_t that contains file's type.
 * @returns On success - a valid pointer to fileInfo_t instance, On failure - NULL.
**/
fileInfo_t* createFileInfo_t(char* name, time_t lastChanged, listDataHandlersEntryType_t fileType);

/**
 * @brief This function creates dirInfo_t.
 * It allocates memory for its own usage. When the user finishes using the struct, he MUST call releaseMemoryDir
 * in order to free all allocated memory.
 * @param name [IN] a pointer to a string that contains dir's name. MUST be smaller or equal length to MAX_PATH_LENGTH.
 * @returns On success - a valid pointer to dirInfo_t instance, On failure - NULL.
**/
dirInfo_t* createDirInfo_t(char* name);

/**
 * @brief This function copies a fileInfo_t struct and allocates needed memory.
 * It allocates memory for its own usage. When the user finishes using the struct, he MUST call releaseMemoryFile.
 * If the input pointer is NULL, nothing happens.
 * @param name [IN] a pointer a valid FileInfo_t struct.
 * @returns On success - a valid pointer to a copied fileInfo_t instance, On failure or if given pointer is NULL - NULL.
**/
void* fileInfoCopyFunction(void* pVoidFileInfo);

#endif //MONEYTOR_LISTDATAHANDLERS_H
