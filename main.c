#include "main.h"
static volatile int keepRunning = 1;

int main(int argc, char **argv)
{
    return 0;
}
    /*
    //Variables declaration:
    char *pFolderPath;
    list_t *pNewFileList = initList(), *pDirsList = initList();

    //Attach SIGINT to the termination handler:
    signal(SIGINT, intHandler);

    //Parse arguments form usr and open the folder's path
    if(argc != 2)
    {
        printf("Incompatible number of parameters. Usage: Moneytor <folder_path>\n");
        return -1;
    }
    pFolderPath = argv[1];
    DIR* pDir = opendir(pFolderPath);

    //Open folder and add folder to DirsList:
    if (pDir == NULL)
    {
        printf("Could not open specified directory. Usage: Monytor <dir_path>.\n");
        return -2;
    }
    addElement(pDirsList, pFolderPath, 0, "Directory");

    //Initialize file list for the first time, including all subdir if exists:
    pDirsList->first->pSubDirList = initList();
    getFileList(pDir, pDirsList->first->entryName, pDirsList->first->pSubDirList, pDirsList);
    closedir(pDir);
    node_t *pDirIt = pDirsList->first->next;
    while (pDirIt != NULL)
    {
        pDir = opendir(pDirIt->entryName);
        if ( pDir != NULL)
        {
            //Whoohoo! Directory exists. Scan files and sub directories:
            //Read new file list:
            getFileList(pDir, pDirIt->entryName, pDirIt->pSubDirList, pDirsList);
            //Close directory:
            closedir(pDir);
        }
        else
        {
            printf("On initialization, could not print folder to read. FATAL ERROR!\n");
            return -1;
        }
        pDirIt = pDirIt->next;
    }
    //DBG: printDirTree(pDirsList);

    //Start guarding the folder :)
    while(keepRunning)
    {
        printf("\n====================...Scanning...====================\n");

        //Iterate over all directories in the monitored folder:
        node_t *pDirIt = pDirsList->first;
        while(pDirIt != NULL)
        {
            pDir = opendir(pDirIt->entryName);
            if (pDir != NULL)
            {
                //Whoohoo! Directory exists. Scan files and sub directories:
                //Read new file list:
                getFileList(pDir, pDirIt->entryName, pNewFileList, pDirsList);
                //Close directory:
                closedir(pDir);

                //Print changes:
                findDiffNodes(pDirIt->pSubDirList, pNewFileList, "Deleted", FALSE);
                findDiffNodes(pNewFileList, pDirIt->pSubDirList, "Added", FALSE);
                findDiffNodes(pDirIt->pSubDirList, pNewFileList, "Updated", TRUE);

                //Swap lists:
                deleteList(pDirIt->pSubDirList);
                pDirIt->pSubDirList = pNewFileList;
                pNewFileList = initList();
                pDirIt = pDirIt->next;
            }
            else
            {
                //Directory has been deleted. Remove it from dirList and continue to next Directory
                node_t *tmp = pDirIt->next;
                removeElement(pDirsList, pDirIt);
                pDirIt = tmp;
            }
        }

        //Wait till next iteration:
        delay(SLEEP_TIME_SEC * 1000);
    }

    //Exit properly: close dir, clean memory, etc:
    deleteList(pDirsList);
    deleteList(pNewFileList);
    printf("\n\nThanks for choosing Moneytor! Seeya again soon :)\n\n");
    return 0;
}

// This function makes a delay of ms milliseconds 
void delay(unsigned int ms)
{
    unsigned int clocks = CLOCKS_PER_SEC * ms / 1000;
    clock_t stopTime = clocks + clock();
    while (stopTime > clock());
}

// This function receives pointer to a directory and stores each entry in a string array (future - list) 
static int getFileList(DIR* pDir, char *fullDirPath, list_t* pFileList, list_t *pDirList)
{
    struct stat status;
    struct dirent *pDirent;
    time_t lastChanged;
    char *pFileType, entryName[MAX_PATH_LENGTH] ;
    int i = 0;
    rewinddir(pDir);
    while((pDirent = readdir(pDir)) != NULL)
    {
        //Skip self and father folder:
        if( (strcmp(pDirent->d_name, "..") == 0) || (strcmp(pDirent->d_name, ".") == 0) )
        {
            continue;
        }

        //Get entry's full path:
        strcpy(entryName, fullDirPath);
        strcat(entryName, "/");
        strcat(entryName, pDirent->d_name);

        //Get entry's type and full path:
        pFileType = getEntryType(pDirent, pDirList, entryName);

        //Check entry's last changed time:
        if(stat(entryName, &status) == -1)
        {
            printf("Could not read status of entry: %s. Please check permitions and try again.", pDirent->d_name);
            lastChanged = 0;
        }
        else
        {
            lastChanged = status.st_atim.tv_sec;
        }
        addElement(pFileList, entryName, lastChanged, pFileType);
        ++i;

        //dbg: printf("[%s], %s, last changed in %s\n", pDirent->d_name, pFileType,  asctime(gmtime(&lastChanged)));
    }
    return i;
}

// This function gets two arrays lists of strings and prints all the strings that differ from the first
 * list to the second, with the strToPrint (Added/Deleted) 
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

// This function receives an entry in a folder and returns it's type and full path.
 * If the current entry is a directory, the function searches it in dirList and if it doen't exists, adds it to the list.
static char* getEntryType(struct dirent *pd, list_t *pDirList, char *entryName)
{
    char *pFileType;
    switch (pd->d_type)
    {
        case DT_REG:
            pFileType = "File";
            break;
        case DT_DIR:
            pFileType = "Directory";
            //Check if directory exists in DirsList, and if not - add it!
            node_t *pDirIt = pDirList->first;
            while(pDirIt != NULL)
            {
                if(strcmp(entryName, pDirIt->entryName) == 0)
                {
                    break;
                }
                pDirIt = pDirIt->next;
            }
            //If pDirIt is NULL - you didn't find any directory in DirsList that matches to this dir.
             * Thus, it is a new sub directory. Add it to DirsList!
            if( pDirIt == NULL && (pDirList->counter > 0) )
            {
                addElement(pDirList, entryName, 0, pFileType);
                pDirList->last->pSubDirList = initList();
            }
            break;
        default:
            pFileType = "Unknown";
            break;
    }
    return pFileType;
}

void intHandler(int dummy) { keepRunning = 0; }

// This function prints the entire tree of files and subfolders in dirList 
static void printDirTree(list_t *pDirList)
{
    printf("**********************************************PrintDirTree**********************************************\n\n");
    printf("List of all subDirs:\n");
    printList(pDirList);
    printf("=================================\n\n");
    node_t *pNodeIt = pDirList->first;
    while(pNodeIt != NULL)
    {
        printf("File list in folder: %s:\n", pNodeIt->entryName);
        if(pNodeIt->pSubDirList == NULL)
        {
            printf("SubDirList Uninitialized!!\n");
            continue;
        }
        printList(pNodeIt->pSubDirList);
        printf("-----------------------------------------\n\n");
        pNodeIt = pNodeIt->next;
    }
}

static void convertPathToWindowsFromat(char *linuxFormat)
{
    char tmp[MAX_PATH_LENGTH] = "c:";
    strcpy(tmp + 2, linuxFormat + 11);
    strcpy(linuxFormat, tmp);
}
     */