#ifndef MONEYTOR_COMMON_H
#define MONEYTOR_COMMON_H

#include <stdbool.h>

// CR: (DC) Notice that whenever you use DEBUG_PRINT (for example, list.c line 181), you put a semicolon
// CR: (DC) after the call to DEBUG_PRINT, but CLion issues a warning because the semicolon is not required
// CR: (DC) Note that it's not required because DEBUG_PRINT expands to a block { ... }, which doesn't need
// CR: (DC) a semicolon afterwards.
// CR: (DC) We want to put a semicolon after DEBUG_PRINT, we want CLion not to issue a warning about it,
// CR: (DC) and we want to enforce that the user puts a semicolon after DEBUG_PRINT
// CR: (DC) For that, wrap the DEBUG_PRINT in do { ... } while (0)
// CR: (DC) Pay attention NOT to put a semicolon after the while (0) in the macro definition, so that the
// CR: (DC) user of DEBUG_PRINT will have to supply that semicolon
// CR: (DC) Also, pay attention to the Release version of DEBUG_PRINT (make sure it will also require
// CR: (DC) a semicolon).
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
    #define DEBUG_PRINT(...) {printf(__VA_ARGS__); printf("\n");}
#else
    #define DEBUG_PRINT(...) {}
#endif

typedef enum {
    RETURNCODE_SUCCESS = 0,
    RETURNCODE_LIST_GETLENGTH_LIST_IS_NULL,
    RETURNCODE_LIST_REMOVEELEMENT_LIST_IS_NULL,
    RETURNCODE_LIST_REMOVEELEMENT_LIST_EMPTY,
    RETURNCODE_LIST_REMOVEELEMENT_ELEMENT_NOT_FOUND,
    RETURNCODE_LIST_REMOVEELEMENT_FAILED_TO_FREE_DATA_MEMORY,
    RETURNCODE_LIST_ADDELEMENT_LIST_NULL,
    RETURNCODE_LIST_ADDELEMENT_MEMORY_ALLOCATION_FAILED,
    RETURNCODE_LIST_DESTROY_FAILED_TO_FREE_DATA_MEMORY,
    RETURNCODE_LIST_PRINT_LIST_IS_NULL,
    RETURNCODE_LIST_PRINT_NOT_ALL_ELEMENTS_PRINTED,
    RETURNCODE_LIST_GETNEXT_LIST_NULL,
    RETURNCODE_LIST_GETNEXT_LAST_ELEMENT,
    RETURNCODE_LIST_GETNEXT_CURRENT_ELEMENT_NOT_FOUND,
    RETURNCODE_LIST_COPY_PARAMETER_NULL,
    RETURNCODE_LIST_COPY_FAILED_TO_ADD_ELEMENT,
    RETURNCODE_LIST_COPY_FAILED_CREATE_NEW_LIST,
    RETURNCODE_LIST_COPY_FAILED_TO_COPY_ELEMENT,
    RETURNCODE_LIST_UTILLS_GETENTRYLASTCHANGED_COULD_NOT_GET_FILE_STATUS,
    RETURNCODE_LIST_UTILLS_GETENTRYLASTCHANGED_OUTPUT_POINTER_NULL,
    RETURNCODE_LIST_UTILLS_NEWDIRFOUNDHANDLER_PARAMETER_NULL,
    RETURNCODE_LIST_UTILLS_NEWDIRFOUNDHANDLER_COULD_NOT_ALLOCATE_MEMORY,
    // CR: (DC) Status is unused
    RETURNCODE_LIST_UTILLS_NEWDIRFOUNDHANDLER_COULD_NOT_ADD_NEW_DIR_TO_DIRLIST,
    RETURNCODE_LIST_UTILLS_GETENTRYFULLPATH_PATH_LENGTH_EXCEEDS_MAX_PATH_LENGTH,
    RETURNCODE_LIST_UTILLS_GETENTRYFULLPATH_OUTPUT_POINTER_NULL,
    RETURNCODE_LIST_UTILLS_GETFILELIST_PARAMETER_NULL,
    RETURNCODE_LIST_UTILLS_GETFILELIST_COULD_NOT_ALLOCATE_MEMORY,
    RETURNCODE_LIST_UTILLS_FINDDIFFELEMENTS_PARAMETER_NULL,
    RETURNCODE_LIST_UTILLS_PRINTDIRTREE_PRINT_FAILED,
    RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER,
    RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED,
    RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER,
    RETURNCODE_MAIN_UNINITIALIZED
} returnCode_t;

// CR: (DC) Remove the semicolon after the while(0), so that the user of FREE() will have to put semicolon
// CR: (DC) Moreover, in a macro, each macro parameter should be wrapped in ()
// CR: (DC) So wrap p in () everywhere it is used inside the macro
// CR: (DC) ALSO wrap it inside the call to free: free((p)). One set of () is for the function call, one
// CR: (DC) set of () is for wrapping the parameter
// CR: (DC) Wrapping macro parameters in () prevents accidental macro misuse that can happen.
#define FREE(p)                 \
        do {                    \
            if (NULL != p) {    \
                free(p);        \
            }                   \
            p = NULL;           \
        }                       \
        while(0);               \

#define CHECK_RETURN_CODE(returnCode)                           \
        do {                                                    \
            if (RETURNCODE_SUCCESS != returnCode) {             \
            return returnCode;                                  \
            }                                                   \
        }                                                       \
        while(0);                                               \



#endif //MONEYTOR_COMMON_H
