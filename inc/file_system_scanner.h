#ifndef MONEYTOR_FILE_SYSTEM_SCANNER_H
#define MONEYTOR_FILE_SYSTEM_SCANNER_H

#include "inc/common.h"
#include "inc/utills.h"

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

#endif //MONEYTOR_FILE_SYSTEM_SCANNER_H
