#ifndef MONEYTOR_COMMON_H
#define MONEYTOR_COMMON_H

#include <stdbool.h>

#define MAX_PATH_LENGTH 255

#ifndef NDEBUG
// CR: (DC) I want you to add more information to your DEBUG_PRINT macro. Consider the following example, where
// CR: (DC) I use your DEBUG_PRINT macro:
// CR: (DC)     DEBUG_PRINT("malloc failed with value %d", return_code);
// CR: (DC) I want you to change your DEBUG_PRINT macro, so that alongside the "malloc failed" message, it will
// CR: (DC) also print the file name in which the macro was called (list.c, for example) and the line number
// CR: (DC) that called the macro (181, for example).
// CR: (DC) An example debug print that will be printed due to the above example:
// CR: (DC)     list.c:181: malloc failed with value 2
// CR: (DC) To implement it, search and read about predefined preprocessor macros and string
// CR: (DC) concatenation in macros.
    #define DEBUG_PRINT(...) do {printf(__VA_ARGS__); printf("\n");} while(0)
#else
    #define DEBUG_PRINT(...) do {} while(0)
#endif

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
    RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND,
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
    RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER,
    RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED,
    RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER,
    RETURNCODE_MAIN_FOUND_NULL_DIRECTORY,
    RETURNCODE_MAIN_COULD_NOT_READ_FILE_LIST,
    RETURNCODE_MAIN_POINTER_TO_DIRLIST_IS_NULL,
    RETURNCODE_MAIN_FOUND_NULL_POINTER,
    RETURNCODE_MAIN_UNINITIALIZED
} returnCode_t;

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
