#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define n 100

typedef struct {
char name[3];
int size;
}File;
typedef struct{
    char data[3];
}Sector;
Sector disk[n];
int Fat[n];

void initializeDisk(){
int i=0;
for(i=0;i<n;i++){
    strcpy(disk[i].data," ");
    Fat[i]=-1;
}
}
int checkSize(int size){
    int i=0;
    int count=0;
    for(i=0;i<n;i++){
        if(strcmp(disk[i].data," ")==0){
            count++;
        }
    }
    if(count >=size){
        printf("there is a suffissant size\n");
        return 0;
    }else{
        printf("inssufissant size");
        return 1;
    }
}
int checkName(char* name){
    int i=0;
    for(i=0;i<n;i++){
        if(strcmp(disk[i].data,name)==0){
            printf("name already taken\n");
            return 1;
        }
    }
    printf("name set seccussfully\n");
    return 0;
}
void store(char* name,int size){
    int i=0;
    int j=0;
    int count=0;
    for(i=0;i<n && count<size;i++){
        if(strcmp(disk[i].data," ")==0){
           strcpy(disk[i].data,name);
           Fat[j++]=i;
           count++;
        }
    }
}

void addFile(){
    File f;
    do{
       printf("enter the file name\n");
       scanf("%s",&f.name);
    }while(checkName(f.name)==1);
      do{
       printf("enter the file size\n");
       scanf("%d",&f.size);
    }while(checkSize(f.size)==1);

     store(f.name,f.size);
      
}
int FindInFat(int i){
    int j=0;
    for(j=0;j<n;j++){
        if(Fat[j]==i){
            return j;
        }
    }
    printf("slot is empty");
}
void deleteFile(){
    File f;
     printf("enter the file name\n");
     scanf("%s",&f.name);
     int i;
     for(i=0;i<n;i++){
        if(strcmp(disk[i].data,f.name)==0){
            strcpy(disk[i].data," ");
            int j= FindInFat(i);
            Fat[j]=-1;
        }
     }

}
void displayDisk(){
   int i;
   for(i=0;i<n;i++){
    if(strcmp(disk[i].data," ")==0){
        printf("##| ");
    }else{
        printf("%2s| ",disk[i].data);
    }
    if(i%10==9){
        printf("\n");
    }
   }
}
void displayFAT(){
int i;
for(i=0;i<n;i++){
    printf("%d| ",Fat[i]);
}
}

int main(){
     int choice;
initializeDisk();
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
            case 1: addFile(); break;
            case 2: deleteFile(); break;
            case 3: displayDisk(); break;
            case 4: displayFAT(); break;
            case 5: printf("Exiting.\n"); break;
            default: printf("Invalid option.\n");
        }
        } while (choice != 5);

    return 0;
}