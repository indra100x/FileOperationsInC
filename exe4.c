#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define n 100

typedef struct INODE {
    int fsize;
    int* fsectores;
} INODE;

typedef struct File {
    char name[3];
    INODE tab;
} File;

File files[n];
int disk[n];

void initializeDisk() {
    for (int i = 0; i < n; i++) {
        strcpy(files[i].name, " ");
        disk[i] = 0;
        files[i].tab.fsectores = NULL;
    }
}

void displayInode() {
    char lastName[3] = " ";
    for (int i = 0; i < n; i++) {
        if (strcmp(files[i].name, " ") != 0) {
            if (strcmp(files[i].name, lastName) != 0) {
                printf("\nName of file: %s\nSectors: ", files[i].name);
            }
            for (int j = 0; j < files[i].tab.fsize; j++) {
                printf("%d -> ", files[i].tab.fsectores[j]);
            }
            strcpy(lastName, files[i].name);
        }
    }
    printf("\n");
}

int checkSize(int size) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (disk[i] == 0) {
            count++;
        }
    }
    return (count >= size) ? 0 : 1;
}

int checkName(char* name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(files[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

void store(char* name, int size) {
    int count = 0, j = 0;
    for (int i = 0; i < n && count < size; i++) {
        if (disk[i] == 0) {
            strcpy(files[j].name, name);
            if (files[j].tab.fsectores == NULL) {
                files[j].tab.fsectores = malloc(size * sizeof(int));
            }
            files[j].tab.fsectores[count] = i;
            disk[i] = 1;
            count++;
        }
    }
    files[j].tab.fsize = size;
    printf("File '%s' stored successfully.\n", name);
}

void addFile() {
    char name[3];
    int size;

    do {
        printf("Enter the file name: ");
        scanf("%s", name);
    } while (checkName(name) == 1);

    do {
        printf("Enter the file size: ");
        scanf("%d", &size);
    } while (checkSize(size) == 1);

    store(name, size);
}

void deleteFile() {
    char name[3];
    printf("Enter the file name to delete: ");
    scanf("%s", name);

    for (int i = 0; i < n; i++) {
        if (strcmp(files[i].name, name) == 0) {
            for (int j = 0; j < files[i].tab.fsize; j++) {
                disk[files[i].tab.fsectores[j]] = 0;
            }
            free(files[i].tab.fsectores);
            files[i].tab.fsectores = NULL;
            strcpy(files[i].name, " ");
            files[i].tab.fsize = 0;
        }
    }
    printf("File '%s' deleted successfully.\n", name);
}

void fromRamToDisk() {
    for (int i = 0; i < n; i++) {
        if (strcmp(files[i].name," ")!=0)
        {
          disk[i]=1;
        }
        
    }
}

void displayDisk() {
    fromRamToDisk();
    for (int i = 0; i < n; i++) {
        printf("%d| ", disk[i]);
        if (i % 10 == 9) {
            printf("\n");
        }
    }
}

int main() {
    int choice;
    initializeDisk();
    do {
        printf("\nMenu:\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Display Disk Status\n");
        printf("4. Display Files Status\n");
        printf("0. Quit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addFile();
                break;
            case 2:
                deleteFile();
                break;
            case 3:
                displayDisk();
                break;
            case 4:
                displayInode();
                break;
            case 0:
                printf("Exiting.\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 0);

    return 0;
}