#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 100

typedef struct
{
    char fname[3];
    int strat;
    int blocksize;
} Disk;

void initializeDisk(Disk *disk)
{
    for (int i = 0; i < DISK_SIZE; i++)
    {
        strcpy(disk[i].fname, "");
    }
}




void store(Disk *disk, char *name, int start, int blocksize)
{
    int n;
    if (start != 0)
    {
        n = blocksize + start;
    }
    else
    {
        n = blocksize + start + 1;
    }
    for (int i = start; i < n; i++)
    {
        strcpy(disk[i].fname, name);
        disk[i].strat = start;
        if (start != 0)
        {
            disk[i].blocksize = blocksize;
        }
    }
    printf("File is stored starting from %d th block\n", start);
}

int checkFileName(Disk *disk, char *name)
{
    if (name == NULL)
    {
        printf("ERROR: Invalid name\n");
        return 0;
    }

    if (name[0] == '\0')
    {
        printf("ERROR: This name is empty\n");
        return 0;
    }

    for (int i = 0; i < DISK_SIZE; i++)
    {
        int j = 0;
        while (name[j] != '\0' && disk[i].fname[j] != '\0' && name[j] == disk[i].fname[j])
        {
            j++;
        }
        if (name[j] != disk[i].fname[j])
        {
            break;
        }
        else if (name[j] == '\0')
        {
            printf("ERROR: This name is already taken\n");
            return 0;
        }
    }

    printf("Name has been set successfully\n");
    return 1;
}

int checkBlocksize(Disk *disk, int blocksize, char *name)
{
    int start = 0;
    int count = 0;
    int free = 0;
    if (blocksize <= 0)
    {
        printf("ERROR: Invalid block size\n");
        return 0;
    }
    for (int i = 0; i < DISK_SIZE; i++)
    {
        if (disk[i].fname[0] == '\0')
        {
            free++;
            count++;
        }
        else
        {
            count = 0;
            start = i;
        }
        if (count == blocksize)
        {
            store(disk, name, start, blocksize);
            return 1;
        }
    }
    if (free >= blocksize)
    {
        printf("file can't be stored however you can use defragmentation\n");
        return 0;
    }
    if (free < blocksize)
    {
        printf("ERROR: Insufficient storage\n");
        return 0;
    }
}

void addFile(Disk *disk)
{
    char name[3];
    int blocksize;

    int v;
    do
    {
        printf("enter file name(2 characters)\n");
        scanf("%s", name);
        name[3] = '\0';

    } while (checkFileName(disk, name) == 0);
    do
    {
        printf("enter the size of your file in blocks\n");
        scanf("%d", &blocksize);
    } while (checkBlocksize(disk, blocksize, name) == 0);
}

void deleteFile(Disk *disk)
{
    char name[3];
    int i, j, n;

    printf("Enter the name of the file to delete: ");
    scanf("%s", name);

    for (i = 0; i < DISK_SIZE; i++)
    {
        if (strcmp(disk[i].fname, name) == 0)
        {

            n = disk[i].blocksize;

            for (j = i; j < i + n; j++)
            {
                disk[j].blocksize = 0;
                strcpy(disk[j].fname, "");
                disk[j].strat = -1;
            }

            i += n - 1;
        }
    }

    printf("Files with name '%s' deleted successfully.\n", name);
}

void displayDisk(Disk *disk)
{

    for (int i = 0; i < DISK_SIZE; i++)
    {

        if (i % 10 == 0)
        {
            printf("\n");
        }
        if (strcmp(disk[i].fname, "") == 0)
        {
            printf("## | ");
        }
        else
        {
            printf("%-2s | ", disk[i].fname);
        }
    }
    printf("\n");
}

void defragmentDisk(Disk *disk)
{
    Disk newDisk[DISK_SIZE];
    int i, j;

    for (i = 0, j = 0; i < DISK_SIZE; i++)
    {
        if (disk[i].fname[0] != '\0')
        {
            newDisk[j++] = disk[i];
        }
    }

    for (i = 0; i < j; i++)
    {
        disk[i] = newDisk[i];
    }

    for (i = j; i < DISK_SIZE; i++)
    {
        strcpy(disk[i].fname, "\0");
    }
}

int main()
{
    Disk disk[DISK_SIZE];
    int choice;

    initializeDisk(disk);

    do
    {
        printf("\n1. Add File\n2. Delete File\n3. Display Disk\n4. Defragment Disk\n5. Read file\n0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addFile(disk);
            break;
        case 2:
            deleteFile(disk);
            break;
        case 3:
            displayDisk(disk);
            break;
        case 4:
            defragmentDisk(disk);
            printf("Disk defragmented.\n");
            break;
        case 0:
            printf("Exiting");
            break;
        default:
            printf("error: option invalide\n");
        }

    } while (choice != 0);

    return 0;
}