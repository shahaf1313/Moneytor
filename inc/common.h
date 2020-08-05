#ifndef MONEYTOR_COMMON_H
#define MONEYTOR_COMMON_H

// CR: (DC) DEBUG_PRINT is a macro that prints a message only in Debug mode.
// CR: (DC) In the field (in production) we don't want these messages to be printed. We don't even want
// CR: (DC) the strings embedded in the messages do be compiled into the binary.
// CR: (DC) As it is now, DEBUG_PRINT will print the message both in Debug and in Release modes.
// CR: (DC) I want you to change DEBUG_PRINT to print the message only in Debug mode.
// CR: (DC) In Release mode, the macro should be simply resolved to nothing (an empty statement).
// CR: (DC) To implement this, you can utilize the NDEBUG preprocessor definition
// CR: (DC) Read about it here: https://stackoverflow.com/questions/2290509/debug-vs-ndebug (2nd answer)
// CR: (DC) NDEBUG is defined in the C standard, and the compiler must define it when compiling
// CR: (DC) in **Release** mode. Note that NDEBUG stands for "Not Debug" (a.k.a, Release).
// CR: (DC) You can test whether NDEBUG is defined or not using #ifdef NDEBUG or #ifndef NDEBUG
#define DEBUG_PRINT(...) {printf(__VA_ARGS__); printf("\n");}
// CR: (DC) Not needed. There's <stdbool.h> that defines the symbols "true" and "false"
#define TRUE 1
#define FALSE 0

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
    RETURNCODE_LIST_COPY_FAILED_CREATE_NEW_LIST,
    RETURNCODE_LIST_COPY_FAILED_TO_ADD_ELEMENT,
    RETURNCODE_LIST_COPY_FAILED_TO_COPY_ELEMENT,
    RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER,
    RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED,
    RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER,
    RETURNCODE_MAIN_UNINITIALIZED
} returnCode_t;

#endif //MONEYTOR_COMMON_H
