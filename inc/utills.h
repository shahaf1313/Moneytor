#ifndef MONEYTOR_UTILLS_H
#define MONEYTOR_UTILLS_H

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "inc/common.h"
#include "inc/file_info.h"
#include "inc/dir_info.h"

/**
 * @brief This function delays the program.
 * @param ms [IN] time, in seconds, to delay.
**/
void delay(unsigned int sec);

/**
 * @brief This function finds all file entries that differ from original list to the updated list.
 * It prints a message stored in strToPrint near each diff node.
 * It can also check if files were updated using the updateCheck bool.
 * @param originalList [IN] original list of files to compare from.
 * @param updatedList [IN] updated list of files to compare to.
 * @param strToPrint [IN] message to print near non-matching files.
 * @param shouldCheckUpdate [IN] boolean parameter that to set in order to allow update check.
 * @returns Appropriate returnCode_t.
 */
returnCode_t findDiffElements(LIST originalList, LIST updatedList, const char* strToPrint, bool shouldCheckUpdate);

/**
 * @brief This function prints a directory tree as saved in a directroy list.
 * @param dirList [IN] the directory tree to print.
 * @returns Appropriate returnCode_t.
 */
returnCode_t printDirTree(LIST dirList);


/**
 * @brief This function returns the last changed time_t of an spicified entry.
 * @param fullPath [IN] entry's full path.
 * @param lastChanged [OUT] valid pointer to time_t where last changed time will be stored.
 * @returns Appropriate returnCode_t.
 */
returnCode_t getLastChangedTime(char* fullPath, time_t* lastChanged);

/**
 * @brief This function joins 2 different path to one, with / between them.
 * @param path1 [IN] First part of the path.
 * @param path2 [IN] Second part of the path.
 * @param joined [OUT] The complete (joined) path.
 * @returns Appropriate returnCode_t.
 */
returnCode_t joinPaths(char* path1, char* path2, char joined[MAX_PATH_LENGTH + 1]);

/**
 * @brief This function gets path to an entry and returns it's type (file/dir/unknown) using the
 * listDataHandlersEntryType_t enum.
 * @param pd [IN] entry's Dirent struct (after opened via opendir() and iterated via readdir()).
 * @param pFileType [OUT] pointer to entry's type via listDataHandlersEntryType_t enum. Pointer mustn't be NULL.
 * @returns Appropriate returnCode_t.
 */
returnCode_t getEntryType(struct dirent* pd, fileInfoEntryType_t* pFileType);

/**
 * @brief This function handles encountering a new directory while iterating the current directory.
 * If directory doesn't exists in dirList, the function adds it to dirList.
 * @param newDirFullPath [IN] directory's full path.
 * @param dirList [IN,OUT] directory list to search in.
 * @returns Appropriate returnCode_t.
 */
returnCode_t newDirFoundHandler(char* newDirFullPath, LIST dirList);


#endif //MONEYTOR_UTILLS_H