#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define BUFFER_SIZE 250


// BST
struct node {
    char eng[BUFFER_SIZE];
    char span[BUFFER_SIZE];
    struct node * left, * right;
};

//set root to null;
struct node * root = NULL;

/* searching the tree until the english word is found and returns the node else return null */
struct node * search(char english[]) {

    struct node * current;

    current = root;

    while (current != NULL) {

        // check if word found
        if (strcmp(current -> eng, english) == 0) {
//            printf("%s ==> %s\n",current->eng,current->span);
            return current;

        }

        if (current -> eng > english) {
            current = current -> left;
        } else {
            current = current -> right;
        }

    }

    return NULL;
}

// inserts pairs into tree
void insert(char english[], char spanish[]) {

    struct node * temp = (struct node * ) malloc(sizeof(struct node));

    struct node * curr;
    struct node * par;

    // copy eng and spanish words to node
    strcpy(temp -> eng, english);
    strcpy(temp -> span, spanish);

    temp -> left = NULL;
    temp -> right = NULL;

    // add node
    if (root == NULL) {

        root = temp;

    } else {

        curr = root;
        par = NULL;

        while (1) {

            par = curr;

            if (english < par -> eng) {
                curr = curr -> left;

                if (curr == NULL) {
                    par -> left = temp;
                    return;
                }
            } else {
                curr = curr -> right;

                if (curr == NULL) {
                    par -> right = temp;
                    return;
                }
            }
        }
    }

}

// remove white spaces and new lines from words to prevent errors
char * remove_white_spaces(char * str) {

    int i = 0, j = 0;

    while (str[i]) { // remove spaces and new lines

        if (str[i] != ' ' && str[i] != '\n')

            str[j++] = str[i];

        i++;
    }
    str[j] = '\0';

    return str;
}

// reads the file and creates the bst
void create_dict() {


    char line[BUFFER_SIZE];
    char * eng, * spa;

    // read dict file
    FILE * file = fopen("dictionary.txt", "r");

    if (file == NULL) {
        printf("\nFile Error");
        exit(1);
    }

    while (fgets(line, sizeof(line), file) != NULL) {


        eng = strtok(line, ",");
        spa = strtok(NULL, ",");

        remove_white_spaces(eng);
        remove_white_spaces(spa);

        // insert word pairs as node in BST
        insert(eng, spa);

    }

    fclose(file);

}

void translate(){



    int ch;
    char *sentence[BUFFER_SIZE];
    char *segment ;
    struct node * word;

    while (1){

        printf("+----------------------------------------------+");
        printf("\nEnter English Sentence : ");
        fgets((char *) sentence, BUFFER_SIZE, stdin);
        fflush(stdin);



        segment = strtok((char *) sentence, " ");

        printf("\nSpanish Sentence : ");
        while (segment != NULL) {

            remove_white_spaces(segment);


            // search and print word

            word = search(segment);

            if (word == NULL) {
                printf("[word not found] ");

            } else {
                printf("%s ", word -> span);
            }


            segment = strtok(NULL, " ");
        }


        printf("\n+----------------------------------------------+");
        printf("\n\n1.TRANSLATE AGAIN\n2.QUIT\nCHOICE:");
        scanf("%d",&ch);
        fflush(stdin);

        if (ch==2){
            break;
        } else{
            continue;
        }

    }
}

int main() {

    // set locate to print special characters
    setlocale(LC_ALL, "");

    // create bst from file
    create_dict();

    // run translate
    translate();



    return EXIT_SUCCESS;
}