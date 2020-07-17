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
    struct stat status;
    struct dirent *pDirent = NULL;
    timestruc_t lastChanged;
    char path[MAX_PATH_LENGTH], *pFileType, *pFolderPath  = "C:\\Users\\Shahaf\\Desktop\\MoneyFolder";
    DIR* pDir = opendir(pFolderPath);
    if (pDir == NULL)
    {
        printf("Could not open specified directory. Usage: Monytor <dir_path>.");
        return -1;
    }
    if (chdir(pFolderPath) != 0)
    {
        printf("Could not enter specified directory. Set appropriate permissions and try again.");
        return -2;
    }

    while(i <= 3)
    {
        printf("Iteration %d:\n", i);
        ++i;
        rewinddir(pDir);
        while((pDirent = readdir(pDir)) != NULL)
        {
            if(stat(pDirent->d_name, &status) == -1)
            {
                printf("Could not read status of entry: %s. Please check permitions and try again.", pDirent->d_name);
                break;
            }
            lastChanged = status.st_atim;
            switch (pDirent->d_type)
            {
                case DT_REG:
                    pFileType = "File";
                    break;
                case DT_DIR:
                    pFileType = "Directory";
                    break;
                default:
                    pFileType = "Unknown";
                    break;
            }
            printf("[%s], %s, last changed in %s\n", pDirent->d_name, pFileType,  asctime(gmtime(&lastChanged.tv_sec)));
        }
        delay(SLEEP_TIME_SEC * 1000);
    }
    closedir(pDir);
    return 0;
}