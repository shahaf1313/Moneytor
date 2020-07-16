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
    struct dirent *pDirent = NULL;
    char* folderPath  = "C:\\Users\\Shahaf\\Desktop\\MoneyFolder";
    DIR* pDir = opendir(folderPath);
    if (pDir == NULL)
    {
        printf("Could not open specified directory. Usage: Monytor <dir_path>.");
        return -1;
    }

    while(i <= 3)
    {
        printf("Iteration %d:\n", i);
        ++i;
        rewinddir(pDir);
        while((pDirent = readdir(pDir)) != NULL)
        {
            switch (pDirent->d_type)
            {
                case DT_REG:
                    printf("[%s], File\n", pDirent->d_name);
                    break;
                case DT_DIR:
                    printf("[%s], Directory\n", pDirent->d_name);
                    break;
                default:
                    printf("[%s], Unknown\n", pDirent->d_name);
                    break;
            }
        }
        delay(SLEEP_TIME_SEC * 1000);
    }
    closedir(pDir);
    return 0;
}