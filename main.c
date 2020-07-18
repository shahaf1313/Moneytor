#include "main.h"
static void listGames(void);

int main(int argc, char **argv)
{
    /*Variables declaration:*/
    char *pFolderPath;
    list_t *pCurrFileList = initList(), *pNewFileList = initList();

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
    int i = 0;
    while(i < 10)
    {
        printf("\n====================Iteration %d:====================\n", i);
        ++i;

        /*Read new file list:*/
        getFileList(pDir, pNewFileList);

        /*dbg: print both lists:
        printf("current list print:\n");
        printList(pCurrFileList);
        printf("new list print:\n");
        printList(pNewFileList);
        printf("\n");*/

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

void listGames(void)
{
    list_t *l1 = initList(), *l2;
    printf("printing an empty list:\n");
    printList(l1);

    addElement(l1, "hakitsy", 5, "Dir");
    addElement(l1, "mitsy", 6, "File");
    addElement(l1, "pitsy", 8, "Dir");
    addElement(l1, "bitsy", 17,"File");

    printf("printing a list with 3 elements:\n");
    printList(l1);

    printf("printing a copied list:\n");
    l2 = copyList(l1);
    printList(l2);

    removeElement(l1, l1->first);
    printf("printing list after removing first element:\n");
    printList(l1);

    removeElement(l1, l1->first->next);
    printf("printing list after removing middle element:\n");
    printList(l1);

    removeElement(l1, l1->last);
    printf("printing list after removing last element:\n");
    printList(l1);

    removeElement(l1, l1->last);
    printf("printing list after removing only element:\n");
    printList(l1);

    printf("printing l2 before delete:\n");
    printList(l2);
    deleteList(l1);
    deleteList(l2);
    printf("deleted successfully l1 and l2!\n");
}