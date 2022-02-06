#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define max_size 2048

// struct to represent the puzzle
struct puzzle {
    // where region represents a row ,column or diagonal line
    char *region;
    struct puzzle *next;
};
struct puzzle *root = NULL;


void read_puzzle ( char *filename);
void search_puzzle (char * word);
char *remove_white_spaces (char *str);
void search_by_word_length(int len);


int main(int argc , char *args[]) {

    if (argc != 3){
        printf("Example usage : solver [file name] [word size]\n");
        printf("    solver puzzle 5\n");
        exit(1);
    }

    int word_size = atoi(args[2]);

    read_puzzle( args[1]);
    search_by_word_length(word_size);

    return EXIT_SUCCESS;
}

/***********************************************************/
/*           search for words by length                    */
/***********************************************************/
void search_by_word_length(int len) {

    char temp[max_size];
    char word[max_size];

    FILE *new_file = fopen("lexis.txt", "r");

    while (fgets(temp, sizeof(temp), new_file)) {

        strcpy(word, remove_white_spaces(temp));

        if (strlen(word) == len) {
            search_puzzle(word);
        }
    }
    fclose(new_file);
}

/***********************************************************/
/*           search the puzzle for words                   */
/***********************************************************/
void search_puzzle(char *word) {

    struct puzzle *temp;
    temp = root;

    while (temp != NULL) {
        // check if word exists in both directions
        if (strstr(temp->region, word) || strstr(strrev(temp->region), word)) {
            printf("Word Found -> %s\n", word);
        }
        temp = temp->next;
    }
}

/***********************************************************/
/*           strip \0 and \n                               */
/***********************************************************/
char *remove_white_spaces(char *str) {
    int i = 0, j = 0;
    while (str[i]) { // remove spaces and new lines
        if (str[i] != ' ' && str[i] != '\n')
            str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    return str;
}

/***********************************************************/
/*     insert regions into the puzzle linked list          */
/***********************************************************/
void insert(char *str) {

    struct puzzle *puzzle;
    struct puzzle *last_region = root;

    puzzle = (struct puzzle *) malloc(sizeof(*puzzle));
    puzzle->region = (malloc (sizeof(char) * strlen(str) +1 ));

    strcpy(puzzle->region, str);
    puzzle->next = NULL;

    if (root == NULL) {
        root = puzzle;
    } else {
        while (last_region->next != NULL) {
            last_region = last_region->next;
        }
        last_region->next = puzzle;
    }
}

/***********************************************************/
/*     read puzzle from file                               */
/***********************************************************/
void read_puzzle(char *filename) {

    char file_txt[40];
    strcpy(file_txt,filename);
    strcat(file_txt,".txt");

    FILE *file = fopen(file_txt, "r");

    if (file == NULL) {
        printf("Error reading file.");
        exit(1);
    }

    char line[max_size];
    while (fgets(line, sizeof(line), file)!=NULL) {

        if (line[0]=='\n'){
            break;
        }
        insert(remove_white_spaces(line));
    }
    fclose(file);

    struct puzzle *temp;

    temp = root;

    int i = 1; // col count
    unsigned j = strlen(temp->region);

    while (temp->next != NULL) {
        temp = temp->next;
        i++;
    }
    printf("------------------------------\n");
    printf("Puzzle Size : %d x %d\n",j,i);
    printf("------------------------------\n");

    int k = 0;
    int l = (int) (j * i); // total chars in grid
    int n = 0;
    int o;
    char *str = malloc(max_size * sizeof(char) + 1);

    // rows to columns
    while (n < l) {
        temp = root;
        o = 0;
        while (o < i) { // i = size of columns
            str[o] = temp->region[k];
            temp = temp->next;
            n++;
            o++;
        }
        // add new column to linked list
        insert(str);
        k++;
    }
}
