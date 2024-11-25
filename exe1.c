#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOK_FILE "book.txt"
#define INDEX_FILE "index.txt"

typedef struct
{
  int ID;
  char title[50];
  char author[50];
  int year;
} Book;

typedef struct
{
  int bookID;
  long position;
} Index;

Index *bookIndex = NULL;
int indexCount = 0;

int isUniqueID(int ID)
{

  for (int i = 0; i < indexCount; i++)
  {
    if (bookIndex[i].bookID == ID)
    {
      return 0;
    }
  }

  FILE *file = fopen(BOOK_FILE, "rb");
  if (!file)
    return 1;

  Book book;
  while (fread(&book, sizeof(Book), 1, file))
  {
    if (book.ID == ID)
    {
      fclose(file);
      return 0;
    }
  }
  fclose(file);

  return 1;
}

void loadIndexInMemory()
{
  FILE *file = fopen(INDEX_FILE, "rb");
  if (!file)
    return;

  while (fread(&bookIndex[indexCount], sizeof(Index), 1, file))
  {
    indexCount++;
    bookIndex = realloc(bookIndex, (indexCount + 1) * sizeof(Index));
  }

  fclose(file);
}

void saveIndex()
{
  FILE *file = fopen(INDEX_FILE, "wb");
  for (int i = 0; i < indexCount; i++)
  {
    fwrite(&bookIndex[i], sizeof(Index), 1, file);
  }
  fclose(file);
}

void addBook()
{
  Book book;
  printf("Enter Book ID: ");
  scanf("%d", &book.ID);

  while (!isUniqueID(book.ID))
  {
    printf("Error: Book ID already exists. Enter a new ID: ");
    scanf("%d", &book.ID);
  }

  printf("Enter Title: ");
  getchar();
  fgets(book.title, sizeof(book.title), stdin);
  book.title[strcspn(book.title, "\n")] = '\0';

  printf("Enter Author: ");
  fgets(book.author, sizeof(book.author), stdin);
  book.author[strcspn(book.author, "\n")] = '\0';

  printf("Enter Year: ");
  scanf("%d", &book.year);

  FILE *file = fopen(BOOK_FILE, "ab");
  fseek(file, 0, SEEK_END);
  long position = ftell(file);
  fwrite(&book, sizeof(Book), 1, file);
  fclose(file);

  bookIndex = realloc(bookIndex, (indexCount + 1) * sizeof(Index));
  bookIndex[indexCount].bookID = book.ID;
  bookIndex[indexCount].position = position;
  indexCount++;

  FILE *f = fopen(INDEX_FILE, "ab");
  fwrite(&bookIndex[indexCount - 1], sizeof(Index), 1, f);
  fclose(f);
}

int searchBook(int ID)
{
 
  int low = 0, high = indexCount - 1;
  while (low <= high)
  {
    int mid = (low + high) / 2;
    if (bookIndex[mid].bookID == ID)
    {
      return bookIndex[mid].position; 
    }
    else if (bookIndex[mid].bookID < ID)
    {
      low = mid + 1;
    }
    else
    {
      high = mid - 1;
    }
  }

  
  FILE *file = fopen(BOOK_FILE, "rb");
  if (!file)
  {
    printf("Error opening file.\n");
    return -1;
  }

  Book book;
  while (fread(&book, sizeof(Book), 1, file))
  {
    if (book.ID == ID)
    {
      long position = ftell(file) - sizeof(Book); 
      fclose(file);
      return position; 
    }
  }

  fclose(file);
  return -1;
}


void deleteBook()
{
  int ID;
  printf("Enter Book ID to delete: ");
  scanf("%d", &ID);

  int position = searchBook(ID);
  if (position != -1)
  {

    FILE *file = fopen(BOOK_FILE, "rb+");
    fseek(file, position, SEEK_SET);
    Book emptyBook = {0};
    fwrite(&emptyBook, sizeof(Book), 1, file);
    fclose(file);

    int i, j;
    for (i = 0; i < indexCount; i++)
    {
      if (bookIndex[i].bookID == ID)
      {
        break;
      }
    }

    for (j = i; j < indexCount - 1; j++)
    {
      bookIndex[j] = bookIndex[j + 1];
    }

    indexCount--;

    saveIndex();
  }
  else
  {
    printf("Book not found.\n");
  }
}

void displayFileContent()
{
  FILE *file = fopen(BOOK_FILE, "rb");
  if (!file)
  {
    printf("Error opening file.\n");
    return;
  }

  Book book;
  while (fread(&book, sizeof(Book), 1, file))
  {
    if (book.ID != 0)
      printf("ID: %d, Title: %s, Author: %s, Year: %d\n", book.ID, book.title, book.author, book.year);
  }
  fclose(file);
}

void displayIndexContent()
{
  FILE *file = fopen(INDEX_FILE, "rb");
  if (!file)
  {
    printf("Error opening index file.\n");
    return;
  }

  Index indexEntry;
  while (fread(&indexEntry, sizeof(Index), 1, file))
  {
    printf("Book ID: %d, Position: %ld\n", indexEntry.bookID, indexEntry.position);
  }

  fclose(file);
}


int main()
{
  loadIndexInMemory();

  int choice;
  do
  {
    printf("\nLibrary Management System\n");
    printf("1. Add Book\n");
    printf("2. Search Book\n");
    printf("3. Delete Book\n");
    printf("4. Display Books\n");
    printf("5. Display Index\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      addBook();
      saveIndex();
      break;
    case 2:
    {
      int searchID;
      printf("Enter Book ID to search: ");
      scanf("%d", &searchID);
      int position = searchBook(searchID);
      if (position != -1)
      {
        FILE *file = fopen(BOOK_FILE, "rb");
        fseek(file, position, SEEK_SET);
        Book book;
        fread(&book, sizeof(Book), 1, file);
        printf("ID: %d, Title: %s, Author: %s, Year: %d\n", book.ID, book.title, book.author, book.year);
        fclose(file);
      }
      else
      {
        printf("Book not found.\n");
      }
      break;
    }
    case 3:
      deleteBook();
      saveIndex();
      break;
    case 4:
      displayFileContent();
      break;
    case 5:
      displayIndexContent();

      break;
    case 0:
      saveIndex();
      free(bookIndex);
      printf("Exiting program.\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 0);

  return 0;
}
