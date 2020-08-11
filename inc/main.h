#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "inc/utills.h"
#include "inc/file_system_scanner.h"
#include "inc/common.h"

#define SLEEP_TIME_SEC 3

#define CHECK_RETURN_CODE_MAIN(returnCode)                      \
        do {                                                    \
            if (RETURNCODE_SUCCESS != (returnCode)) {           \
                returnCodeMain = (returnCode);                  \
                goto exit;                                      \
            }                                                   \
        }                                                       \
        while(0)                                                \

#define CHECK_NULL_POINTER_MAIN(p)                                      \
        do {                                                            \
            if (NULL == (p)) {                                          \
                returnCodeMain = RETURNCODE_MAIN_FOUND_NULL_POINTER;    \
                goto exit;                                              \
            }                                                           \
        }                                                               \
        while(0)                                                        \


#endif //MONEYTOR_MAIN_H
