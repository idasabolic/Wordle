#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define word_count 2315   // number of allowed solution words
#define guess_count 14855 // number of allowed guess words

// function for printing in colors green and yellow;
void colorPrint(const char *text, const char *color) {
    printf("%s%s\033[0m", color, text);
}
#define green "\033[32m"
#define yellow "\033[33m"

int main() {
    // opening the answers file and picking a random word for the solution:
    FILE *answers = fopen("wordle.answers.txt", "r");
    if (!answers) {
        perror("wordle.answers.txt");
        return 1;
    }

    char words[word_count][6];
    for (int i = 0; i < word_count; i++) {
        fscanf(answers, "%5s", words[i]);
    }
    fclose(answers);

    srand(time(NULL));
    int index = rand() % word_count;

    char word[6];
    strcpy(word, words[index]);

    // opening the allowed guesses file to later check the input:
    FILE *guesses = fopen("wordle.guesses.txt", "r");
    if (!guesses) {
        perror("wordle.guesses.txt");
        return 1;
    }

    char solutions[guess_count][6];
    for (int i = 0; i < guess_count; i++) {
        fscanf(guesses, "%5s", solutions[i]);
    }
    fclose(guesses);

    // preparation done
    // the main algorithm:

    char guess[6];

    // checking for invalid inputs:
    for (int b = 0; b < 6; b++) {
        int t = 0;
        while (t == 0) {
            printf("%d. ", b + 1);
            scanf("%5s", guess);
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            for (int i = 0; i < 5; i++) {
                guess[i] = tolower(guess[i]);
            }
            for (int i = 0; i < guess_count; i++) {
                if (strcmp(guess, solutions[i]) == 0) {
                    t = 1;
                    break;
                }
            }
            if (t == 0) {
                printf("That is not a valid guess word!\n");
            }
        }

        // storing the number of each letters in the solution:
        int letter_count[26] = {0}, result[6] = {0};
        for (int i = 0; i < 5; i++) {
            letter_count[word[i] - 'a']++;
        }

        // returning the appropriate colors:
        // first pass: mark greens
        for (int i = 0; i < 5; i++) {
            if (guess[i] == word[i]) {
                result[i] = 1; // green
                letter_count[guess[i] - 'a']--;
            }
        }

        // second pass: mark yellows
        for (int i = 0; i < 5; i++) {
            if (result[i] == 0) { // not green
                if (letter_count[guess[i] - 'a'] > 0) {
                    result[i] = 2; // yellow
                    letter_count[guess[i] - 'a']--;
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            char letter[] = {guess[i], '\0'};
            if (result[i] == 1) {
                colorPrint(letter, green);
            } else if (result[i] == 2) {
                colorPrint(letter, yellow);
            } else {
                printf("%s", letter);
            }
        }

        printf("\n");

        // checking for the correct answer:
        if (strcmp(guess, word) == 0) {
            printf("Good job! You got the right answer!");
            return 0;
        }
    }

    printf("Failed! The correct answer was %s.", word);
    return 0;
}