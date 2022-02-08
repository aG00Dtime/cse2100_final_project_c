#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define max_size 2048

// struct to represent the puzzle
struct word_grid {
    // where region represents a row ,column or diagonal line
    char *region;
    struct word_grid *next;
};

// struct to store the words
struct words {
    char *word;
    struct words *prev, *next;
};

// roots
struct words *words_root = NULL;
struct word_grid *grid_root = NULL;

void parse_puzzle(char *filename);

void create_word_list();

void search_puzzle(char *word);

void search_for_words(int len);

//count
int found_count = 0;
int searched_count = 0;

// main
int main(int argc, char *args[]) {

    if (argc < 3 || argc > 3) {

        printf("Run from terminal:\n-> EXAMPLE USAGE : solver[puzzle name] [word size]\n    solver 8 5");
        exit(1);
    }

    int word_size = atoi(args[2]);

    if (word_size == 1) {
        printf("\n->Word size must be 2 letter words or more.\n");
        exit(2);
    }

    parse_puzzle(args[1]);

    create_word_list();

    clock_t start = clock();
    search_for_words(word_size);
    clock_t stop = clock();

    double t = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nSearch took : %f s\n", t);
    printf("Searched %d times and found %d matches.\n", searched_count, found_count);

    return EXIT_SUCCESS;
}

/***********************************************************/
/*           reverse string func                           */
/***********************************************************/
char *reverse_str(char str[]) {
    unsigned n = strlen(str);
    int i;
    char ch;

    for (i = 0; i < n / 2; i++) {
        ch = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = ch;
    }

    return str;

}

/***********************************************************/
/*           search the puzzle for words                   */
/***********************************************************/
void search_puzzle(char *word) {
    struct word_grid *temp;
    temp = grid_root;

    while (temp != NULL) {

        if (strlen(temp->region) >= strlen(word)) {

            // check if word exists in both directions,return immediately if found
            if (strstr(temp->region, word) != NULL) {
                printf("Word Found -> %s\n", word);
                found_count++;
                return;
            }
            if (strstr(temp->region, reverse_str(word)) != NULL) {
                printf("Word Found -> %s\n", word);
                found_count++;
                return;
            }
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
void insert_words(char *word) {

    struct words *temp;
    temp = malloc(sizeof(*temp));
    if (temp == NULL) {
        printf("ERROR->OVERFLOW[TEMP]");
        exit(1);
    }
    unsigned size = strlen(word);

    temp->word = malloc(size * sizeof(char) + 1);
    if (temp->word == NULL) {
        printf("ERROR->OVERFLOW[TEMP->WORD]");
        exit(1);
    }

    strcpy(temp->word, (word));
    temp->next = temp->prev = NULL;

    if (words_root == NULL) {
        words_root = temp;
    } else {
        temp->next = words_root;
        words_root->prev = temp;
        words_root = temp;
    }

}

/***********************************************************/
/*     insert regions into the puzzle linked list          */
/***********************************************************/
void insert_regions(char *str) {

    struct word_grid *grid;
    struct word_grid *last_region = grid_root;

    grid = malloc(strlen(str) * (char) +1);

    if (grid == NULL) {
        printf("ERROR->OVERFLOW[GRID]");
        exit(1);
    }
    grid->region = malloc(strlen(str) * sizeof(char) + 1);

    if (grid->region == NULL) {
        printf("ERROR->OVERFLOW[GRID_REGION]");
        exit(1);
    }
    strcpy(grid->region, str);

    grid->next = NULL;

    if (grid_root == NULL) {
        grid_root = grid;

    } else {
        while (last_region->next != NULL) {
            last_region = last_region->next;
        }
        last_region->next = grid;
    }

}
/***********************************************************/
/*     read puzzle from file                               */
/***********************************************************/
void parse_puzzle(char *filename) {

    char file_txt[40];
    strcpy(file_txt, filename);
    strcat(file_txt, ".txt");

    FILE *file = fopen(file_txt, "r");

    if (file == NULL) {
        printf("Error reading file.");
        exit(1);
    }

    char *line = malloc(max_size * sizeof(line) + 1);
    if (line == NULL) {
        printf("ERROR->OVERFLOW[LINE]");
        exit(1);
    }

    while (fgets(line, max_size * sizeof(line) + 1, file) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        insert_regions(clean(line));
    }

    fclose(file);


    struct word_grid *temp;
    temp = grid_root;

    int row_size = 1; // row_size in grid

    unsigned column_size = strlen(temp->region);

    while (temp->next != NULL) {
        temp = temp->next;
        row_size++;
    }
    printf("------------------------------\n");
    printf("Puzzle Size : %d x %d\n", row_size, column_size);
    printf("------------------------------\n");

    int k = 0, n = 0, o;
    int total_chars = (int) (column_size * row_size); // total chars in grid


    char *str = malloc(column_size * sizeof(char) + 1);

    if (str == NULL) {
        printf("ERROR->OVERFLOW[STR]");
        exit(1);
    }

    // row to columns
    while (n < total_chars) {
        temp = grid_root;
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

//    Diagonal rows
    char **string_array = malloc(row_size * sizeof(char *));
    if (string_array == NULL) {
        printf("ERROR->OVERFLOW[STRING_ARRAY]");
        exit(1);
    }
    int i, j;
    for (i = 0; i < row_size; ++i) {
        string_array[i] = (char *) malloc(column_size * sizeof(char) +1);
        if (string_array[i] == NULL) {
            printf("ERROR->OVERFLOW[STRING_ARRAY[i]");
            exit(1);
        }
    }

    temp = grid_root;

    for (j = 0; j < row_size; ++j) {
        strcpy(string_array[j], temp->region);
        temp = temp->next;
    }

    for (k = 0; k < row_size - 1; k++) {

        i = k;
        j = 0;

        while (i >= 0) {
//            printf("[%c]", string_array[i][j]);
            str[j] = string_array[i][j];
            i--;
            j++;
        }
        str[j] = '\0';
        insert_regions(str);
    }

    unsigned c = column_size;

    for (k = 0; k <= column_size - 1; ++k) {
        i = row_size - 1;
        j = k;
        o = 0;
        c--;
        while (j <= column_size - 1) {
            str[o] = string_array[i][j];
            i--;
            j++;
            o++;
        }
        str[c + 1] = '\0';
        insert_regions(str);
    }

    for (i = 0; i < row_size; ++i) {
        free(string_array[i]);
        string_array[i] = NULL;
    }

    free(string_array);
    string_array = NULL;


}


/***********************************************************/
/* comparison method for qsort() - compares string length  */
/***********************************************************/
int compare(const void *a, const void *b) {
    const struct words *string_a = a;
    const struct words *string_b = b;

    return (int) strlen(string_a->word) - (int) strlen(string_b->word);
}

/***********************************************************/
/*     read words from file and sort them by length        */
/***********************************************************/

void create_word_list() {
    // using qsort() to sort words before inserting into linked list
    FILE *file = fopen("lexis.txt", "r");

    if (file == NULL) {
        printf("FILE ERROR");
        exit(1);
    }

    char temp_word[255];
    int count = 0;
    int i = 0;

    while (fgets(temp_word, 255, file)) {
        count++;
    }

    char **temp_array;
    temp_array = malloc( count * sizeof(char *) + 1  );

    if (temp_array == NULL) {
        printf("ERROR->OVERFLOW[TEMP_ARRAY]");
        exit(1);
    }
    // add words to a temp array for sorting
    fseek(file, 0, SEEK_SET);
    while (fgets(temp_word, 255, file)) {

        temp_array[i] = malloc(strlen(temp_word) * sizeof(char) + 1 );

        if (temp_array[i] == NULL) {
            printf("ERROR->OVERFLOW[TEMP_ARRAY[i]");
            exit(1);

        }
        strcpy(temp_array[i], clean(temp_word));
        i++;
    }
    fclose(file);

    // quick sorting by word length
    qsort(temp_array, count, sizeof(temp_array[0]), compare);

    // add sorted list of words to linked list
    while (count > 0) {
        insert_words(temp_array[count - 1]);
        free(temp_array[count - 1]);
        (temp_array[count - 1]) = NULL;
        count--;
    }

    free(temp_array);
    temp_array = NULL;
}

/***********************************************************/
/*    search list for wards matching of matching length    */
/***********************************************************/
void search_for_words(int len) {

    struct words *temp, *first, *last, *mid;

    first = mid = last = words_root;
    int front = 0, middle = 0;

    // find middle and last words
    while (last != NULL && last->next != NULL) {
        last = last->next->next;
        mid = mid->next;
    }
    if (len > strlen(first->word) && len <= strlen(mid->word)) {
        temp = first;
        front = 1;
    } else if (len > strlen(mid->word)) {
        temp = mid;
        middle = 1;
    } else {
        temp = last;
    }
    // find the words matching length given
    printf("Searching for all %d letter words...\n", len);

    while (1) {

        if (strlen(temp->word) == len) {
            search_puzzle(temp->word);
            searched_count++;
        }
        if (front || middle) {
            if (strlen(temp->next->word) > len) {
                break;
            } else {
                temp = temp->next;
            }
        } else {
            temp = temp->prev;
        }
    }

    free(temp);
    free(first);
    free(mid);
    free(last);

    temp = first = mid = last = NULL;
}



