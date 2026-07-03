#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[40];
    int count;
} wordStat;

typedef void (*text)(char *);
typedef void (*stats)(wordStat *, int);
typedef float (*custom)(char *);

// read input
void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }

    int len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
}

// convert word to small letters
void norm_word(char *w) {
    char temp[40];
    int i = 0, j = 0;

    while (w[i] != '\0') {
        if (isalpha((unsigned char)w[i])) {
            temp[j++] = tolower((unsigned char)w[i]);
        }
        i++;
    }

    temp[j] = '\0';
    strcpy(w, temp);
}

// add to array
void add_word(wordStat **stats, int *used, char *word) {
    for (int i = 0; i < *used; i++) {
        if (strcmp((*stats)[i].name, word) == 0) {
            (*stats)[i].count++;
            return;
        }
    }

    wordStat *tmp = realloc(*stats, (*used + 1) * sizeof(wordStat));
    if (!tmp) {
        printf("Memory error\n");
        return;
    }

    *stats = tmp;
    strcpy((*stats)[*used].name, word);
    (*stats)[*used].count = 1;
    (*used)++;
}

// split into words
void split_text(char *text, wordStat **stats, int *used) {
    char word[40];
    int k = 0;

    for (char *p = text; *p != '\0'; p++) {
        if (isalpha((unsigned char)*p)) {
            if (k < 39) word[k++] = *p;
        } else if (k > 0) {
            word[k] = '\0';
            norm_word(word);
            add_word(stats, used, word);
            k = 0;
        }
    }

    if (k > 0) {
        word[k] = '\0';
        norm_word(word);
        add_word(stats, used, word);
    }
}

// count words
void count_words(char *text) {
    int count = 0, in_word = 0;

    for (char *p = text; *p; p++) {
        if (isalpha((unsigned char)*p)) {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    printf("Total words: %d\n", count);
}

// longest word
void long_word(char *text) {
    char word[40], longest[40] = "";
    int k = 0;

    for (char *p = text; *p; p++) {
        if (isalpha((unsigned char)*p)) {
            if (k < 39) word[k++] = *p;
        } else if (k > 0) {
            word[k] = '\0';
            if (strlen(word) > strlen(longest)) {
                strcpy(longest, word);
            }
            k = 0;
        }
    }

    if (k > 0) {
        word[k] = '\0';
        if (strlen(word) > strlen(longest)) {
            strcpy(longest, word);
        }
    }

    printf("Longest word: %s\n", longest);
}

// most frequent word
void most_frequent(wordStat *stats, int used) {
    if (used == 0) {
        printf("No words found\n");
        return;
    }

    int max = 0;
    for (int i = 1; i < used; i++) {
        if (stats[i].count > stats[max].count) {
            max = i;
        }
    }

    printf("Most frequent word: %s (%d times)\n",
           stats[max].name, stats[max].count);
}

// average word length
float avg_len(char *text) {
    int letters = 0, words = 0;
    char word[40];
    int k = 0;

    for (char *p = text; *p; p++) {
        if (isalpha((unsigned char)*p)) {
            if (k < 39) word[k++] = *p;
        } else if (k > 0) {
            word[k] = '\0';
            letters += strlen(word);
            words++;
            k = 0;
        }
    }

    if (k > 0) {
        word[k] = '\0';
        letters += strlen(word);
        words++;
    }

    if (words == 0) return 0.0f;
    return (float)letters / words;
}

// display stats
void display_stats(wordStat *stats, int used) {
    if (used == 0) {
        printf("No stats yet\n");
        return;
    }

    printf("\nWord\tCount\n");
    for (int i = 0; i < used; i++) {
        printf("%s\t%d\n", stats[i].name, stats[i].count);
    }
}

/* reset */
void reset(wordStat **stats, int *used) {
    free(*stats);
    *stats = NULL;
    *used = 0;
}

int main(void) {
    char *text = malloc(2048);
    wordStat *stats = NULL;
    int used = 0;
    int choice;

    void (*count)(char *) = count_words;
    void (*longest)(char *) = long_word;
    void (*freq)(wordStat *, int) = most_frequent;
    float (*custom)(char *) = avg_len;

    if (!text) {
        printf("Memory error\n");
        return 1;
    }

    printf("Student ID: ALU1000\n");
    printf("Analysis Function: Average Word Length\n\n");

    printf("Enter paragraph:\n");
    read_line(text, 2048);

    split_text(text, &stats, &used);

    printf("\nText memory address: %p\n", (void *)text);
    printf("Stats memory address: %p\n", (void *)stats);

    while (1) {
        printf("\n1. Count words\n");
        printf("2. Longest word\n");
        printf("3. Most frequent word\n");
        printf("4. Display stats\n");
        printf("5. Custom analysis\n");
        printf("6. Reset\n");
        printf("7. Exit\n");
        printf("Choice: ");

        scanf("%d", &choice);
        getchar();

        if (choice == 1) count(text);
        else if (choice == 2) longest(text);
        else if (choice == 3) freq(stats, used);
        else if (choice == 4) display_stats(stats, used);
        else if (choice == 5)
            printf("Average word length: %.2f\n", custom(text));
        else if (choice == 6) {
            reset(&stats, &used);
            printf("Enter new paragraph:\n");
            read_line(text, 2048);
            split_text(text, &stats, &used);
        }
        else if (choice == 7) break;
        else printf("Invalid choice\n");
    }

    free(text);
    free(stats);

    printf("Done.\n");
    return 0;
}

