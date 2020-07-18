#include "main.h"

/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms)
{
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

/* This function receives pointer to a directory and stores each entry in a string array (future - list) */
static int getFileList(DIR* pDir, char **pFileList)
{
    struct dirent *pDirent;
    int i = 0;
    rewinddir(pDir);
    while((pDirent = readdir(pDir)) != NULL)
    {
        strcpy(pFileList[i], pDirent->d_name);
        ++i;
    }
    return i;
}

/* This function gets two arrays (future - lists) of strings and prints all the strings that differ from the first
 * array to the second, with the strToPrint (Added/Deleted) */
static void findDiffStrings(char **firstArr, int n1, char **secondArr, int n2, char *strToPrint)
{
    int foundFile = FALSE, i = 0, j = 0;
    for (i = 0; i < n1; ++i)
    {
        foundFile = FALSE;
        for (j = 0; j < n2; ++j)
        {
            if (strcmp(firstArr[i], secondArr[j]) == 0)
            {
                foundFile = TRUE;
                break;
            }
        }
        if(!foundFile)
        {
            printf("File %s %s!\n", firstArr[i], strToPrint);
        }
    }
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

void copyStringArr(char **dest, char **src, int row)
{
    for(int i = 0; i < row; ++i)
    {
        strcpy(dest[i], src[i]);
    }
}

void zeroStringArray(char **arr, int row, int col)
{
    for(int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            arr[i][j] = '\0';
        }
    }
}

int main()
{
    /*Variables declaration:*/
    unsigned int i = 0, j = 0, currNumOfFiles = 0, newNumOfFiles = 0;
    struct stat status;
    struct dirent *pDirent;
    timestruc_t lastChanged;
    char *pFileType, *pFolderPath  = "C:\\Users\\Shahaf\\Desktop\\MoneyFolder";
    char *pCurrList[MAX_FILES_NUM], *pNewList[MAX_FILES_NUM];
    DIR* pDir = opendir(pFolderPath);

    /*todo: parse arguments form usr and extract the folder's path*/
    /*===========================================================*/

    /*Allocate memory for the 2 static arrays:*/
    for(i=0; i<MAX_FILES_NUM; ++i)
    {
        pCurrList[i] = (char*)malloc(MAX_PATH_LENGTH * sizeof(char));
        pNewList[i] = (char*)malloc(MAX_PATH_LENGTH * sizeof(char));
    }
    zeroStringArray(pCurrList, MAX_FILES_NUM, MAX_PATH_LENGTH);
    zeroStringArray(pNewList, MAX_FILES_NUM, MAX_PATH_LENGTH);

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

    /*Initialize file list:*/
    currNumOfFiles = getFileList(pDir, pCurrList);

    /*Start guarding the folder :)*/
    i = 0;
    while(i <= 3)
    {
        printf("Iteration %d:\n", i);
        ++i;
        rewinddir(pDir);
        newNumOfFiles = getFileList(pDir, pNewList);
        findDiffStrings(pCurrList, currNumOfFiles, pNewList, newNumOfFiles, "Deleted");
        findDiffStrings(pNewList, newNumOfFiles, pCurrList, currNumOfFiles, "Added");
        zeroStringArray(pCurrList, MAX_FILES_NUM, MAX_PATH_LENGTH);
        copyStringArr(pCurrList, pNewList, MAX_FILES_NUM);
        currNumOfFiles = newNumOfFiles;

        /*
        while((pDirent = readdir(pDir)) != NULL)
        {
            Check entry's type:
            pFileType = getEntryType(pDirent);

            Check entry's last changed time:
            if(stat(pDirent->d_name, &status) == -1)
            {
                printf("Could not read status of entry: %s. Please check permitions and try again.", pDirent->d_name);
                break;
            }
            lastChanged = status.st_atim;

            dbg: print entry's full details:
            printf("[%s], %s, last changed in %s\n", pDirent->d_name, pFileType,  asctime(gmtime(&lastChanged.tv_sec)));

        }
        */

        /*Wait till next iteration:*/
        delay(SLEEP_TIME_SEC * 1000);
    }

    /*Exit properly: close dir, clean memory, etc:*/
    for( i=0; i<MAX_FILES_NUM; ++i)
    {
        free(pCurrList[i]);
        free(pNewList[i]);
    }
    closedir(pDir);
    return 0;
}