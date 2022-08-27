/* Authors: 
    El Mehdi El Boustani
    CSC 2302 PROJECT
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_SIZE 20
typedef struct nd
{
    char bookName[MAX_SIZE];
    int bookISBN;
    int borrowed;
    char borrower[MAX_SIZE];
    char due_date[MAX_SIZE];
    char field[MAX_SIZE];
    struct nd *prev, *next;
} book;

book *head = NULL;
void menu();
void read_data(char *);
void insert(book *);
int cmp(char *s1, char *s2);
void display();
int exists(int);
book *search(int);
void searchF(char *);
int existsF(char *);
int existsN(char *);
int existsB(char *);
void searchN(char *);
void searchB(char *);
void save(char *);
void deleteI(int isbn);
char *_gets(char *);
char *_fgets(char *, FILE *);
void fixB(book *node);
void fixN(char *);

int main(void)
{
    char path[MAX_SIZE];
    printf("Please enter the path to the text file: ");
    fgets(path, MAX_SIZE, stdin);

    strtok(path, "\n"); //takes out "\n"
    read_data(path);    // reads data from the file with directory: path
    menu(path);         //does all the rest
}
/*Read Data*/
void read_data(char *path)
{
    FILE *fp;

    while ((fp = fopen(path, "r")) == NULL)
    {
        printf("File could not open. Please enter another path: \n");
        fgets(path, MAX_SIZE, stdin);
        strtok(path, "\n");
    }

    char tmp[MAX_SIZE];
    // feof(fp) detects if we arrived to the end of the file
    while (!feof(fp))
    {
        book *temp = (book *)malloc(sizeof(book));
        _fgets(temp->bookName, fp);
        fscanf(fp, "%d", &temp->bookISBN);
        getc(fp);
        fscanf(fp, "%d", &temp->borrowed);
        getc(fp);
        _fgets(temp->borrower, fp);
        _fgets(temp->due_date, fp);
        _fgets(temp->field, fp);
        temp->prev = temp->next = NULL;
        insert(temp);
    }
    fclose(fp);
}
/*Read Data*/
/* MENU */
void menu(char *path)
{
    char name[MAX_SIZE];
    int answer;
    do
    {

        printf("\n\n**********************************\n*************  Menu  *************\n**********************************\n");
        printf("1. Display all Books.\n2. Insert a new Book.\n3. Update a book by ISBN.\n4. Delete a book by ISBN.\n5. Search.\n6. Exit.");
        printf("\n**********************************\n************ END Menu ************\n**********************************\n");
        printf("Your choice: ");
        scanf("%d", &answer);
        getchar();

        book *newB = (book *)malloc(sizeof(book)); // the book to be inserted used in function 2
        switch (answer)
        {

        case 1:
            //1. Display all Books.
            display(); // displays all the nodes
            break;

        case 2:
            //2. Insert a new Book.
            // first we look for the ISBN
            printf("Please enter the ISBN of the book: ");
            scanf("%d", &newB->bookISBN);
            getchar();
            while (exists(newB->bookISBN))
            {
                printf("Sorry! The book's ISBN already exists.\nPlease Try again: ");
                scanf("%d", &newB->bookISBN);
                getchar();
            }
            // here we are sure the ISBN does not exist
            printf("Please enter the name of the book: ");
            _gets(newB->bookName);
            printf("Is the book borrowed? (1/0): ");
            scanf("%d", &newB->borrowed);
            getchar();
            if (newB->borrowed == 0)
            {
                strcpy(newB->borrower, "None               "); // to keep the output aligned
                strcpy(newB->due_date, "None               ");
            }
            else
            {
                printf("Please enter the name of the borrower: ");
                _gets(newB->borrower);
                printf("Please enter the due date of the book's return: ");
                _gets(newB->due_date);
            }
            printf("Please enter the field of the book: ");
            _gets(newB->field);

            newB->next = newB->prev = NULL;
            // function insert makes the nodes sorted
            insert(newB);
            break;
            int isbn;
            book *book_t; // will save the book to be updated
        case 3:
            //3. Update a book by ISBN.
            printf("Please enter the ISBN to be updated: ");
            scanf("%d", &isbn);
            getchar();
            while (!exists(isbn))
            {
                printf("ISBN does not exist. Please try again: ");
                scanf("%d", &isbn);
                getchar();
            }
            book_t = search(isbn);
            if (book_t->borrowed == 1)
            {
                book_t->borrowed = 0;
                strcpy(newB->borrower, "None               ");
                strcpy(newB->due_date, "None               ");
                printf("Book updated successfully!\n");
                continue; // goes to next iteration
            }
            // we know that borrowed = 0 here
            book_t->borrowed = 1;
            printf("Please enter the name of the borrower: ");
            _gets(book_t->borrower);
            printf("Please enter the due date: ");
            _gets(book_t->due_date);
            break;
        case 4:
            //4. Delete a book by ISBN.
            printf("Please enter the ISBN: ");
            scanf("%d", &isbn);
            getchar();
            // delete handles if isbn does not exist
            deleteI(isbn);
            break;
            int choice;
            char tmp[MAX_SIZE]; // used to save the name in case 2, 3, and 4
        case 5:
            //5. Search.
            printf("\n\n**********************************\n*************  Menu  *************\n**********************************\n");
            printf("1 Search by ISBN.\n2 Search by field.\n3 Search by book name.\n4 Search for borrowers by borrower name.");
            printf("\n**********************************\n************ END Menu ************\n**********************************\n");
            printf("Please enter your choice: ");
            scanf("%d", &choice);
            getchar();
            switch (choice)
            {
            case 1:
                printf("Please enter the ISBN: ");
                scanf("%d", &isbn);
                getchar();
                book_t = search(isbn);
                if (search(isbn) == NULL)
                {
                    printf("ISBN does not exist.\n");
                    continue;
                }
                printf("Name\t\t\t\tISBN\t\tborrowed\tborrower\t\t\tdueDate\t\t\t\tfield\n\n");
                printf("%s\t", book_t->bookName);
                printf("%d\t\t", book_t->bookISBN);
                printf("%d\t\t\t", book_t->borrowed);
                printf("%s\t", book_t->borrower);
                printf("%s\t", book_t->due_date);
                printf("%s\n", book_t->field);
                break;
            case 2:
                printf("Please enter the field: ");
                _gets(tmp);
                searchF(tmp);
                break;
            case 3:
                printf("Please enter the book's name: ");
                _gets(tmp);
                searchN(tmp);
                break;
            case 4:
                printf("Please enter the borrower's name: ");
                _gets(tmp);
                searchB(tmp);
                break;
            default:
                printf("Wrong input.\nPlease try again.\n");
                break;
            }
            break;
        case 6:
            //6. Quit.
            save(path); // saves the information in a file
            printf("Thank you for using our program.\nGood bye.\n");
            break;
        default:
            printf("Wrong input.\nPlease try again.\n");
            break;
        }
    } while (answer != 6);
}
/* MENU */
void insert(book *temp)
{
    //DLL empty
    if (head == NULL)
    {
        head = temp;
        return;
    }

    //Insert before head
    if (!cmp(temp->bookName, head->bookName))
    {
        temp->next = head;
        head->prev = temp;
        head = temp;
        temp->prev = NULL;
        return;
    }
    //Insert in b/w two nodes
    book *w;
    for (w = head; w->next; w = w->next)
    {
        if (!cmp(temp->bookName, w->next->bookName))
        {
            temp->next = w->next;
            temp->prev = w;
            w->next->prev = temp;
            w->next = temp;
            return;
        }
    }
    // insert at the end
    w->next = temp;
    temp->prev = w;
}
int cmp(char *s, char *s_)
{
    char s1[MAX_SIZE], s2[MAX_SIZE];
    strcpy(s1, s);
    strcpy(s2, s_);
    for (int i = 0; i < strlen(s1); i++)
    {
        s1[i] = tolower(s1[i]);
    }
    for (int i = 0; i < strlen(s2); i++)
    {
        s2[i] = tolower(s2[i]);
    }
    if (strncmp(s1, s2, MAX_SIZE) < 0)
        return 0;
    if (!strcmp(s1, s2))
        return 1;
    return 2;
    // returns 0 if s1 < s2
    // returns 1 if s1 = s2
    // returns 2 if s1 > s2
}
void display()
{
    printf("Name\t\t\t\tISBN\t\tborrowed\tborrower\t\t\tdueDate\t\t\t\tfield\n\n");
    for (book *w = head; w; w = w->next)
    {
        printf("%s\t", w->bookName);
        printf("%d\t\t", w->bookISBN);
        printf("%d\t\t\t", w->borrowed);
        printf("%s\t", w->borrower);
        printf("%s\t", w->due_date);
        printf("%s\n", w->field);
    }
}
int exists(int isbn)
{
    for (book *w = head; w; w = w->next)
    {
        if (w->bookISBN == isbn)
            return 1;
    }
    return 0;
    // returns 1 if the book is found
}
book *search(int isbn)
{
    if (head == NULL)
        return NULL;
    for (book *w = head; w; w = w->next)
    {
        if (w->bookISBN == isbn)
            return w;
    }
    return NULL;
    // returns the pointer to the node if it exists otherwise it returns NULL
}
void searchF(char *s)
{
    if (!existsF(s))
    {
        printf("Field not found.\n");
        return;
    }
    printf("Name\t\t\t\tISBN\t\tborrowed\tborrower\t\t\tdueDate\t\t\t\tfield\n\n");
    for (book *w = head; w; w = w->next)
    {
        if (cmp(w->field, s) == 1)
        {
            printf("%s\t", w->bookName);
            printf("%d\t\t", w->bookISBN);
            printf("%d\t\t\t", w->borrowed);
            printf("%s\t", w->borrower);
            printf("%s\t", w->due_date);
            printf("%s\n", w->field);
        }
    }
}
int existsF(char *s)
{
    for (book *w = head; w; w = w->next)
    {
        if (cmp(w->field, s) == 1)
        {
            return 1;
        }
    }
    return 0;
}
void searchN(char *s)
{
    if (!existsN(s))
    {
        printf("The book does not exist.\n");
        return;
    }
    printf("Name\t\t\t\tISBN\t\tborrowed\tborrower\t\t\tdueDate\t\t\t\tfield\n\n");
    for (book *w = head; w; w = w->next)
    {
        if (cmp(w->bookName, s) == 1)
        {
            printf("%s\t", w->bookName);
            printf("%d\t\t", w->bookISBN);
            printf("%d\t\t\t", w->borrowed);
            printf("%s\t", w->borrower);
            printf("%s\t", w->due_date);
            printf("%s\n", w->field);
        }
    }
}
void searchB(char *s)
{
    if (!existsB(s))
    {
        printf("The borrower does not exist.\n");
        return;
    }
    printf("Name\t\t\t\tISBN\t\tborrowed\tborrower\t\t\tdueDate\t\t\t\tfield\n\n");
    for (book *w = head; w; w = w->next)
    {
        if (cmp(w->borrower, s) == 1)
        {
            printf("%s\t", w->bookName);
            printf("%d\t\t", w->bookISBN);
            printf("%d\t\t\t", w->borrowed);
            printf("%s\t", w->borrower);
            printf("%s\t", w->due_date);
            printf("%s\n", w->field);
        }
    }
}
void save(char *path)
{
    FILE *fp;
    if ((fp = fopen(path, "w")) == NULL)
    {
        printf("File could not open.\n");
        return;
    }

    for (book *w = head; w; w = w->next)
    {
        fixB(w); // removes additional spaces in the names
        fprintf(fp, "%s\n", w->bookName);
        fprintf(fp, "%d\n", w->bookISBN);
        fprintf(fp, "%d\n", w->borrowed);
        fprintf(fp, "%s\n", w->borrower);
        fprintf(fp, "%s\n", w->due_date);
        fprintf(fp, "%s", w->field);
        if (w->next != NULL)
            fputs("\n", fp); // prints "\n" only if we havent reached the end yet
    }
    fclose(fp);
}
void deleteI(int isbn)
{
    if (!exists(isbn))
    {
        printf("ISBN does not exist.\n");
        return;
    }
    //if the head points to the node with the isbn
    if (head->bookISBN == isbn)
    {
        head = head->next;
        return;
    }
    // if the node is in the middle
    book *w;
    for (w = head; w->next; w = w->next)
    {
        if (w->bookISBN == isbn)
        {
            w->prev->next = w->next;
            w->next->prev = w->prev;
            return;
        }
    }
    // if the node is at the end
    w->prev->next = NULL;
}
char *_gets(char *s)
{
    fgets(s, MAX_SIZE, stdin);
    strtok(s, "\n");
    for (int i = strlen(s); i < MAX_SIZE - 1; i++)
    {
        s[i] = ' ';
    }
    s[MAX_SIZE - 1] = '\0';

    return s;
}
// adds spaces to the end to be printed alligned
char *_fgets(char *s, FILE *fp)
{
    fgets(s, MAX_SIZE, fp);
    strtok(s, "\n");
    for (int i = strlen(s); i < MAX_SIZE - 1; i++)
    {
        s[i] = ' ';
    }
    s[MAX_SIZE - 1] = '\0';

    return s;
}
int existsN(char *name)
{
    for (book *w = head; w; w = w->next)
        if (cmp(w->bookName, name) == 1)
            return 1;
    return 0;
}
int existsB(char *name)
{
    for (book *w = head; w; w = w->next)
        if (cmp(w->borrower, name) == 1)
            return 1;
    return 0;
}
void fixB(book *node)
{
    fixN(node->bookName);
    fixN(node->borrower);
    fixN(node->due_date);
    fixN(node->field);
}
void fixN(char *s)
{
    for (int i = strlen(s) - 1; s[i] == ' '; i--)
    {
        s[i] = '\0';
    }
}