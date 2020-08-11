#ifndef MONEYTOR_COMMON_H
#define MONEYTOR_COMMON_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH_LENGTH 255
#define MONEYTOR_REPO_NAME "Moneytor"

typedef enum {
    RETURNCODE_SUCCESS = 0,
    RETURNCODE_LIST_GETLENGTH_LIST_OR_LENGTH_POINTER_ARE_NULL,
    RETURNCODE_LIST_REMOVEELEMENT_LIST_IS_NULL,
    RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY,
    RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND,
    RETURNCODE_LIST_REMOVEELEMENT_FAILED_TO_FREE_DATA_MEMORY,
    RETURNCODE_LIST_ADDELEMENT_LIST_NULL,
    RETURNCODE_LIST_ADDELEMENT_MEMORY_ALLOCATION_FAILED,
    RETURNCODE_LIST_DESTROY_FAILED_TO_FREE_DATA_MEMORY,
    RETURNCODE_LIST_FIND_PARAMETERS_NULL,
    RETURNCODE_LIST_PRINT_LIST_IS_NULL,
    RETURNCODE_LIST_PRINT_NOT_ALL_ELEMENTS_PRINTED,
    RETURNCODE_LIST_GETNEXT_LIST_OR_NODE_NULL,
    RETURNCODE_LIST_GETNEXT_LAST_ELEMENT,
    RETURNCODE_LIST_GETPREV_LIST_OR_NODE_NULL,
    RETURNCODE_LIST_GETPREV_FIRST_ELEMENT,
    RETURNCODE_LIST_COPY_PARAMETER_NULL,
    RETURNCODE_LIST_COPY_FAILED_TO_ADD_ELEMENT,
    RETURNCODE_LIST_COPY_FAILED_CREATE_NEW_LIST,
    RETURNCODE_LIST_COPY_FAILED_TO_COPY_ELEMENT,
    RETURNCODE_UTILLS_GETENTRYLASTCHANGED_COULD_NOT_GET_FILE_STATUS,
    RETURNCODE_UTILLS_GETENTRYLASTCHANGED_OUTPUT_POINTER_NULL,
    RETURNCODE_UTILLS_NEWDIRFOUNDHANDLER_PARAMETER_NULL,
    RETURNCODE_UTILLS_NEWDIRFOUNDHANDLER_COULD_NOT_ALLOCATE_MEMORY,
    RETURNCODE_UTILLS_JOINPATHS_PATH_LENGTH_EXCEEDS_MAX_PATH_LENGTH,
    RETURNCODE_UTILLS_JOINPATHS_OUTPUT_POINTER_NULL,
    RETURNCODE_UTILLS_GETFILELIST_PARAMETER_NULL,
    RETURNCODE_UTILLS_GETFILELIST_COULD_NOT_ALLOCATE_MEMORY,
    RETURNCODE_UTILLS_FINDDIFFELEMENTS_PARAMETER_NULL,
    RETURNCODE_UTILLS_PRINTDIRTREE_PRINT_FAILED,
    RETURNCODE_COMMON_FINDRELATIVEPATH_RELATIVE_PATH_NOT_FOUND,
    RETURNCODE_COMMON_FINDRELATIVEPATH_STRING_EXCEEDS_MAX_PATH_LENGTH,
    RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER,
    RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED,
    RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER,
    RETURNCODE_MAIN_FOUND_NULL_DIRECTORY,
    RETURNCODE_MAIN_COULD_NOT_READ_FILE_LIST,
    RETURNCODE_MAIN_POINTER_TO_DIRLIST_IS_NULL,
    RETURNCODE_MAIN_FOUND_NULL_POINTER,
    RETURNCODE_MAIN_FOLDER_TO_GUARD_DELETED,
    RETURNCODE_MAIN_UNINITIALIZED
} returnCode_t;

// CR: (SE) This function belongs to utiils.h but it causes cyclic declaration (utill.h includes common.h and vise versa).
// CR: (SE) I don't have better idea but to add common.c with this function. Do you?
/**
 * @brief This function gets full path and name of a folder in this path. It returns a string contains only the relative path to this folder.
 * The function assumes that the path contains only / as separators between folder and that there are no leading or trilling spaces in the full path.
 * @param fullPath [IN] The full path to extract relative path from.
 * @param folderName [IN] Folder name to relate.
 * @param relativePath [OUT] String contains relative path.
 * @returns Appropriate returnCode_t.
 */
returnCode_t findRelativePath(const char* fullPath, const char* folderName, char relativePath[MAX_PATH_LENGTH+1]);

#ifndef NDEBUG
#define DEBUG_PRINT(...)                                                                                 \
                do {                                                                                     \
                    char relativePath[MAX_PATH_LENGTH+1] = "\0";                                         \
                    findRelativePath(__FILE__, MONEYTOR_REPO_NAME, relativePath);                        \
                    printf("%s: line %d: ", relativePath, __LINE__);                                     \
                    printf(__VA_ARGS__);                                                                 \
                    printf("\n");                                                                        \
                }                                                                                        \
                while(0)
#else
#define DEBUG_PRINT(...) do {} while(0)
#endif

#define FREE(p)                 \
        do {                    \
            if (NULL != (p)) {  \
                free((p));      \
            }                   \
            (p) = NULL;         \
        }                       \
        while(0)                \

#define CHECK_RETURN_CODE(returnCode)                           \
        do {                                                    \
            if (RETURNCODE_SUCCESS != (returnCode)) {           \
            return (returnCode);                                \
            }                                                   \
        }                                                       \
        while(0)                                                \

#endif //MONEYTOR_COMMON_H
