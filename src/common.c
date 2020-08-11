#include <inc/common.h>

returnCode_t findRelativePath(const char* fullPath, const char* folderName, char relativePath[MAX_PATH_LENGTH + 1]) {
    char* rest = NULL;
    char* delimiter = "/";
    char internalFullPath[MAX_PATH_LENGTH+1];
    char internalFolderName[MAX_PATH_LENGTH+1];

    if ((strlen(fullPath) > MAX_PATH_LENGTH + 1) || (strlen(folderName) > MAX_PATH_LENGTH + 1)) {
        return RETURNCODE_COMMON_FINDRELATIVEPATH_STRING_EXCEEDS_MAX_PATH_LENGTH;
    }
    strcpy(internalFolderName, folderName);
    strcpy(internalFullPath, fullPath);

    char* token = strtok_r(internalFullPath, delimiter, &rest);
    while (token != NULL) {
        if (0 == strcmp(token, internalFolderName)) {
            strcpy(relativePath, rest);
            break;
        }
        token = strtok_r(NULL, delimiter, &rest);
    }

    if (NULL == token) {
        return RETURNCODE_COMMON_FINDRELATIVEPATH_RELATIVE_PATH_NOT_FOUND;
    }
    return RETURNCODE_SUCCESS;
}
