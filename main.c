#include "main.h"

/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms)
{
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

/* This function receives an entry in a folder and returns it's type */
static char* getEntryType(struct dirent *pd)
{
    char *pFileType;
    switch (pd->d_type)
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
    return pFileType;
}

int main()
{
    /*Variables declaration:*/
    unsigned int i = 0;
    struct stat status;
    struct dirent *pDirent = NULL;
    timestruc_t lastChanged;
    char path[MAX_PATH_LENGTH], *pFileType, *pFolderPath  = "C:\\Users\\Shahaf\\Desktop\\MoneyFolder";
    DIR* pDir = opendir(pFolderPath);

    /*todo: parse arguments form usr and extract the folder's path*/
    /*===========================================================*/

    /*Open folder and change working dir:*/
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

    /*Start guarding the folder :)*/
    while(i <= 3)
    {
        printf("Iteration %d:\n", i);
        ++i;
        rewinddir(pDir);
        while((pDirent = readdir(pDir)) != NULL)
        {
            /*Check entry's type:*/
            pFileType = getEntryType(pDirent);

            /*Check entry's last changed time:*/
            if(stat(pDirent->d_name, &status) == -1)
            {
                printf("Could not read status of entry: %s. Please check permitions and try again.", pDirent->d_name);
                break;
            }
            lastChanged = status.st_atim;

            /*dbg: print entry's full details:*/
            printf("[%s], %s, last changed in %s\n", pDirent->d_name, pFileType,  asctime(gmtime(&lastChanged.tv_sec)));
        }

        /*Wait till next iteration:*/
        delay(SLEEP_TIME_SEC * 1000);
    }

    /*Exit properly: close dir, clean memory, etc:*/
    closedir(pDir);
    return 0;
}