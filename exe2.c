#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 100

typedef struct
{
    char fname[3];
    int start;
    int nextblock;
} Disk;

void initializeDisk(Disk *disk)
{
    for (int i = 0; i < DISK_SIZE; i++)
    {
        strcpy(disk[i].fname, "");
        disk[i].start = -1;
        disk[i].nextblock=0;
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
        disk[i].start = start;
       
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
    int i, j, count = 0;

    for (i = 0; i < DISK_SIZE; i++)
    {
        if (disk[i].start == -1)
        {
            break;
        }
    }

    for (j = i; j < DISK_SIZE && count < blocksize; j++)
    {
        if (disk[j].start != -1)
        {
            break;
        }
        count++;
    }

    if (count >= blocksize)
    {

        strcpy(disk[i].fname, name);
        disk[i].start = i;
       

        for (j = i + 1; j < i + blocksize; j++)
        {
            disk[j - 1].nextblock = j;
            strcpy(disk[j].fname, name);
            disk[j].start = i;
            
        }

        disk[j - 1].nextblock = -1;
       printf("file is stored starting from %d\n",disk[j-1].start);
        return 1;
    }
    else
    {
        printf("Insufficient free space.\n");
        return 0;
    }
}

void addFile(Disk *disk)
{
    char name[3];
    int blocksize;

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



void deleteFile(Disk *disk) {
    char *name = (char *)malloc(3);

    if (name == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter the name of the file to delete: ");
    scanf("%s", name);

    int i;
    for (i = 0; i < DISK_SIZE; i++) {
        if (strcmp(disk[i].fname, name) == 0) {
            break;
        }
    }

    if (i < DISK_SIZE) {
        int current_block = i;
        while (disk[current_block].nextblock != -1) {
            int next_block = disk[current_block].nextblock;
           
            strcpy(disk[current_block].fname, "");
            disk[current_block].start = -1;
            
            disk[current_block].nextblock = -1;
           
            current_block = next_block;
        }
        strcpy(disk[current_block].fname, "");
        disk[current_block].start = -1;
       
        disk[current_block].nextblock = -1;

        printf("File '%s' deleted successfully.\n", name);
    } else {
        printf("File '%s' not found.\n", name);
    }

    free(name);
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

int main()
{
    Disk disk[DISK_SIZE];
    int choice;

    initializeDisk(disk);

    do
    {
        printf("\n1. Add File\n2. Delete File\n3. Display Disk\n0. Exit\n");
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
        case 0:
            printf("Exiting");
        default:
            printf("error: option invalide\n");
        }

    } while (choice != 0);

    return 0;
}