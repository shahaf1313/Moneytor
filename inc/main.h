#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "inc/utills.h"
 #include "inc/moneytorDefinitions.h"

#define SLEEP_TIME_SEC 10

typedef enum {
    RETURNCODE_MAIN_INVALID_ARGUMENT_NUMBER,
    RETURNCODE_MAIN_MEMORY_ALOOCATION_FAILED,
    RETURNCODE_MAIN_COULDNT_OPEN_GIVEN_FOLDER
} mainReturnCode_t;

// This function catches ctrl + c and causes to program termination
void intHandler(int dummy);

#endif //MONEYTOR_MAIN_H
