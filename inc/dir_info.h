#ifndef MONEYTOR_DIR_INFO_H
#define MONEYTOR_DIR_INFO_H

#include <stdlib.h>
#include "inc/common.h"
#include "inc/list.h"

typedef struct dirInfo {
    char dirName[MAX_PATH_LENGTH];
    LIST filesList;
} dirInfo_t;

/**
 * @brief This function gets fileInfo_t's name.
 * @param pDirInfo_t [IN] pointer to dirInfo_t.
 * @returns On success - pointer to the name string, on failure - NULL.
**/
char* getDirName(dirInfo_t* pDirInfo_t);

/**
* @brief This function releases allocated memory of dirInfo_t.
* @param pDirInfo_t [IN] pointer to dirInfo_t.
* @returns On success - 0, on failure -  appropriate listReturnCode_t defined in list.h file.
**/
int releaseMemoryDir(dirInfo_t* pDirInfo_t);

/**
 * @brief This function creates dirInfo_t.
 * It allocates memory for its own usage. When the user finishes using the struct, he MUST call releaseMemoryDir
 * in order to free all allocated memory.
 * @param name [IN] a pointer to a string that contains dir's name. MUST be smaller or equal length to MAX_PATH_LENGTH.
 * @returns On success - a valid pointer to dirInfo_t instance, On failure - NULL.
**/
dirInfo_t* createDirInfo_t(char* name);

/**
 * @brief This function compares between two dirInfo_t and decides whether they are equal or not, by checking if their name matches.
 * @param pDir1 [IN] Pointer to the first dirInfo_t.
 * @param pDir2 [IN] Pointer to the second dirInfo_t.
 * @returns Integer that indicates whether dirs has the same name. 0 is returned if true. Different integer returned otherwise.
 */
int dirInfoCompareFunction(dirInfo_t* pDir1, dirInfo_t* pDir2);

#endif //MONEYTOR_DIR_INFO_H
