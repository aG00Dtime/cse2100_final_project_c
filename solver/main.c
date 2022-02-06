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

    printf("\nFinished");

    return EXIT_SUCCESS;
}

/***********************************************************/
/*           search for words by length                    */
/***********************************************************/
void search_by_word_length(int len) {

//    char temp[max_size+1];
    char *temp = malloc(max_size *sizeof(temp));
    if (temp==NULL){
        printf("ERROR->TEMP");
        exit(1);
    }
    char *word=malloc(max_size *sizeof(word));

    if (word==NULL){
        printf("ERROR->WORD");
        exit(1);
    }

    FILE *new_file = fopen("lexis.txt", "r");

    while (fgets(temp, max_size * sizeof(temp), new_file)) {

        strcpy(word, remove_white_spaces(temp));

        if (strlen(word) == len) {
            search_puzzle(word);
        }
    }

    fclose(new_file);
    free(temp);
    free(word);

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
//            printf("%s\n",temp->region);
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

    puzzle = malloc(sizeof(*puzzle));

    if (puzzle==NULL){
        printf("ERROR->puzzle");
        exit(1);
    }

    puzzle->region = malloc (sizeof(char) * strlen(str) +1 );

    if (puzzle->region==NULL){
        printf("ERROR->puzzle->region");
        exit(1);
    }

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

    char *line = malloc(max_size  * sizeof(line) + 1);
    if (line==NULL){
        printf("ERROR->LINE");
        exit(1);
    }

    while (fgets(line, max_size * sizeof(line ) +1, file)!=NULL) {
        if (line[0]=='\n'){
            break;
        }
        insert(remove_white_spaces(line));
    }

    fclose(file);
    free(file);
    free(line);

    struct puzzle *temp;
    temp = root;

    int row_size = 1; // row_size in grid

    unsigned column_size = strlen(temp->region);

    while (temp->next != NULL) {
//        printf("%s\n",temp->region);
        temp = temp->next;
        row_size++;
    }
    printf("------------------------------\n");
    printf("Puzzle Size : %d x %d\n", row_size, column_size);
    printf("------------------------------\n");

    int k = 0;
    int total_chars = (int) (column_size * row_size); // total chars in grid
    int n = 0;
    int o;

    char *str = malloc(max_size  * sizeof(char) + 1 );

    if(str==NULL){
        printf("ERROR->STR");
        exit(1);
    }

    // row to columns
    while (n < total_chars) {
        temp = root;
        o = 0;
        while (o < row_size) {
            str[o] = temp->region[k];
            temp = temp->next;
            n++;
            o++;
        }
        // add new column to linked list
        insert(str);
        k++;
    }

//    //insert diags code here
//    char (*string_array)[row_size];
//
//    string_array = malloc(columns * sizeof(char) + 1 );
//
//


}



