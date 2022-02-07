#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define max_size 2048

// struct to represent the puzzle
struct puzzle {
    // where region represents a row ,column or diagonal line
    char *region;
    struct puzzle *next;
};
struct words {
    char *word ;
    struct words *prev, *next;
};

// roots
struct words *head = NULL;
struct puzzle *root = NULL;


void parse_puzzle (char *filename);
void create_word_list();
void find_words(int len);


int main(int argc , char *args[]) {

    if (argc < 3 || argc > 3){
        printf("-> EXAMPLE USAGE : solver[puzzle name] [word size]\n    solver 8 5");
        exit(1);
    }

    int word_size = atoi(args[2]);

    parse_puzzle(args[1]);

    create_word_list();

    clock_t start = clock();

    find_words(word_size);

    clock_t stop = clock();

    double t = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("\nSearch took : %fs\n", t  );

    return EXIT_SUCCESS;
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
// find words
void find_words(int len) {
    struct words *temp;
    temp = head;
    printf("searching..\n");

    while (temp != NULL) {
        if (strlen(temp->word) == len){
            search_puzzle(temp->word);
        }
        temp = temp->next;
    }
}
/***********************************************************/
/*           strip \0 and \n                               */
/***********************************************************/
char *clean(char *str) {
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
/*           insert words into list                        */
/***********************************************************/
void insert_words(char *word){

    struct words * temp;
    temp = malloc(sizeof(*temp));
    if (temp==NULL){
        printf("ERROR-->TEMP");
        exit(1);
    }
    unsigned size = strlen(word);

    temp->word = malloc ( size * sizeof(char) + 1);

    if (temp->word==NULL){
        printf("ERROR-->temp->word");
        exit(1);
    }

    strcpy(temp->word,word);


    if (head == NULL){
        temp->next = NULL;
        temp->prev = NULL;
        head=temp;
    } else
    {
        temp->prev=NULL;
        temp->next=head;
        head->prev = temp;
        head=temp;
    }


}

void create_word_list(){

    FILE *new_file = fopen("lexis.txt", "r");
    char *new_word = malloc(sizeof(char) *255);

    while (fgets(new_word, sizeof(new_word) * 255 , new_file)) {
        if (new_word[0] == '\n'){
            break;
        }
        insert_words(clean(new_word));
    }
    fclose(new_file);
    free(new_word);
    // sort here after
}

/***********************************************************/
/*     insert regions into the puzzle linked list          */
/***********************************************************/
void insert_regions(char *str) {

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
void parse_puzzle(char *filename) {

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
        insert_regions(clean(line));
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
        insert_regions(str);
        k++;
    }

//    //insert diags code here
//    char (*string_array)[row_size];
//    string_array = malloc(columns * sizeof(char) + 1 );

}


