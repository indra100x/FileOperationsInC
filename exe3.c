#include <stdio.h>
#include <string.h>

#define DISK_SIZE 100
#define INDEX_BLOCK_SIZE 5

typedef struct File {
    char data[3]; 
    int size;
}File;
int disk[DISK_SIZE];
int FAT[DISK_SIZE];
int freeIndexBlock = 0;

void init_disk() {
    int i;
    for ( i = 0; i < DISK_SIZE; i++) {
           disk[i]==0;
        }
        FAT[i] = -1;
    }


int find_free_sector() {
    for (int i = 0; i < DISK_SIZE; i++) {
        if (strcmp(disk[i].data, "0") == 0) return i;
    }
    return -1;
}

int allocate_index_block() {
    if (freeIndexBlock < DISK_SIZE) return freeIndexBlock++;
    return -1;
}

void create_file() {
    char name[3];
    int size, count = 0;
    printf("Enter file name (2 characters): ");
    scanf("%2s", name);
    printf("Enter file size (sectors): ");
    scanf("%d", &size);

    int indexBlock = allocate_index_block();
    if (indexBlock == -1) {
        printf("No space for index block.\n");
        return;
    }

    int currentIndexBlock = indexBlock;
    while (count < size) {
        for (int i = 0; i < INDEX_BLOCK_SIZE && count < size; i++) {
            int freeSector = find_free_sector();
            if (freeSector == -1) {
                printf("Not enough space to create file.\n");
                return;
            }
            strcpy(disk[freeSector].data, name);
            FAT[currentIndexBlock].indices[i] = freeSector;
            count++;
        }
        if (count < size) {
            int newIndexBlock = allocate_index_block();
            if (newIndexBlock == -1) {
                printf("No space for additional index block.\n");
                return;
            }
            FAT[currentIndexBlock].next = newIndexBlock;
            currentIndexBlock = newIndexBlock;
        }
    }

    printf("File '%s' created.\n", name);
}

void delete_file() {
    char name[3];
    printf("Enter file name to delete: ");
    scanf("%2s", name);

    for (int i = 0; i < freeIndexBlock; i++) {
        for (int j = 0; j < INDEX_BLOCK_SIZE; j++) {
            int sector = FAT[i].indices[j];
            if (sector != -1 && strcmp(disk[sector].data, name) == 0) {
                strcpy(disk[sector].data, "0");
                FAT[i].indices[j] = -1;
            }
        }
    }
    printf("File '%s' deleted.\n", name);
}

void display_disk_status() {
    printf("Disk Status:\n");
    for (int i = 0; i < DISK_SIZE; i++) {
        if (i % 10 == 0) printf("\n");
        if (strcmp(disk[i].data, "0") == 0) printf("##| ");
        else printf("%s| ", disk[i].data);
    }
    printf("\n");
}

void display_index_blocks() {
    printf("Index Blocks:\n");
    for (int i = 0; i < freeIndexBlock; i++) {
        printf("Index Block %d: ", i);
        for (int j = 0; j < INDEX_BLOCK_SIZE; j++) {
            if (FAT[i].indices[j] != -1) {
                printf("%d ", FAT[i].indices[j]);
            }
        }
        if (FAT[i].next != -1) {
            printf("-> %d", FAT[i].next);
        }
        printf("\n");
    }
}

int main() {
    init_disk();
    int choice;

    do {
        printf("\nMenu:\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Display Disk Status\n");
        printf("4. Display Index Blocks\n");
        printf("5. Quit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_file(); break;
            case 2: delete_file(); break;
            case 3: display_disk_status(); break;
            case 4: display_index_blocks(); break;
            case 5: printf("Exiting.\n"); break;
            default: printf("Invalid option.\n");
        }
        } while (choice != 5);

    return 0;
}