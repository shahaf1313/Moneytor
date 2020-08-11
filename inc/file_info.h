#ifndef MONEYTOR_FILE_INFO_H
#define MONEYTOR_FILE_INFO_H

#include <stdlib.h>
#include <time.h>
#include "inc/common.h"
#include "inc/list.h"

typedef enum {
    FILE_INFO_ENTRY_TYPE_FILE = 1,
    FILE_INFO_ENTRY_TYPE_DIRECTORY,
    FILE_INFO_ENTRY_TYPE_UNKNOWN
} fileInfoEntryType_t;

typedef struct fileInfo {
    char fileName[MAX_PATH_LENGTH];
    time_t lastChanged;
    fileInfoEntryType_t fileType;
} fileInfo_t;

/**
 * @brief This function gets fileInfo_t's name.
 * @param pFileInfo_t [IN] pointer to fileInfo_t.
 * @returns On success - pointer to the name string, on failure - NULL.
**/
char* getFileName(fileInfo_t* pFileInfo_t);

/**
 * @brief This function releases allocated memory of fileInfo_t.
 * @param pFileInfo_t [IN] pointer to fileInfo_t.
 * @returns On success - 0,
**/
int releaseMemoryFile(fileInfo_t* pFileInfo_t);

/**
 * @brief This function creates fileInfo_t.
 * It allocates memory for its own usage. When the user finishes using the struct, he MUST call releaseMemoryFile
 * in order to free all allocated memory.
 * @param name [IN] a pointer to a string that contains file's name. MUST be smaller or equal length to MAX_PATH_LENGTH.
 * @param lastChanged [IN] time_t carries the last time that the file changed.
 * @param fileType [IN] listDataHandlersEntryType_t that contains file's type.
 * @returns On success - a valid pointer to fileInfo_t instance, On failure - NULL.
**/
fileInfo_t* createFileInfo_t(char* name, time_t lastChanged, fileInfoEntryType_t fileType);

/**
 * @brief This function copies a fileInfo_t struct and allocates needed memory.
 * It allocates memory for its own usage. When the user finishes using the struct, he MUST call releaseMemoryFile.
 * If the input pointer is NULL, nothing happens.
 * @param name [IN] a pointer a valid FileInfo_t struct.
 * @returns On success - a valid pointer to a copied fileInfo_t instance, On failure or if given pointer is NULL - NULL.
**/
fileInfo_t* fileInfoCopyFunction(fileInfo_t* pfileInfo);

/**
 * @brief This function compares between two fileInfo_t and decides whether they are equal or not, by checking if their name matches.
 * @param pFile1 [IN] Pointer to the first fileInfo_t.
 * @param pFile2 [IN] Pointer to the second fileInfo_t.
 * @returns Integer that indicates whether files has the same name. 0 is returned if true. Different integer returned otherwise.
 */
int fileInfoCompareFunction(fileInfo_t* pFile1, fileInfo_t* pFile2);

#endif //MONEYTOR_FILE_INFO_H
