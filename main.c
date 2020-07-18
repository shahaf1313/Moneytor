#include "main.h"
static volatile int keepRunning = 1;

int main(int argc, char **argv)
{
    /*Variables declaration:*/
    char *pFolderPath;
    list_t *pCurrFileList = initList(), *pNewFileList = initList();

    /*Attach SIGINT to the termination handler:*/
    signal(SIGINT, intHandler);

    /*Parse arguments form usr and open the folder's path*/
    if(argc != 2)
    {
        printf("Incompatible number of parameters. Usage: Moneytor <folder_path>\n");
        return -1;
    }
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);

    /*Open folder and change working dir:*/
    if (pDir == NULL)
    {
        printf("Could not open specified directory. Usage: Monytor <dir_path>.");
        return -2;
    }
    if (chdir(pFolderPath) != 0)
    {
        printf("Could not enter specified directory. Set appropriate permissions and try again.");
        return -3;
    }

    /*Initialize file list for the first time:*/
    getFileList(pDir, pCurrFileList);

    /*Start guarding the folder :)*/
    while(keepRunning)
    {
        printf("\n====================...Scanning...====================\n");

        /*Read new file list:*/
        getFileList(pDir, pNewFileList);

        /*Print changes:*/
        findDiffNodes(pCurrFileList, pNewFileList, "Deleted", FALSE);
        findDiffNodes(pNewFileList, pCurrFileList, "Added", FALSE);
        findDiffNodes(pCurrFileList, pNewFileList, "Updated", TRUE);

        /*Swap lists:*/
        deleteList(pCurrFileList);
        pCurrFileList = pNewFileList;
        pNewFileList = initList();

        /*Wait till next iteration:*/
        delay(SLEEP_TIME_SEC * 1000);
    }

    /*Exit properly: close dir, clean memory, etc:*/
    deleteList(pCurrFileList);
    deleteList(pNewFileList);
    closedir(pDir);
    printf("\n\nThanks for choosing Moneytor! Seeya again soon :)");
    return 0;
}

/* This function makes a delay of ms milliseconds */
void delay(unsigned int ms)
{
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

/* This function receives pointer to a directory and stores each entry in a string array (future - list) */
static int getFileList(DIR* pDir, list_t* pFileList)
{
    struct stat status;
    struct dirent *pDirent;
    time_t lastChanged;
    char *pFileType;
    int i = 0;
    rewinddir(pDir);
    while((pDirent = readdir(pDir)) != NULL)
    {
        /*Check entry's type:*/
        pFileType = getEntryType(pDirent);

        /*Check entry's last changed time:*/
        if(stat(pDirent->d_name, &status) == -1)
        {
            printf("Could not read status of entry: %s. Please check permitions and try again.", pDirent->d_name);
            lastChanged = 0;
        }
        else
        {
            lastChanged = status.st_atim.tv_sec;
        }
        addElement(pFileList, pDirent->d_name, lastChanged, pFileType);
        ++i;

        /*dbg: printf("[%s], %s, last changed in %s\n", pDirent->d_name, pFileType,  asctime(gmtime(&lastChanged)));*/
    }
    return i;
}

/* This function gets two arrays lists of strings and prints all the strings that differ from the first
 * list to the second, with the strToPrint (Added/Deleted) */
static void findDiffNodes(list_t *original, list_t *updated, char *strToPrint, int updateCheck)
{
    int foundFile;
    node_t *pOriginalNode = original->first, *pUpdatedNode;
    while(pOriginalNode != NULL)
    {
        foundFile = FALSE;
        pUpdatedNode = updated->first;

        while(pUpdatedNode != NULL)
        {
            if (strcmp(pOriginalNode->entryName, pUpdatedNode->entryName) == 0)
            {
                foundFile = TRUE;
                break;
            }
            pUpdatedNode = pUpdatedNode->next;
        }

        if(!updateCheck && !foundFile)
        {
            printf("File %s %s!\n", pOriginalNode->entryName, strToPrint);
        }
        else if(updateCheck && foundFile)
        {
            if(pOriginalNode->lastChanged - pUpdatedNode->lastChanged != 0)
                printf("File %s %s!\n", pOriginalNode->entryName, strToPrint);
        }

        pOriginalNode = pOriginalNode->next;
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

void intHandler(int dummy) { keepRunning = 0; }