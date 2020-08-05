#ifndef MONEYTOR_UTILLS_H
#define MONEYTOR_UTILLS_H

#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "inc/common.h"
#include "inc/listDataHandlers.h"

/**
 * @brief This function delays the program.
 * @param ms [IN] time, in milliseconds, to delay.
**/
void delay(unsigned int ms);

/**
 * @brief This function reads all file in a directory and stores them in fileList. If directories are found, the
 * function adds them (in auddition to fileList) to dirList.
 * @param pDir [IN] a pointer to a directory to read files from.
 * @param currentWorkingDirectory [IN] string path to the cwd.
 * @param fileList [IN] list where files and dirs are added.
 * @param dirList [IN] list where dirs are added.
 * @returns Appropriate returnCode_t.
 */
returnCode_t getFileList(DIR* pDir, char* currentWorkingDirectory, LIST fileList, LIST dirList);

/**
 * @brief This function finds all file entries that differ from original list to the updated list.
 * It prints a message stored in strToPrint near each diff node.
 * It can also check if files were updated using the updateCheck bool.
 * @param original [IN] original list of files to compare from.
 * @param updated [IN] updated list of files to compare to.
 * @param strToPrint [IN] message to print near non-matching files.
 * @param updateCheck [IN] boolean parameter that to set in order to allow update check.
 * @returns Appropriate returnCode_t.
 */
returnCode_t findDiffElements(LIST original, LIST updated, char* strToPrint, int updateCheck);

/**
 * @brief This function prints a directory tree as saved in a directroy list.
 * @param dirList [IN] the directory tree to print.
 * @returns Appropriate returnCode_t.
 */
returnCode_t printDirTree(LIST dirList);

#endif //MONEYTOR_UTILLS_H