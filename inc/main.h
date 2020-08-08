#ifndef MONEYTOR_MAIN_H
#define MONEYTOR_MAIN_H

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "inc/utills.h"
#include "inc/common.h"

#define SLEEP_TIME_SEC 10

// CR: (DC) Why is this function in the header file? It's used only internally in main.c
// This function catches ctrl + c and causes to program termination
void intHandler(int dummy);

#endif //MONEYTOR_MAIN_H
