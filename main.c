#include "main.h"

/* This function makes a delay of ms milliseconds */
void delay(int ms)
{
    int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

int main() {
    int i = 0;
    while(i <= 3)
    {
        printf("Iteration %d\n", i);
        ++i;
        delay(SLEEP_TIME_SEC * 1000);
    }
    return 0;
}